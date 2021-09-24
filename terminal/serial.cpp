#include "serial.h"

Serial::Serial(QObject *parent) : QSerialPort(parent)
{
    connect(this, &QSerialPort::errorOccurred, this, &Serial::handleError);
    connect(this, &QSerialPort::readyRead, this, &Serial::slotRead);
}

Serial::~Serial()
{
    closeSerialPort();
}

bool Serial::openSerialPort(const SettingsDialog::Settings &settings)
{
    setPortName(settings.name);
    setBaudRate(settings.baudRate);
    setDataBits(settings.dataBits);
    setParity(settings.parity);
    setStopBits(settings.stopBits);
    setFlowControl(settings.flowControl);

    if (!open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(nullptr, tr("Error"), errorString());
        Log::write("Error open port");
        return false;
    }

    Log::write("Port opened");

    return true;
}

void Serial::closeSerialPort()
{
    if(isOpen())
    {
        close();
        Log::write("Port closed");
    }
}

void Serial::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError || error == QSerialPort::ReadError)
    {
        QMessageBox::critical(nullptr, tr("Critical Error"), errorString());
        emit signalPortError();
    }
}

void Serial::slotRead()
{
    static QByteArray data;
    QByteArray buffer;

    data += readAll();

    if(!data.contains('\n'))
    {
        if(data.length() > 256)
            Log::write("Buffer overload: " + data, Log::Flags::WRITE_TO_FILE_AND_STDERR);

        return;
    }
    else
    {
        auto index = data.indexOf('\n');
        buffer = data.mid(0, index + 1);
        data = data.mid(index + 1);
    }

    emit signalReadLine(buffer);
}
