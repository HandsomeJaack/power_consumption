#include <QElapsedTimer>
#include <QFuture>
#include <QRunnable>
#include <QThreadPool>
#include <QtMath>

class Worker : public QObject
{
    Q_OBJECT
public:
  Worker(int thread): thread(thread) {}
  ~Worker() = default;

public slots:
    void process() {
        QElapsedTimer timer;
        timer.start();
        QString s = "Executing calculations from dummy_two " +
                QString::number(thread) + " thread";
        qDebug() << s;
        double x = 1.00001, y = 0.001;
        while(timer.elapsed() < 15000) {
            x = qPow(x,y);
        }
        QString f = "Calculation finished for dummy_two " +
                QString::number(thread) + " thread";
        qDebug() << f;
        emit finished();
    }
signals:
    void finished() {}
private:
    int thread;
};

int main()
{
    QThread threads[4];
    Worker* workers[4];
    for(int i = 0; i < 4; ++i) {
        workers[i]->moveToThread(&threads[i]);
    }

    for(int i = 0; i < 4; ++i) {
        QObject::connect(&threads[i], &QThread::started, workers[i], &Worker::process);
        QObject::connect(workers[i], &Worker::finished, &threads[i], &QThread::quit);
        QObject::connect(workers[i], &Worker::finished, workers[i], &Worker::deleteLater);
        QObject::connect(&threads[i], &QThread::finished, &threads[i], &QThread::deleteLater);
    }

    for(int i = 0; i < 4; ++i) {
        threads[i].start();
    }

    return 0;
}

