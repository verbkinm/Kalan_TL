#ifndef CHART_H
#define CHART_H

#include <QtWidgets/QGesture>
#include <QtCharts/QChart>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Chart : public QChart
{
public:
    Chart(QGraphicsItem *parent = nullptr);
    ~Chart();

protected:
    bool sceneEvent(QEvent *event);

private:
    bool gestureEvent(QGestureEvent *event);

};

#endif // CHART_H
