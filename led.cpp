#include "led.h"

Led::Led(QWidget *parent) : QWidget(parent), _state(Qt::red)
{
}

void Led::setValue(int value)
{
    if(value == 0)
        _state = Qt::red;
    else if (value == 1)
        _state = Qt::green;

    repaint();
}

int Led::value() const
{
    if(_state == Qt::green)
        return 1;

    return 0;
}

void Led::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setBrush(QBrush(_state));
    painter.setPen((QPen(Qt::black, 1)));

    int size = width() < height() ? width() - 20 : height() - 20;
    painter.drawRoundedRect(QRect(width() / 2 - size / 2, height() / 2 - size / 2, size, size), size, size);

}
