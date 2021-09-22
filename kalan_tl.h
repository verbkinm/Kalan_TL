#ifndef KALAN_TL_H
#define KALAN_TL_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <fstream>
#include <iostream>

#include "terminal/serial.h"
#include "log.h"

namespace Ui {
class Kalan_TL;
}

class Kalan_TL : public QWidget
{
    Q_OBJECT

public:
    explicit Kalan_TL(QWidget *parent = nullptr);
    ~Kalan_TL();

private:
    Ui::Kalan_TL *ui;
    Serial _serial;

    void openFile();

    void panelEnable(bool state);

signals:
    void signalOpenFile(const QString &fileName);
    void signalReadLine(const QDateTime &dt, const std::array<float, 4> &arr);

public slots:
    void slotConnect(const SettingsDialog::Settings &settings);

private slots:
    void slotOpenFile();

    void slotSwitchLed();
    void slotSwitchAutoRead();

    void slotReadLine(const QByteArray &data);
};

#endif // KALAN_TL_H
