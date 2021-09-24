#ifndef SERIAL_H
#define SERIAL_H

#include <QSerialPort>
#include <QMessageBox>

#include "SettingsDialog.h"
#include "../log.h"

class Serial : public QSerialPort
{
    Q_OBJECT
public:
    explicit Serial(QObject *parent = nullptr);

public slots:
    bool openSerialPort(const struct SettingsDialog::Settings &settings);

    void handleError(QSerialPort::SerialPortError error);

signals:
    void signalReadLine(const QByteArray &data);

private slots:
    void slotRead();
};

#endif // SERIAL_H
