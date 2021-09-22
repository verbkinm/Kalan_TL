#include "graph/centralwidget.h"

#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CentralWidget w;

    w.showMaximized();

    for (int i = 0; i < 5; i++)
    {
        qsrand(QDateTime::currentMSecsSinceEpoch());
        float t1 = qrand() % ((70 + 1) - 30) + 30;
        float t2 = qrand() % ((70 + 1) - 30) + 30;
        float t3 = qrand() % ((70 + 1) - 30) + 30;
        float t4 = qrand() % ((70 + 1) - 30) + 30;
        std::array<float, 4> arr{ t1, t2, t3, t4 };
        w.addPoints(QDateTime::currentDateTime(), arr);

        QThread::sleep(5);
    }


    return a.exec();
}
