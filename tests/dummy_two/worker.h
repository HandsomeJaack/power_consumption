#ifndef WORKER_H
#define WORKER_H

#include <QElapsedTimer>
#include <QThread>
#include <QtMath>
#include <QDebug>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(int thread): thread(thread) {}
    ~Worker() = default;
public slots:
    void doWork() {
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
    void finished();
private:
    int thread;
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller() = default;
    void init(int i) {
        Worker *worker = new Worker(i);
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &Worker::doWork);
        connect(worker, &Worker::finished, this, [=](){
            QString s = "Thread " + QString::number(i) + " finished";
            qDebug() << s;
        });
        workerThread.start();
    }

    ~Controller() {
        qDebug() << "Controller destroyed";
        workerThread.quit();
        workerThread.wait();
    }
signals:
    void operate();
};

class Test : public QObject
{
    Q_OBJECT
public:
    Test() = default;
    void calc() {
        QScopedArrayPointer<Controller> c(new Controller[4]);
        for(int i = 0; i < 4; ++i) {
            c[i].init(i);
            c[i].operate();
        }
        emit finished();
    }
signals:
    void finished();
};

#endif // WORKER_H
