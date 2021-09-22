#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QLineEdit>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::SettingsDialog)
{
    _ui->setupUi(this);

    _ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(_ui->connect, &QPushButton::clicked, this, &SettingsDialog::slotConnect);

    fillPortsParameters();

    updateSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete _ui;
}

SettingsDialog::Settings SettingsDialog::settings() const
{
    return _currentSettings;
}

void SettingsDialog::slotConnect()
{
    updateSettings();
    emit signalConnect(_currentSettings);
}

void SettingsDialog::fillPortsParameters()
{
    _ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    _ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    _ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    _ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    _ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    _ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    _ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    _ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    _ui->dataBitsBox->setCurrentIndex(3);

    _ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    _ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    _ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    _ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    _ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    _ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    m_ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    _ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    _ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    _ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    _ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SettingsDialog::updateSettings()
{
    _currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(_ui->baudRateBox->itemData(_ui->baudRateBox->currentIndex()).toInt());
    _currentSettings.dataBits = static_cast<QSerialPort::DataBits>(_ui->dataBitsBox->itemData(_ui->dataBitsBox->currentIndex()).toInt());
    _currentSettings.parity = static_cast<QSerialPort::Parity>(_ui->parityBox->itemData(_ui->parityBox->currentIndex()).toInt());
    _currentSettings.stopBits = static_cast<QSerialPort::StopBits>(_ui->stopBitsBox->itemData(_ui->stopBitsBox->currentIndex()).toInt());
    _currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(_ui->flowControlBox->itemData(_ui->flowControlBox->currentIndex()).toInt());
}

