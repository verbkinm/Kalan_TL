#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QScrollBar>
#include <QDateTime>

namespace Ui {
class Console;
}

class Console : public QWidget
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = nullptr);
    ~Console();

    void inputData(const QString &data);
    void outputData(const QString &data);

    void enableInput(bool state);

signals:
    void signalSendData(const QByteArray &data);

private slots:
    void slotSendData();

private:
    Ui::Console *ui;

    void printText(const QString &str);

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // CONSOLE_H
