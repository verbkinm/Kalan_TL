#ifndef LED_H
#define LED_H

#include <QWidget>
#include <QPainter>

class Led : public QWidget
{
    Q_OBJECT
public:
    explicit Led(QWidget *parent = nullptr);

    void setValue(int value);
    int value() const;

private:
    Qt::GlobalColor _state;

signals:

protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // LED_H
