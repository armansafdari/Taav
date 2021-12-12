#ifndef DOCKWIDGETCHART_H
#define DOCKWIDGETCHART_H

#include <QDockWidget>
#include <QtCharts>

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QActionGroup)
QT_FORWARD_DECLARE_CLASS(QMenu)
//QT_FORWARD_DECLARE_CLASS(QLineSeries)

class dockwidgetchart : public QDockWidget
{
    Q_OBJECT

public:
    explicit dockwidgetchart(const QString &colorName, QMainWindow *parent = nullptr, Qt::WindowFlags flags = 0);

    QMenu *dockwidgetchartMenu() const { return menu; }
//    void setSeries(QLineSeries *_series);
//    void updateChart();
        void setChartFile(QString file);

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU
    void resizeEvent(QResizeEvent *e) override;

private slots:
    void changeVerticalTitleBar(bool on);
//    void updateChart(int N);
    void updateChart(QAction *action);

    void updateContextMenu();

    void placeRight(bool p);
    void placeTop(bool p);

    void splitInto(QAction *action);
    void tabInto(QAction *action);


private:
    void allow(Qt::DockWidgetArea area, bool allow);
    void place(Qt::DockWidgetArea area, bool place);

    QAction *verticalTitleBarAction;

    QActionGroup *areaActions;
    QAction *rightAction;
    QAction *topAction;

    QMenu *tabMenu;
    QMenu *splitHMenu;
    QMenu *splitVMenu;
    QMenu *menu;
    QMenu *ChartMenu;
    QStringList ChartValuesYtitles;

    QMenu *dockWidgetMenu;

    QList<QDockWidget *> extraDockWidgetsChart;

    QMenu *destroyDockWidgetMenuGraphic;

    QLineSeries *series;
    QChart *chart;
    QChartView *chartView;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QString ChartFile;

    QMainWindow *mainWindow;
};

#endif // DOCKWIDGETCHART_H
