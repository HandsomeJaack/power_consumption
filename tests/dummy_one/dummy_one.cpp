#include <QElapsedTimer>
#include <QFuture>
#include <QRunnable>
#include <QThreadPool>
#include <QtMath>

class Worker : public QRunnable
{
    void run() {
        QElapsedTimer *timer = new QElapsedTimer();
        timer->start();

        qDebug() << "Executing calculations from dummy_one";
        double x = 1.00001, y = 0.001;
        while(timer->elapsed() < 15000) {
            x = qPow(x,y);
        }
        qDebug() << "Calculation finished for dummy_one";
    }
};

int main()
{
    Worker *worker = new Worker();
    QThreadPool::globalInstance()->start(worker);
    return 0;
}
