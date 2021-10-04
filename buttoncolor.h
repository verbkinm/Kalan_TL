#ifndef BUTTONCOLOR_H
#define BUTTONCOLOR_H

#include <QPushButton>
#include <QTimer>

class ButtonColor : public QPushButton
{
    Q_OBJECT

public:
    ButtonColor(QWidget *parent = nullptr);

    void setValue(int value);
    int value() const;

    void timerStart();
    void timerStop();

private:
    int _value;
    QTimer _timer;

    QColor _defaultColor;

signals:
    void valueChanged(int value);

private slots:
    void slotTimerOut();
};

#endif // BUTTONCOLOR_H
