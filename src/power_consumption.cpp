#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QDate>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QPair>
#include <QtQml>

#include "power_consumption.h"
#define BAT_STAT "/home/nemo/Downloads/total_stat"

static QPair<QString, int> parseString(QString line)
{
    QStringList list = line.split('|');
    return qMakePair(list[0], list[1].toInt());
}

Chart::Chart(QObject *parent): QObject(parent)
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

Chart::~Chart()
{
}

QList<QString> Chart::getTime()
{
    return time;
}

QList<int> Chart::getPercentage()
{
    return percentage;
}

int main(int argc, char *argv[])
{
    qmlRegisterType<Chart>("battery.chart", 1, 0, "Chart");
    return SailfishApp::main(argc, argv);
}

#include "moc_power_consumption.cpp"
