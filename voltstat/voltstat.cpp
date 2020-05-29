#define CUR_VOLTAGE "/sys/class/power_supply/battery/voltage_now"
#define VOLTAGE_STAT "/home/nemo/Downloads/voltage_stat"

#include <QFile>
#include <QString>
#include <QTime>
#include <QTextStream>
#include <QDateTime>

class VoltageLevelUpdater
{
public:
    VoltageLevelUpdater() = default;
    ~VoltageLevelUpdater() = default;
    void update();
};

void VoltageLevelUpdater::update()
{
    QFile volLevel(CUR_VOLTAGE);
    volLevel.open(QIODevice::ReadOnly);
    QTextStream in(&volLevel);
    QString level = in.readLine();
    volLevel.close();

    QFile voltageStat(VOLTAGE_STAT);
    voltageStat.open(QIODevice::WriteOnly | QIODevice::Append);

    QString now = QDateTime::currentDateTime().toString("dd MMM hh:mm");
    QString measurement = now + " " + level + "\n";

    QTextStream out(&voltageStat);
    out << measurement;
    voltageStat.close();
}

int main() {
    VoltageLevelUpdater vol;
    vol.update();
    return 0;
}
