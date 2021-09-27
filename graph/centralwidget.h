#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QMainWindow>
#include <QScatterSeries>
#include <QSplineSeries>
#include <QXYSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QMessageBox>

#include "chart.h"
#include "chartview.h"
#include "panelwidget.h"
#include "range.h"
#include "log.h"
#include "callout.h"

QT_CHARTS_USE_NAMESPACE

class CentralWidget : public QMainWindow
{
    Q_OBJECT
public:
    CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();

    void addSeries(std::map<QDateTime, float> &map, QString legendTitle = "");
    void openFile(const QString &fileName);

    void setTitle(QString title);

    void addPoints(const QDateTime &dt, const std::array<float, 4> &arr);

    QString getTitle() const;

private:
    QWidget _centralWidget;
    QLabel _statusBarWidget;

    ChartView _chartView;
    Chart _chart;

    QHBoxLayout _layout;

    PanelWidget _panelWidget;

    QDateTimeAxis* _pAxisX;
    QValueAxis* _pAxisY;

    Range<QDateTime> _rangeX;
    Range<float> _rangeY;

    QMenu _menuFile, _menuView, _menuTool, _menuAbout;

    Callout _tooltip;

    void addSeries(const std::map<QDateTime, float> &map, QXYSeries::SeriesType type = QXYSeries::SeriesTypeLine,
                   QString legendTitle = "Title");

    void createAxes();
    void createMenu();
    QXYSeries *createSeriesAccordingType(QXYSeries::SeriesType &type);

    void connectPanelWidgetSignals();
    void fillSeriesAndSetRanges(const std::map<QDateTime, float> &map, QXYSeries *series);

    void setChartViewXYRange();

private slots:
    void slotSetTheme(int);
    void slotSetLegentPosition(int);
    void slotAnimation(bool);
    void slotAntialiasing(bool);

    void slotSetTcickCountX(int);
    void slotSetTcickCountY(int);

    void slotSaveBMP();
    void slotResetZoomAndPosition();
    void slotViewXYCoordinate(QPointF);
    void slotSetRubberMode(QChartView::RubberBand);

    void slotRangeXChanged(QDateTime, QDateTime);
    void slotRangeYChanged(qreal, qreal);
    void slotRangeXSet(QDateTime, QDateTime);
    void slotRangeYSet(qreal, qreal);

    void slotDoubleClick();
    void slotFullScreen();

    void slotTooltip(QPointF point, bool state);
    void slotSeriesRecreated();

    void slotOpenFile();
    void slotAuthor();
};

#endif // CENTRALWIDGET_H
