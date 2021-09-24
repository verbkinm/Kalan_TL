#include "centralwidget.h"

#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QStatusBar>
#include <limits>

#include <QApplication>

#define FONT_SIZE 18

CentralWidget::CentralWidget(QWidget *parent) : QMainWindow(parent),
    _pAxisX(nullptr), _pAxisY(nullptr)
{
    _rangeX.setMin(QDateTime(QDate(3000, 1, 1), QTime(0, 0, 0)));

    _panelWidget.setParent(this);
    createMenu();
    statusBar()->insertWidget(0, &_statusBarWidget);

    QFont font = _chart.titleFont();
    font.setPixelSize(FONT_SIZE);
    _chart.setTitleFont(QFont(font));

    createAxes();
    _chart.setDropShadowEnabled(true);

    _chartView.setChart(&_chart);

    _layout.addWidget(&_chartView);
    _layout.addWidget(&_panelWidget);
    _layout.setStretch(0, 1);

    _centralWidget.setLayout(&_layout);
    setCentralWidget(&_centralWidget);
    _chartView.setFocus();

    connectPanelWidgetSignals();
    connect(&_chartView, &ChartView::signalMousePosition, this, &CentralWidget::slotViewXYCoordinate);
    connect(&_chartView, &ChartView::signalDoubleClick, this, &CentralWidget::slotDoubleClick);

    connect(_pAxisX, SIGNAL(rangeChanged(QDateTime, QDateTime)), this, SLOT(slotRangeXChanged(QDateTime, QDateTime)));
    connect(_pAxisY, SIGNAL(rangeChanged(qreal, qreal)), this, SLOT(slotRangeYChanged(qreal, qreal)));

    connect(&_panelWidget, &PanelWidget::signalOpenFile, this, &CentralWidget::openFile);
    connect(&_panelWidget, &PanelWidget::signalReadLine, this, &CentralWidget::addPoints);

    openFile(QDate::currentDate().toString("yyyy-MM-dd") + ".txt");
}

CentralWidget::~CentralWidget()
{
    delete _pAxisX;
    delete _pAxisY;
}

void CentralWidget::addSeries(const std::map<QDateTime, float> &map, QXYSeries::SeriesType type, QString legendTitle)
{
    QXYSeries* series = createSeriesAccordingType(type);
    series->setName(legendTitle);

    fillSeriesAndSetRanges(map, series);

    _chart.addSeries(series);

    series->attachAxis(_pAxisX);
    series->attachAxis(_pAxisY);
    setChartViewXYRange();

    _chartView.slotResetZoomAndPosition();

    _panelWidget.addSeriesInList(series);
}

void CentralWidget::addSeries(std::map<QDateTime, float> &map, QString legendTitle)
{
    addSeries(map, QXYSeries::SeriesType(_panelWidget.getSeriesType()), legendTitle);
}

void CentralWidget::openFile(const QString &fileName)
{
    QFile file(fileName);

    if(!file.exists())
    {
        file.open(QIODevice::WriteOnly);
        if(!file.isOpen())
        {
            QMessageBox::critical(nullptr, tr("Critical Error"), "Can not create new file: \"" + fileName + "\"");
            Log::write("Can not create new file: \"" + fileName + "\"", Log::Flags::WRITE_TO_FILE_AND_STDERR);
            return;
        }
        file.close();
    }


    file.open(QIODevice::ReadOnly);

    if(!file.isOpen())
    {
        QMessageBox::critical(nullptr, tr("Critical Error"), "Can not open file: \"" + fileName + "\"");
        Log::write("Can not open file: \"" + fileName + "\"", Log::Flags::WRITE_TO_FILE_AND_STDERR);
        return;
    }
    setTitle(QFileInfo(file).fileName());

    std::map<QDateTime, float> map_t1, map_t2, map_t3, map_t4;

    while (!file.atEnd())
    {
        QString buffStr = file.readLine();
        QStringList strList = buffStr.split(' ');

        if(strList.length() != 9)
            continue;

        int day, month, year;
        buffStr = strList.at(0);
        day = buffStr.split('-').at(2).toInt();
        month = buffStr.split('-').at(1).toInt();
        year = buffStr.split('-').at(0).toInt();

        int h, m, s;
        buffStr = strList.at(1);
        h = buffStr.split(':').at(0).toInt();
        m = buffStr.split(':').at(1).toInt();
        s = buffStr.split(':').at(2).toInt();

        float t1, t2, t3, t4;
        t1 = strList.at(4).toFloat();
        t2 = strList.at(5).toFloat();
        t3 = strList.at(6).toFloat();
        t4 = strList.at(7).toFloat();

        QDateTime dt(QDate(year, month, day), QTime(h, m, s));

        map_t1[dt] = t1;
        map_t2[dt] = t2;
        map_t3[dt] = t3;
        map_t4[dt] = t4;
    }
    file.close();

    _panelWidget.removeAllSeries();

    addSeries(map_t1, "T1");
    addSeries(map_t2, "T2");
    addSeries(map_t3, "T3");
    addSeries(map_t4, "T4");
}

void CentralWidget::setTitle(QString title)
{
    _chart.setTitle(title);
}

void CentralWidget::addPoints(const QDateTime &dt, const std::array<float, 4> &arr)
{
    if(_chart.series().length() != 4)
        return;

    for(int i = 0; i < 4; i++)
        *qobject_cast<QXYSeries*>(_chart.series()[i]) << QPointF(dt.toMSecsSinceEpoch(), arr[i]);

    if(_rangeX.min() > dt)
        _rangeX.setMin(dt);
    if(_rangeX.max() < dt)
        _rangeX.setMax(dt);

    std::array<float, 4> sortArr = arr;
    std::sort(sortArr.begin(), sortArr.end());

    float min = *sortArr.begin();
    float max = *sortArr.rbegin();

    if(_rangeY.min() > min)
        _rangeY.setMin(min);
    if(_rangeY.max() < max)
        _rangeY.setMax(max);

    setChartViewXYRange();

    if(_panelWidget.isAutoZoom())
        slotResetZoomAndPosition();
}

QString CentralWidget::getTitle() const
{
    return _chart.title();
}

void CentralWidget::createMenu()
{
    _menuFile.setTitle("File");
    _menuFile.addAction(QIcon(":/save_as"), "save as BMP", this, SLOT(slotSaveBMP()));

    _menuView.setTitle("View");
    _menuView.addAction(QIcon(":/reset"), "Reset zoom and position (Esc)", this, SLOT(slotResetZoomAndPosition()));
    _menuView.addAction(QIcon(":/"), "Hide/visible right panel (chart 2xClick)", this, SLOT(slotDoubleClick()));

    this->menuBar()->addMenu(&_menuFile);
    this->menuBar()->addMenu(&_menuView);
}

void CentralWidget::connectPanelWidgetSignals()
{
    connect(&_panelWidget, &PanelWidget::signalChangeTheme, this, &CentralWidget::slotSetTheme);
    connect(&_panelWidget, &PanelWidget::signalSetLegendPosition, this, &CentralWidget::slotSetLegentPosition);
    connect(&_panelWidget, &PanelWidget::signalAnimation, this, &CentralWidget::slotAnimation);
    connect(&_panelWidget, &PanelWidget::signalAntialiasing, this, &CentralWidget::slotAntialiasing);
    connect(&_panelWidget, &PanelWidget::signalTickCountChangeX, this, &CentralWidget::slotSetTcickCountX);
    connect(&_panelWidget, &PanelWidget::signalTickCountChangeY, this, &CentralWidget::slotSetTcickCountY);
    connect(&_panelWidget, &PanelWidget::signalRubberMode, this, &CentralWidget::slotSetRubberMode);
    connect(&_panelWidget, &PanelWidget::signalAxisXRangeChanged, this, &CentralWidget::slotRangeXSet);
    connect(&_panelWidget, &PanelWidget::signalAxisYRangeChanged, this, &CentralWidget::slotRangeYSet);
}

QXYSeries *CentralWidget::createSeriesAccordingType(QAbstractSeries::SeriesType &type)
{
    QXYSeries *series = nullptr;
    switch (type)
    {
    case QXYSeries::SeriesTypeLine:
        series = new QLineSeries();
        break;
    case QXYSeries::SeriesTypeSpline:
        series = new QSplineSeries();
        break;
    case QXYSeries::SeriesTypeScatter:
        series = new QScatterSeries();
        break;
    default:
        series = new QLineSeries();
        break;
    }
    return series;
}

void CentralWidget::fillSeriesAndSetRanges(const std::map<QDateTime, float> &map, QXYSeries *series)
{
    for(const auto &[x, y] : map)
    {
        *series << QPointF(x.toMSecsSinceEpoch(), y);

        if(x < _rangeX.min())
            _rangeX.setMin(x);
        if(x > _rangeX.max())
            _rangeX.setMax(x);

        if(y < _rangeY.min())
            _rangeY.setMin(y);
        if(y > _rangeY.max())
            _rangeY.setMax(y);
    }
}

void CentralWidget::setChartViewXYRange()
{
    _chartView.setRangeDefault(_rangeX, _rangeY);
}

void CentralWidget::createAxes()
{
    _pAxisX = new QDateTimeAxis;
    _pAxisX->setRange(QDateTime(QDate(2021, 9, 20), QTime(0, 0, 0)), QDateTime(QDate(2021, 9, 20), QTime(23, 59, 59)));
    _pAxisX->setFormat("hh:mm:ss");
    _pAxisX->setTitleText("Time");

    _pAxisY = new QValueAxis();
    _pAxisY->setTitleText("t°");
    _pAxisY->setRange(0, 100);

    _chart.addAxis(_pAxisX, Qt::AlignBottom);
    _chart.addAxis(_pAxisY, Qt::AlignLeft);

    _panelWidget.setRangeAxisX(_pAxisX->min(), _pAxisX->max());
    _panelWidget.setRangeAxisY(_pAxisY->min(), _pAxisY->max());
}

void CentralWidget::slotSetTheme(int theme)
{
    _chartView.chart()->setTheme(QChart::ChartTheme(theme));
    _chart.setDropShadowEnabled(true);
}

void CentralWidget::slotSetLegentPosition(int position)
{
    switch (position) {
    case 0:
        _chart.legend()->setAlignment(Qt::AlignTop);
        break;
    case 1:
        _chart.legend()->setAlignment(Qt::AlignRight);
        break;
    case 2:
        _chart.legend()->setAlignment(Qt::AlignBottom);
        break;
    case 3:
        _chart.legend()->setAlignment(Qt::AlignLeft);
        break;
    default:
        break;
    }
}

void CentralWidget::slotAnimation(bool value)
{
    if(value)
        _chart.setAnimationOptions(QChart::AllAnimations);
    else
        _chart.setAnimationOptions(QChart::NoAnimation);
}

void CentralWidget::slotAntialiasing(bool value)
{
    if(value)
        _chartView.setRenderHint(QPainter::Antialiasing);
    else
        _chartView.setRenderHint(QPainter::Antialiasing, false);
}

void CentralWidget::slotSetTcickCountY(int value)
{
    _pAxisY->setTickCount(value);
}

void CentralWidget::slotSaveBMP()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    ("Save as BMP"),
                                                    QDir::rootPath() + _chart.title(),
                                                    "Images (*.bmp);;All files (*.*)");
    if(fileName != "")
    {
        QPixmap pix = _chartView.grab();
        pix.toImage().save(fileName, "BMP");
    }
}

void CentralWidget::slotResetZoomAndPosition()
{
    _chartView.slotResetZoomAndPosition();
}

void CentralWidget::slotViewXYCoordinate(QPointF point)
{
    QDateTime dt;
    dt.setMSecsSinceEpoch(point.x());
    _statusBarWidget.setText("X: " + dt.toString("yyyy-MM-dd hh:mm:ss") + "   Y: " + QString::number(point.y()) + " C°");
}

void CentralWidget::slotSetRubberMode(QChartView::RubberBand mode)
{
    _chartView.setRubberBand(mode);
}

void CentralWidget::slotRangeXChanged(QDateTime min, QDateTime max)
{
    _panelWidget.setRangeAxisX(min, max);
}

void CentralWidget::slotRangeYChanged(qreal min, qreal max)
{
    _panelWidget.setRangeAxisY(min, max);
}

void CentralWidget::slotRangeXSet(QDateTime min, QDateTime max)
{
    _pAxisX->setRange(min, max);
}

void CentralWidget::slotRangeYSet(qreal min, qreal max)
{
    _pAxisY->setRange(min, max);
}

void CentralWidget::slotDoubleClick()
{
    _panelWidget.setVisible(!_panelWidget.isVisible());
}

void CentralWidget::closeEvent(QCloseEvent*)
{
    exit(0);
}

void CentralWidget::slotSetTcickCountX(int value)
{
    _pAxisX->setTickCount(value);
}
