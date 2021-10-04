#include "buttoncolor.h"

ButtonColor::ButtonColor(QWidget *parent) : QPushButton(parent),
    _value(0), _defaultColor(palette().window().color())
{
    connect(&_timer, &QTimer::timeout, this, &ButtonColor::slotTimerOut);
}

void ButtonColor::setValue(int value)
{
    _value = value;

    emit valueChanged(value);

    QColor color(Qt::red);

    if(value == 1)
        color.setRgb(0, 255, 0);

    QPalette pal = palette();
    pal.setColor(backgroundRole(), color);
    setAutoFillBackground(true);
    setPalette(pal);
    update();
}

int ButtonColor::value() const
{
    return _value;
}

void ButtonColor::timerStart()
{
    _timer.start(1000);
}

void ButtonColor::timerStop()
{
    _timer.stop();
}

void ButtonColor::slotTimerOut()
{
    if(!isEnabled())
        return;

    static int timerStep = 0;
    QColor color;

    if(timerStep == 1)
        color = _defaultColor;
    else if(_value == 0)
        color.setRgb(255, 0, 0);
    else if(_value == 1)
        color.setRgb(0, 255, 0);

    timerStep = !timerStep;

    QPalette pal = palette();
    pal.setColor(backgroundRole(), color);
    setAutoFillBackground(true);
    setPalette(pal);
    update();
}
