#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QLineEdit>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsDialog),
    _settings(QSettings::IniFormat, QSettings::UserScope, "VMS", "Kalan_TL")
{
    ui->setupUi(this);

    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->connect, &QPushButton::clicked, this, &SettingsDialog::slotConnect);

    fillPortsParameters();

    updateSettings();

    checkSettingsFileExist();
    readSettings();

    ui->connect->setValue(0);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

SettingsDialog::Settings SettingsDialog::settings() const
{
    return _currentSettings;
}

void SettingsDialog::slotConnected()
{
    panelEnable(false);
    ui->connect->setText("Disconnect");
    ui->connect->setValue(1);
}

void SettingsDialog::slotDisconneced()
{
    panelEnable(true);
    ui->connect->setText("Connect");
    ui->connect->setValue(0);
}

void SettingsDialog::slotConnect()
{
    saveSettings();
    updateSettings();
    emit signalConnect(_currentSettings);
}

void SettingsDialog::fillPortsParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    m_ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SettingsDialog::updateSettings()
{
    _currentSettings.name = ui->devicePath->text();
    _currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    _currentSettings.dataBits = static_cast<QSerialPort::DataBits>(ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    _currentSettings.parity = static_cast<QSerialPort::Parity>(ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    _currentSettings.stopBits = static_cast<QSerialPort::StopBits>(ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    _currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
}

void SettingsDialog::panelEnable(bool state)
{
    ui->devicePath->setEnabled(state);
    ui->baudRateBox->setEnabled(state);
    ui->dataBitsBox->setEnabled(state);
    ui->parityBox->setEnabled(state);
    ui->stopBitsBox->setEnabled(state);
    ui->flowControlBox->setEnabled(state);
}

void SettingsDialog::checkSettingsFileExist()
{
    if(!QFileInfo::exists(_settings.fileName()))
    {
        _settings.beginGroup("Port");
        _settings.setValue("name", "/dev/ttyUSB0");
        _settings.setValue("baudRate", "9600");
        _settings.setValue("dataBits", "8");
        _settings.setValue("parity", "None");
        _settings.setValue("stopBits", "1");
        _settings.setValue("flowControl", "None");
        _settings.endGroup();
        _settings.sync();
    }
}

void SettingsDialog::readSettings()
{
    _settings.beginGroup("Port");
    ui->devicePath->setText(_settings.value("name").toString());
    ui->baudRateBox->setCurrentText(_settings.value("baudRate").toString());
    ui->dataBitsBox->setCurrentText(_settings.value("dataBits").toString());
    ui->parityBox->setCurrentText(_settings.value("parity").toString());
    ui->stopBitsBox->setCurrentText(_settings.value("stopBits").toString());
    ui->flowControlBox->setCurrentText(_settings.value("flowControl").toString());
    _settings.endGroup();
}

void SettingsDialog::saveSettings()
{
    _settings.beginGroup("Port");
    _settings.setValue("name", ui->devicePath->text());
    _settings.setValue("baudRate", ui->baudRateBox->currentText());
    _settings.setValue("dataBits", ui->dataBitsBox->currentText());
    _settings.setValue("parity", ui->parityBox->currentText());
    _settings.setValue("stopBits", ui->stopBitsBox->currentText());
    _settings.setValue("flowControl", ui->flowControlBox->currentText());
    _settings.endGroup();
}

