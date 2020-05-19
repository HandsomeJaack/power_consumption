#include <QDirIterator>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include <QVector>
#include <QPair>
// Neccessary for sysconf
#include <unistd.h>
#include "cpustat.h"

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
    // track child processes if neccessary
    cutime = total_stat[15].toDouble()/sysconf(_SC_CLK_TCK);
    cstime = total_stat[16].toDouble()/sysconf(_SC_CLK_TCK);
    starttime = total_stat[21].toDouble()/sysconf(_SC_CLK_TCK);
    cpu_usage = utime + stime;
    QFile uidFile("/proc/" + QString::number(pid) + "/status");
    uidFile.open(QIODevice::ReadOnly);
    QString status = QTextCodec::codecForName("UTF-8")->toUnicode(uidFile.readAll());
    uidFile.close();

    QStringList lines = status.split('\n');
    QString uidLine = lines[6].split(':').last();
    uid = uidLine.split('\t').last().toInt();
}


QString Process::getProcessName()
{
    return app_name;
}

double Process::getCpuUsage()
{
    return cpu_usage;
}

int Process::getPid()
{
    return pid;
}

int Process::getUserId()
{
    return uid;
}

Program::Program(QString name) : name(name) {}

double Program::getCpuUsage()
{
    double program_cpu_usage = 0;
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

int Program::getUserId()
{
    return pids.first().getUserId();
}

void Usage::getStats()
{
    QDirIterator it(PIDS_PATH, QDirIterator::NoIteratorFlags);
    // list pids in /proc/[pid]/dir
    while(it.hasNext()) {
        QFile file(it.next());
        QString filename = file.fileName();
        filename.remove(0,6);

        // check if folder is [pid]
        bool ok;
        int pid = filename.toInt(&ok);
        if(ok) {
            Process proc(pid);
            // add pid to the list of pids
            pids.push_back(proc);

            bool pid_already_exists = false;
            int pos = 0;

            // check if [pid] with [appname] already exists
            for( ; pos < programs.size(); ++pos) {
                if(programs[pos].getName() == proc.getProcessName()) {
                    pid_already_exists = true;
                    break;
                }
            }
            // if [pid] with [appname] exists add [pid] to list of [appname] pids
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

QVector<QPair<double, QString>> Usage::totalProgramUsage()
{
    QVector<QPair<double, QString>> programList;
    QFile totalUsage("/home/nemo/Downloads/cpuusage");
    totalUsage.open(QIODevice::WriteOnly);

    QPair<double, QString> system = {0, "System"};
    QTextStream out(&totalUsage);

    double total_cpu_usage = 0;
    for(auto &program: programs) {
        total_cpu_usage += program.getCpuUsage();
    }

    for(auto &program: programs) {
        double percentage = program.getCpuUsage()/total_cpu_usage*100;
        QString appName = program.getName();

        if(program.getUserId() == 0) {
            system.first+= percentage;
            continue;
        }

        if(percentage < 0.01)
            continue;

        out << appName + ": " + QString::number(percentage, 'f', 3) + "\n";
        programList.append({percentage,appName});
    }
    out << system.second + ": " + system.first + "\n";
    programList.append(system);
    totalUsage.close();
    return programList;
}
