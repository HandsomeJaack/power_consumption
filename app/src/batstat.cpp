#include <QDate>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QPair>

#include "batstat.h"
#define BAT_STAT "/home/nemo/Downloads/total_stat"

static QPair<QString, int> parseString(QString line)
{
    QStringList list = line.split('|');
    return qMakePair(list[0], list[1].toInt());
}

BatteryStatistics::BatteryStatistics(QObject *parent): QObject(parent)
{
   QFile bat_stat(BAT_STAT);
   if (!bat_stat.open(QIODevice::ReadOnly | QIODevice::Text)) {
       qDebug() << "Unable to open stat file!\n";
   }

   QTextStream stream(&bat_stat);
   QString line;
   while( !(line = stream.readLine()).isNull())
   {
       QPair<QString, int> parsedLine = parseString(line);
       time.append(parsedLine.first);
       percentage.append(parsedLine.second);
   }
}

BatteryStatistics::~BatteryStatistics()
{
}

QStringList BatteryStatistics::getTime()
{
    return time;
}

QList<int> BatteryStatistics::getPercentage()
{
    return percentage;
}
