#include "worker.h"
#include <QScopedPointer>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Test t;
    t.calc();
    QObject::connect(&t, &Test::finished, &a, &QCoreApplication::quit);
    a.exec();
    return 0;
}

