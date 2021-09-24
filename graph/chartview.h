#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

#include <QDateTime>
#include "range.h"

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
    Q_OBJECT
public:
    ChartView(QWidget *parent = nullptr);

    void setRangeDefault(const Range<QDateTime> &rangeX, const Range<float> &rangeY);

public slots:
    void slotResetZoomAndPosition();

protected:
    bool vieportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool event(QEvent* ev) override;

private:
    QPointF _lastMousePos;

    Range<QDateTime> _rangeX;
    Range<float> _rangeY;

signals:
    void signalMousePosition(QPointF);
    void signalDoubleClick();

};

#endif
