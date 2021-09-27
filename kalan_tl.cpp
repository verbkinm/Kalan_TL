#include "kalan_tl.h"
#include "ui_kalan_tl.h"

#define AUTOREAD_INTERVAL 20000

Kalan_TL::Kalan_TL(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Kalan_TL)
{
    ui->setupUi(this);

    connect(ui->switchLed, &QPushButton::clicked, this, &Kalan_TL::slotSwitchLed);
    connect(ui->switchAutoRead, &QPushButton::clicked, this, &Kalan_TL::slotSwitchAutoRead);

    connect(&_console, &Console::signalSendData, this, &Kalan_TL::slotWriteLine);
    connect(&_serial, &Serial::signalReadLine, this, &Kalan_TL::slotReadLine);
    connect(&_serial, &Serial::signalPortError, this, &Kalan_TL::slotPortError);

    connect(ui->autoReadState, SIGNAL(valueChanged(int)), this, SLOT(slotAutoReadChanged(int)));

    connect(&_timer, &QTimer::timeout, this, &Kalan_TL::slotTimerOut);

    _timer.setInterval(AUTOREAD_INTERVAL);
}

Kalan_TL::~Kalan_TL()
{
    delete ui;
}

void Kalan_TL::panelEnable(bool state)
{
    ui->switchLed->setEnabled(state);
    ui->switchAutoRead->setEnabled(state);

    _console.enableInput(state);
}

void Kalan_TL::slotConnect(const SettingsDialog::Settings &settings)
{
    if(_serial.isOpen())
    {
        _serial.closeSerialPort();
        panelEnable(false);

        ui->ledState->setValue(0);
        ui->autoReadState->setValue(0);

        emit signalDisconnected();
    }
    else if(_serial.openSerialPort(settings))
    {
        panelEnable(true);
        emit signalConnected();
    }
}

void Kalan_TL::slotPortError()
{
    _serial.closeSerialPort();
    panelEnable(false);

    ui->ledState->setValue(0);
    ui->autoReadState->setValue(0);

    emit signalDisconnected();
}

void Kalan_TL::slotSwitchLed()
{
    QString data = "led=" + QString::number(!ui->ledState->value()) + "\n";
    _serial.write(data.toStdString().c_str());
    _console.outputData(data);
    ui->ledState->setValue(!ui->ledState->value());
    Log::write("Output data: " + data.remove("\n"));
}

void Kalan_TL::slotSwitchAutoRead()
{
    QString data = "on=" + QString::number(!ui->autoReadState->value()) + "\n";
    _serial.write(data.toStdString().c_str());
    _console.outputData(data);
    ui->autoReadState->setValue(!ui->autoReadState->value());
    Log::write("Output data: " + data.remove("\n"));
}

void Kalan_TL::slotReadLine(const QByteArray &data)
{
    QString dataStr = data;
    std::array<float, 4> arr;
    int ledState;

    _console.inputData(data);
    Log::write("Input data: " + dataStr.remove("\n"));

    QStringList strList = dataStr.split(' ');

    if(dataStr.startsWith("DS18:") && strList.length() == 6)
    {
        arr[0] = strList.at(1).toFloat();
        arr[1] = strList.at(2).toFloat();
        arr[2] = strList.at(3).toFloat();
        arr[3] = strList.at(4).toFloat();
        ledState = strList.at(5).toInt();

        ui->autoReadState->setValue(1);
        slotAutoReadChanged(1);
        ui->ledState->setValue(ledState);

        QDateTime dt = QDateTime::currentDateTime();
        Log::write(dataStr.remove("\n"), Log::Flags::WRITE_TO_FILE_ONLY, dt.toString("yyyy-MM-dd") + ".txt");

        emit signalReadLine(dt, arr);
    }
}

void Kalan_TL::slotWriteLine(const QByteArray &data)
{
    _serial.write(data);
    Log::write("Output data: " + data);
}

void Kalan_TL::slotConsoleShow()
{
    if(_console.isVisible())
        _console.close();
    else
        _console.show();
}

void Kalan_TL::slotAutoReadChanged(int value)
{
    if(value == 1)
        _timer.start();
    else
        _timer.stop();
}

void Kalan_TL::slotTimerOut()
{
    ui->autoReadState->setValue(0);
}

void Kalan_TL::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    _console.close();
}
