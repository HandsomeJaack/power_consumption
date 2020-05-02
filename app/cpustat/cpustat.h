#ifndef CPUSTAT_H
#define CPUSTAT_H

#define PID_STAT_PATH "/proc/%u/stat"
#define PIDS_PATH "/proc"

#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>

class Process
{
    public:
        Process() = default;
        Process(int pid);

         double getCpuUsage();
         QString getProcessName();
         int getPid();
         int getUserId();
    private:
        // all time calculates in SECONDS, NOT TICKS
        int pid;
        int uid;
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
        int getUserId();
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
        QVector<QPair<double, QString>> totalProgramUsage();
    private:
        QVector<Process> pids;
        QVector<Program> programs;
};

#endif // CPUSTAT_H
