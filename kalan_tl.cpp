#include "kalan_tl.h"
#include "ui_kalan_tl.h"

Kalan_TL::Kalan_TL(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Kalan_TL)
{
    ui->setupUi(this);

    connect(ui->openFile, &QPushButton::clicked, this, &Kalan_TL::slotOpenFile);

    connect(ui->switchLed, &QPushButton::clicked, this, &Kalan_TL::slotSwitchLed);
    connect(ui->switchAutoRead, &QPushButton::clicked, this, &Kalan_TL::slotSwitchAutoRead);

    connect(&_serial, &Serial::signalReadLine, this, &Kalan_TL::slotReadLine);
}

Kalan_TL::~Kalan_TL()
{
    delete ui;
}

void Kalan_TL::panelEnable(bool state)
{
    ui->ledState->setEnabled(state);
    ui->autoReadState->setEnabled(state);
    ui->switchLed->setEnabled(state);
    ui->switchAutoRead->setEnabled(state);
    ui->filePath->setEnabled(!state);
    ui->openFile->setEnabled(!state);
}

void Kalan_TL::slotConnect(const SettingsDialog::Settings &settings)
{
    if(_serial.isOpen())
    {
       _serial.close();
        panelEnable(false);
    }
    else if(_serial.openSerialPort(settings))
        panelEnable(true);
}

void Kalan_TL::slotOpenFile()
{
    QString result = QFileDialog::getOpenFileName(this, "Select file", "/home", /*ui->filePath->text()*/ "TXT(*.txt)");
    if(!result.isEmpty())
    {
        ui->filePath->setText(result);
        emit signalOpenFile(result);
    }
}

void Kalan_TL::slotSwitchLed()
{
    disconnect(&_serial, &Serial::signalReadLine, this, &Kalan_TL::slotReadLine);

    QString data = "led=" + QString::number(!ui->ledState->value()) + "\n";
    _serial.write(data.toStdString().c_str());

    QByteArray dataIn = _serial.readLine();

    if( (dataIn.length() == 2) && (dataIn.at(1) == 1) )
        ui->ledState->setValue(!ui->ledState->value());

    connect(&_serial, &Serial::signalReadLine, this, &Kalan_TL::slotReadLine);
}

void Kalan_TL::slotSwitchAutoRead()
{
    disconnect(&_serial, &Serial::signalReadLine, this, &Kalan_TL::slotReadLine);

    QString data = "on=" + QString::number(!ui->autoReadState->value()) + "\n";
    _serial.write(data.toStdString().c_str());

    QByteArray dataIn = _serial.readLine();

    if( (dataIn.length() == 2) && (dataIn.at(0) == 1))
        ui->autoReadState->setValue(!ui->autoReadState->value());

    connect(&_serial, &Serial::signalReadLine, this, &Kalan_TL::slotReadLine);
}

void Kalan_TL::slotReadLine(const QByteArray &data)
{
    std::array<float, 4> arr;
    int ledState;

    QString dataStr = data;
    QStringList strList = dataStr.split(' ');

    if(!dataStr.startsWith("DS18:") || strList.length() < 6)
    {
        Log::write("Error data: " + dataStr, Log::Flags::WRITE_TO_FILE_AND_STDERR);
        return;
    }

    arr[0] = strList.at(1).toFloat();
    arr[1] = strList.at(2).toFloat();
    arr[2] = strList.at(3).toFloat();
    arr[3] = strList.at(4).toFloat();
    ledState = strList.at(5).toInt();

    ui->ledState->setValue(ledState);

    QDateTime dt = QDateTime::currentDateTime();
    Log::write(dataStr, Log::Flags::WRITE_TO_FILE_ONLY, dt.toString("dd-MM-yyy") + ".txt");

    emit signalReadLine(dt, arr);
}

