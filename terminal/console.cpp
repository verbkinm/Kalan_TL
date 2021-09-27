#include "console.h"
#include "ui_console.h"

Console::Console(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);

    connect(ui->send, &QPushButton::clicked, this, &Console::slotSendData);

    setWindowFlags(Qt::WindowStaysOnTopHint);

    QCommonStyle style;
    ui->send->setIcon(style.standardIcon(QStyle::SP_ArrowRight));
    ui->clear->setIcon(style.standardIcon(QStyle::SP_LineEditClearButton));
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
    printText(" >> "  + data);

    data += "\n";
    emit signalSendData(data.toStdString().c_str());
}

void Console::printText(const QString &str)
{
    if(!isVisible())
        return;

    QString buff = str;
    ui->text->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + buff.remove("\n"));
}

void Console::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    ui->text->clear();
}

void Console::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
        close();
}
