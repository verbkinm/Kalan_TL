#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QWidget>
#include <QSerialPort>
#include <QSettings>
#include <QFileInfo>

QT_BEGIN_NAMESPACE

namespace Ui {
class SettingsDialog;
}

QT_END_NAMESPACE

class SettingsDialog : public QWidget
{
    Q_OBJECT

public:
    struct Settings
    {
        QString name;
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
    };

    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    Settings settings() const;

signals:
    void signalConnect(const Settings &settings);

public slots:
    void slotConnected();
    void slotDisconneced();

private slots:
    void slotConnect();

private:
    void fillPortsParameters();
    void updateSettings();
    void panelEnable(bool state);

    void checkSettingsFileExist();
    void readSettings();
    void saveSettings();

    Ui::SettingsDialog *ui = nullptr;
    Settings _currentSettings;
    QSettings _settings;
};

#endif // SETTINGSDIALOG_H
