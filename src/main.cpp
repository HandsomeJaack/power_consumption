#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QtQml>
#include "plotview.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<PlotView>("powerstat.plotview", 1, 0, "PlotView");
    return SailfishApp::main(argc, argv);
}
