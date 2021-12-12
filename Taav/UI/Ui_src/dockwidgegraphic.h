#ifndef DOCKWIDGEGRAPHIC_H
#define DOCKWIDGEGRAPHIC_H

#include <QDockWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QActionGroup)
QT_FORWARD_DECLARE_CLASS(QMenu)

class Dockwidgegraphic : public QDockWidget
{
    Q_OBJECT

public:
    explicit Dockwidgegraphic(const QString &Name, QMainWindow *parent = nullptr, Qt::WindowFlags flags = 0);

    QMenu *DockwidgegraphicMenu() const { return menu; }

    void UpdateImage(int ImageNum);
    void setImageFolder(QString folder);

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU
    void resizeEvent(QResizeEvent *e) override;

private slots:
    void changeVerticalTitleBar(bool on);
    void updateContextMenu();

    void placeRight(bool p);
    void placeTop(bool p);

    void splitInto(QAction *action);
    void tabInto(QAction *action);
    void VeloctyFolderImage(QAction *action);
    void DensityFolderImage(QAction *action);



private:
    void allow(Qt::DockWidgetArea area, bool allow);
    void place(Qt::DockWidgetArea area, bool place);

    QAction *verticalTitleBarAction;

    QGraphicsScene *GraphicsScene;
    QGraphicsView *GraphicsView;
    QString Imagefolder;
    QString ImageFolderselected;
    QStringList ImageList;

    QActionGroup *areaActions;
    QAction *rightAction;
    QAction *topAction;

    QMenu *tabMenu;
    QMenu *PicVelocityMenu;
    QMenu *PicDensityMenu;
    QMenu *splitHMenu;
    QMenu *splitVMenu;
    QMenu *menu;

    QMenu *dockWidgetMenu;

    QList<QDockWidget *> extraDockWidgetsGraphic;
    QList<QDockWidget *> extraDockWidgetsChart;
    QList<QGraphicsScene *> GraphicsScenes;
    QGraphicsView *TempGraphicsView;
    QString PicVelocityFolder;

    QMenu *destroyDockWidgetMenuGraphic;

    QMainWindow *mainWindow;

     QImage image;
};

#endif // DOCKWIDGEGRAPHIC_H
