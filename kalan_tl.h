#ifndef KALAN_TL_H
#define KALAN_TL_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QTimer>

#include "terminal/serial.h"
#include "terminal/console.h"
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
    Console _console;
    QTimer _timer;

    void openFile();

    void panelEnable(bool state);

signals:
    void signalReadLine(const QDateTime &dt, const std::array<float, 4> &arr);

    void signalConnected();
    void signalDisconnected();

public slots:
    void slotConnect(const SettingsDialog::Settings &settings);
    void slotPortError();
    void slotConsoleShow();

private slots:
    void slotSwitchLed();
    void slotSwitchAutoRead();

    void slotReadLine(const QByteArray &data);
    void slotWriteLine(const QByteArray &data);

    void slotAutoReadChanged(int value);
    void slotTimerOut();

    // QWidget interface
protected:
    virtual void hideEvent(QHideEvent *event) override;
};

#endif // KALAN_TL_H
