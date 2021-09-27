#include "graph/centralwidget.h"
//#include "led.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CentralWidget w;
//    Led w;

    w.showMaximized();

    return a.exec();
}
