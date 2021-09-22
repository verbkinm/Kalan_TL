#ifndef PANELWIDGET_H
#define PANELWIDGET_H

#include <QWidget>
#include <QLineSeries>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QSlider>
#include <QVXYModelMapper>
#include <QDateTime>

#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class PanelWidget;
}

class PanelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PanelWidget(QWidget *parent = nullptr);
    ~PanelWidget();

    void addSeriesInList(QXYSeries* series);
    int getSeriesType();

    void setRangeAxisX(QDateTime min, QDateTime maxQDateTime);
    void setRangeAxisY(qreal min, qreal max);

    void removeAllSeries();

private:
    Ui::PanelWidget *ui;

    QList<QXYSeries*> _seriesList;

    QXYSeries* _currentSeries;

    void recreateSeries(int numberSeries, int seriesType);
    QXYSeries *newSeries(int seriesType);
    //отключение всех необходимых элементов, если список графов пуст!
    void setDisableUnits(bool value);

signals:
    void signalChangeTheme(int);
    void signalSetLegendPosition(int);

    void signalAnimation(bool);
    void signalAntialiasing(bool);

    void signalTickCountChangeX(int);
    void signalTickCountChangeY(int);

    void signalSeriesDeleted();

    void signalRubberMode(QChartView::RubberBand);

    void signalAxisXRangeChanged(QDateTime, QDateTime);
    void signalAxisYRangeChanged(qreal, qreal);

    void signalSeriesTypeChanged();

    void signalOpenFile(const QString &fileName);
    void signalReadLine(const QDateTime &dt, const std::array<float, 4> &arr);

private slots:
    void slotSetSeriesPropery(int value);
    void slotSetPointVisible(bool value);

    void slotSetSeriesColor();
    void slotSetSeriesType(int seriesType);
    void slotHideSeries(bool value);
    void slotSetSeriesPenWidth(int value);

    void on_actionSetRubberMode_triggered();

    void slotAxisXRangeChanged();
    void slotAxisYRangeChanged();
};

#endif // PANELWIDGET_H
