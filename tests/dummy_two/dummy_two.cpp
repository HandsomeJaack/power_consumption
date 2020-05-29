#include <QElapsedTimer>
#include <QFuture>
#include <QRunnable>
#include <QThreadPool>
#include <QtMath>

class Worker : public QRunnable
{
    int thread;
    void run() {
        QElapsedTimer *timer = new QElapsedTimer();
        timer->start();
        QString s = "Executing calculations from dummy_two " +
                QString::number(thread) + " thread";
        qDebug() << s;
        double x = 1.00001, y = 0.001;
        while(timer->elapsed() < 15000) {
            x = qPow(x,y);
        }
        QString f = "Calculation finished for dummy_two " +
                QString::number(thread) + " thread";
        qDebug() << f;
    }
public:
  Worker(int thread): thread(thread) {}

};

int main()
{
    Worker worker1(1);
    Worker worker2(2);
    Worker worker3(3);
    Worker worker4(4);
    QThreadPool *threadPool = new QThreadPool();
    threadPool->start(&worker1);
    threadPool->start(&worker2);
    threadPool->start(&worker3);
    threadPool->start(&worker4);
    return 0;
}
