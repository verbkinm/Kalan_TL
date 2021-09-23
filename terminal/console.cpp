#include "console.h"
#include "ui_console.h"

Console::Console(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);

    connect(ui->send, &QPushButton::clicked, this, &Console::slotSendData);

    setWindowFlags(Qt::WindowStaysOnTopHint);
}

Console::~Console()
{
    delete ui;
}

void Console::inputData(const QString &data)
{
    printText(" << "  + data);
}

void Console::outputData(const QString &data)
{
    printText(" >> " + data);
}

void Console::enableInput(bool state)
{
    ui->dataToSend->setEnabled(state);
    ui->send->setEnabled(state);
}

void Console::slotSendData()
{
    QString data = ui->dataToSend->text();
    printText(" >> "  + data + "\n");

    emit signalSendData(data.toStdString().c_str());
}

void Console::printText(const QString &str)
{
    ui->text->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + str);
}
