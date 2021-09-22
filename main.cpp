#include "graph/centralwidget.h"

#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CentralWidget w;

    w.showMaximized();

    return a.exec();
}
