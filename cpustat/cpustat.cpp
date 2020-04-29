#include <QDirIterator>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include <QVector>

#define PID_STAT_PATH "/proc/%u/stat"
#define PIDS_PATH "/proc"

// Neccessary for sysconf
#include <unistd.h>

class Process
{
    public:
        Process() = default;
        Process(int pid);

         double getCpuUsage();
         QString getProcessName();
         int getPid();
    private:
        // all time calculates in SECONDS, NOT TICKS
        int pid;
        QString app_name;
        QStringList total_stat;
        int utime;
        int stime;
        int cutime;
        int cstime;
        unsigned long starttime;
        // percentage
        double cpu_usage;
};

class Program
{
    public:
        Program() = default;
        Program(QString name);

        double getCpuUsage();
        QString getName();
        void addPid(int pid);
        void listPids();
    private:
        QString name;
        QVector<Process> pids;
};

class Usage
{
    public:
        Usage() = default;
        void getStats();
        void totalProgramUsage();
    private:
        QVector<Process> pids;
        QVector<Program> programs;
};

Process::Process(int pid) : pid(pid)
{

    QFile filename("/proc/" + QString::number(pid) + "/stat");
    filename.open(QIODevice::ReadOnly);

    QString statistics = QTextCodec::codecForName("UTF-8")->toUnicode(filename.readAll());
    filename.close();

    total_stat = statistics.split(' ');

    app_name = total_stat[1];

    if(app_name[0] == '('){
        app_name.remove(0,1);
    }

    if(app_name[app_name.size()-1] == ')') {
        app_name.remove(app_name.size()-1, 1);
    }

    // calculates seconds
    utime = total_stat[13].toDouble()/sysconf(_SC_CLK_TCK);
    stime = total_stat[14].toDouble()/sysconf(_SC_CLK_TCK);
    cutime = total_stat[15].toDouble()/sysconf(_SC_CLK_TCK);
    cstime = total_stat[16].toDouble()/sysconf(_SC_CLK_TCK);
    starttime = total_stat[21].toDouble()/sysconf(_SC_CLK_TCK);
}

static double now()
{
    double uptime_seconds;
    // file contains two numbers in seconds: system uptime and idle time
    QFile uptime("/proc/uptime");
    uptime.open(QIODevice::ReadOnly);
    QTextStream tmp(&uptime);
    QStringList up = tmp.readLine().split(' ');
    uptime_seconds = up[0].toDouble();
    uptime.close();

    return uptime_seconds;
}

QString Process::getProcessName()
{
    return app_name;
}

double Process::getCpuUsage()
{
    cpu_usage = (utime + stime + cutime + cstime)/(now() - starttime);
    return cpu_usage;
}

int Process::getPid()
{
    return pid;
}

Program::Program(QString name) : name(name) {}

double Program::getCpuUsage()
{
    double program_cpu_usage;
    for(auto& pid: pids) {
        program_cpu_usage += pid.getCpuUsage();
    }
    return program_cpu_usage;
}

QString Program::getName()
{
    return name;
}

void Program::addPid(int pid)
{
    pids.push_back(pid);
}

void Usage::getStats()
{
    QDirIterator it(PIDS_PATH, QDirIterator::NoIteratorFlags);
    while(it.hasNext()) {
        QFile file(it.next());
        QString filename = file.fileName();
        filename.remove(0,6);

        bool ok;
        int pid = filename.toInt(&ok);
        if(ok) {
            Process proc(pid);
            pids.push_back(proc);

            bool pid_already_exists = false;
            int pos = 0;

            for( ; pos < programs.size(); ++pos) {
                if(programs[pos].getName() == proc.getProcessName()) {
                    pid_already_exists = true;
                    break;
                }
            }

            if(pid_already_exists){
                programs[pos].addPid(pid);
            } else {
                Program prog(proc.getProcessName());
                prog.addPid(pid);
                programs.push_back(prog);
            }
        }
    }
}

void Usage::totalProgramUsage()
{
    QFile totalUsage("/home/nemo/Downloads/cpuusage");
    totalUsage.open(QIODevice::WriteOnly);

    QTextStream out(&totalUsage);
    for(auto &program: programs) {
        out << program.getName() + ": " + QString::number(program.getCpuUsage(), 'f', 3) + "\n";
    }
    totalUsage.close();
}

int main()
{
    Usage u;
    u.getStats();
    u.totalProgramUsage();
}
