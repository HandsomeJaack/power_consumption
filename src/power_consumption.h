#ifndef POWER_CONSUMPTION_H
#define POWER_CONSUMPTION_H

#include <QObject>
#include <QString>
#include <QList>

class Chart : public QObject
{
    Q_OBJECT
public:
    explicit Chart(QObject *parent = 0);
    ~Chart();
    Q_INVOKABLE QList<QString> getTime();
    Q_INVOKABLE QList<int> getPercentage();
private:
    QList<QString> time;
    QList<int> percentage;
};

#endif // POWER_CONSUMPTION_H
