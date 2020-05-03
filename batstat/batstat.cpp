#define CUR_BAT_LEVEL "/sys/class/power_supply/battery/capacity"
#define BAT_STATUS "/sys/class/power_supply/battery/status"
#define TOTAL_STAT "/home/nemo/Downloads/total_stat"

#include <QFile>
#include <QString>
#include <QTime>
#include <QTextStream>
#include <QDateTime>

class BatteryLevelUpdater
{
public:
    BatteryLevelUpdater() = default;
    ~BatteryLevelUpdater() = default;
    void update();
};

void BatteryLevelUpdater::update()
{
    QFile batLevel(CUR_BAT_LEVEL);
    batLevel.open(QIODevice::ReadOnly);
    QTextStream in(&batLevel);
    QString level = in.readLine();
    batLevel.close();

    QFile totalStat(TOTAL_STAT);
    if(level.toInt() < 100) {
        totalStat.open(QIODevice::WriteOnly | QIODevice::Append);
    } else {
        QFile deviceStatus(BAT_STATUS);
        QTextStream rewrite(&deviceStatus);
        QString status = rewrite.readLine();
        deviceStatus.close();

        if(status == "Charging") {
            totalStat.open(QIODevice::WriteOnly | QIODevice::Append);
        } else {
            totalStat.open(QIODevice::WriteOnly);
        }
    }

    QString now = QDateTime::currentDateTime().toString("dd MMM hh:mm");
    QString measurement = now + "|" + level + "\n";

    QTextStream out(&totalStat);
    out << measurement;
    totalStat.close();
}

int main() {
    BatteryLevelUpdater bat;
    bat.update();
    return 0;
}
