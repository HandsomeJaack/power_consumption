#ifndef POWER_CONSUMPTION_H
#define POWER_CONSUMPTION_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>

class BatteryStatistics : public QObject
{
    Q_OBJECT
public:
    explicit BatteryStatistics(QObject *parent = 0);
    ~BatteryStatistics();
    Q_INVOKABLE QStringList getTime();
    Q_INVOKABLE QList<int> getPercentage();
private:
    QList<QString> time;
    QList<int> percentage;
};

#endif // POWER_CONSUMPTION_H
