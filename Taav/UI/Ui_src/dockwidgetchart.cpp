#include "dockwidgetchart.h"
#include "graphic_view_zoom.h"

#include <QAction>
#include <QtEvents>
#include <QMainWindow>
#include <QMenu>
#include <QtDebug>


#undef DEBUG_SIZEHINTS

//QT_FORWARD_DECLARE_CLASS(QLineSeries)

dockwidgetchart::dockwidgetchart(const QString &Name, QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget(parent, flags), mainWindow(parent)
{
    series = new QLineSeries();
    chart = new QChart();
    chartView = new QChartView(chart);

    setObjectName(Name);
    setWindowTitle(objectName());
    setWidget(chartView);

    setAllowedAreas(Qt::TopDockWidgetArea | Qt::RightDockWidgetArea);

    verticalTitleBarAction = new QAction(tr("Vertical title bar"), this);
    verticalTitleBarAction->setCheckable(true);
    connect(verticalTitleBarAction, &QAction::triggered,
            this, &dockwidgetchart::changeVerticalTitleBar);

    areaActions = new QActionGroup(this);
    areaActions->setExclusive(true);

    rightAction = new QAction(tr("Place on Right") , this);
    rightAction->setCheckable(true);
    connect(rightAction, &QAction::triggered, this, &dockwidgetchart::placeRight);

    topAction = new QAction(tr("Place on Top") , this);
    topAction->setCheckable(true);
    connect(topAction, &QAction::triggered, this, &dockwidgetchart::placeTop);

    areaActions->addAction(rightAction);
    areaActions->addAction(topAction);

    ChartMenu = new QMenu(this);
    ChartMenu->setTitle(tr("set the chart"));
    connect(ChartMenu, &QMenu::triggered, this, &dockwidgetchart::updateChart);

    tabMenu = new QMenu(this);
    tabMenu->setTitle(tr("Tab into"));
    connect(tabMenu, &QMenu::triggered, this, &dockwidgetchart::tabInto);

    splitHMenu = new QMenu(this);
    splitHMenu->setTitle(tr("Split horizontally into"));
    connect(splitHMenu, &QMenu::triggered, this, &dockwidgetchart::splitInto);

    splitVMenu = new QMenu(this);
    splitVMenu->setTitle(tr("Split vertically into"));
    connect(splitVMenu, &QMenu::triggered, this, &dockwidgetchart::splitInto);

    menu = new QMenu(Name, this);
    menu->addAction(toggleViewAction());
    menu->addSeparator();
    menu->addMenu(ChartMenu);
    menu->addSeparator();
    menu->addActions(areaActions->actions());
    menu->addSeparator();
    menu->addMenu(splitHMenu);
    menu->addMenu(splitVMenu);
    menu->addMenu(tabMenu);
    menu->addSeparator();
    menu->addAction(verticalTitleBarAction);

    connect(menu, &QMenu::aboutToShow, this, &dockwidgetchart::updateContextMenu);
}

void dockwidgetchart::updateContextMenu()
{
    const Qt::DockWidgetArea area = mainWindow->dockWidgetArea(this);
    const Qt::DockWidgetAreas areas = allowedAreas();

    if (windowType() == Qt::Drawer) {
        verticalTitleBarAction->setChecked(false);
    } else {
        // done after floating, to get 'floatable' correctly initialized
        verticalTitleBarAction
            ->setChecked(features() & QDockWidget::DockWidgetVerticalTitleBar);
    }

    {
        const QSignalBlocker blocker(rightAction);
        rightAction->setChecked(area == Qt::RightDockWidgetArea);
    }
    {
        const QSignalBlocker blocker(topAction);
        topAction->setChecked(area == Qt::TopDockWidgetArea);
    }

    if (areaActions->isEnabled()) {
        rightAction->setEnabled(areas & Qt::RightDockWidgetArea);
        topAction->setEnabled(areas & Qt::TopDockWidgetArea);
    }

    tabMenu->clear();
    splitHMenu->clear();
    splitVMenu->clear();
    QList<dockwidgetchart*> dock_list = mainWindow->findChildren<dockwidgetchart*>();
    foreach (dockwidgetchart *dock, dock_list) {
        if (this->objectName()!=dock->objectName()){
            tabMenu->addAction(dock->objectName());
            splitHMenu->addAction(dock->objectName());
            splitVMenu->addAction(dock->objectName());
        }
    }

    ChartMenu->clear();

    QFile chartfile(ChartFile);
    qDebug() << ChartFile ;
    if (chartfile.open(QIODevice::ReadOnly | QIODevice::Text)) {

    QTextStream stream(&chartfile);

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if ( line.startsWith("::")){
            qDebug() << line;
            ChartValuesYtitles = line.split(" ", QString::SkipEmptyParts);
        }

    }
    chartfile.close();
    }

    for(int i=2;i<ChartValuesYtitles.size();i++){
        ChartMenu->addAction(ChartValuesYtitles[i]);
    }

}

void dockwidgetchart::setChartFile(QString file){
    ChartFile = file;
}

static dockwidgetchart *findByName(const QMainWindow *mainWindow, const QString &name)
{
    foreach (dockwidgetchart *dock, mainWindow->findChildren<dockwidgetchart*>()) {
        if (name == dock->objectName())
            return dock;
    }
    return nullptr;
}

void dockwidgetchart::splitInto(QAction *action)
{
    dockwidgetchart *target = findByName(mainWindow, action->text());
    if (!target)
        return;

    const Qt::Orientation o = action->parent() == splitHMenu
        ? Qt::Horizontal : Qt::Vertical;
    mainWindow->splitDockWidget(target, this, o);
}

void dockwidgetchart::tabInto(QAction *action)
{
    if (dockwidgetchart *target = findByName(mainWindow, action->text()))
        mainWindow->tabifyDockWidget(target, this);
}

#ifndef QT_NO_CONTEXTMENU
void dockwidgetchart::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
    menu->exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU

void dockwidgetchart::resizeEvent(QResizeEvent *e)
{
    QDockWidget::resizeEvent(e);
}

void dockwidgetchart::allow(Qt::DockWidgetArea area, bool a)
{
    Qt::DockWidgetAreas areas = allowedAreas();
    areas = a ? areas | area : areas & ~area;
    setAllowedAreas(areas);

    if (areaActions->isEnabled()) {
        rightAction->setEnabled(areas & Qt::RightDockWidgetArea);
        topAction->setEnabled(areas & Qt::TopDockWidgetArea);
    }
}

void dockwidgetchart::place(Qt::DockWidgetArea area, bool p)
{
    if (!p)
        return;
    mainWindow->addDockWidget(area, this);
}

void dockwidgetchart::placeRight(bool p)
{ place(Qt::RightDockWidgetArea, p); }

void dockwidgetchart::placeTop(bool p)
{ place(Qt::TopDockWidgetArea, p); }

void dockwidgetchart::changeVerticalTitleBar(bool on)
{
    setFeatures(on ? features() | DockWidgetVerticalTitleBar
                    : features() & ~DockWidgetVerticalTitleBar);
}

void dockwidgetchart::updateChart(QAction *action){

    qDebug() << " it ia called action->text()" << action->text() ;

    series->clear();
    chart->removeSeries(series);
    chart->removeAxis(axisX);
    chart->removeAxis(axisY);



    QFile chartfile(ChartFile);
    qDebug() << ChartFile ;
    if (chartfile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QTextStream stream(&chartfile);
        int setChartcolum=0;
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if ( line.startsWith("::")){
                ChartValuesYtitles = line.split(" ", QString::SkipEmptyParts);

                for(int i=1;i<ChartValuesYtitles.size();++i)
                    if(ChartValuesYtitles[i].compare( action->text())==0)
                        setChartcolum = i-1;

            }
            else{

                QStringList Chartvalues = line.split(" ", QString::SkipEmptyParts);
                if (line.size()>=1){
                series->append(QPointF(Chartvalues[0].toDouble(),Chartvalues[setChartcolum].toDouble()));
                }
            }
        }
        chartfile.close();

    }

//    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->addSeries(series);
    chart->legend()->hide();
//    chart->setTitle("Chart for " + action->text());

    axisX = new QValueAxis;
    axisX->setTickCount(11);
    axisX->setLabelFormat("%i");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis;
//    axisY->setLabelFormat("%i");
    axisY->setTitleText(action->text());
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand);
    setWidget(chartView);

}
