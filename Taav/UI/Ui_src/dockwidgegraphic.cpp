#include "dockwidgegraphic.h"
#include "graphic_view_zoom.h"

#include <QAction>
#include <QtEvents>
#include <QFrame>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QLabel>
#include <QPainterPath>
#include <QPushButton>
#include <QHBoxLayout>
#include <QBitmap>
#include <QtDebug>
#include "QDir"


#undef DEBUG_SIZEHINTS



Dockwidgegraphic::Dockwidgegraphic(const QString &Name, QMainWindow *parent, Qt::WindowFlags flags)
    : QDockWidget(parent, flags), mainWindow(parent)
{

    GraphicsScene = new QGraphicsScene;
    GraphicsView = new QGraphicsView;
    GraphicsView->setScene(GraphicsScene);

//    QImage image("/media/cpu/DATA/Palabos/palabos-v2.0r0/Ui_palabos/results/Image/Velocity_0/VelocityNorm_0_000000.gif");
//    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
//    GraphicsScene->addItem(item);
    GraphicsView->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
    Graphics_view_zoom* z = new Graphics_view_zoom(this->GraphicsView);
    z->set_modifiers(Qt::NoModifier);

    setObjectName(Name);
    setWindowTitle(objectName());

    setAllowedAreas(Qt::TopDockWidgetArea | Qt::RightDockWidgetArea);
    setWidget(GraphicsView);

    verticalTitleBarAction = new QAction(tr("Vertical title bar"), this);
    verticalTitleBarAction->setCheckable(true);
    connect(verticalTitleBarAction, &QAction::triggered,
            this, &Dockwidgegraphic::changeVerticalTitleBar);

    areaActions = new QActionGroup(this);
    areaActions->setExclusive(true);

    rightAction = new QAction(tr("Place on Right") , this);
    rightAction->setCheckable(true);
    connect(rightAction, &QAction::triggered, this, &Dockwidgegraphic::placeRight);

    topAction = new QAction(tr("Place on Top") , this);
    topAction->setCheckable(true);
    connect(topAction, &QAction::triggered, this, &Dockwidgegraphic::placeTop);

    areaActions->addAction(rightAction);
    areaActions->addAction(topAction);

    PicVelocityMenu = new QMenu(this);
    PicVelocityMenu->setTitle(tr("Velocity"));
    connect(PicVelocityMenu, &QMenu::triggered, this, &Dockwidgegraphic::VeloctyFolderImage);


    PicDensityMenu = new QMenu(this);
    PicDensityMenu->setTitle(tr("Density"));
    connect(PicDensityMenu, &QMenu::triggered, this, &Dockwidgegraphic::DensityFolderImage);
//    connect(PicDensityMenu,&QMenu::triggered,[=](QString s){
//              PicVelocityFolder = s;
//               qDebug() << PicVelocityFolder;});

    tabMenu = new QMenu(this);
    tabMenu->setTitle(tr("Tab into"));
    connect(tabMenu, &QMenu::triggered, this, &Dockwidgegraphic::tabInto);

    splitHMenu = new QMenu(this);
    splitHMenu->setTitle(tr("Split horizontally into"));
    connect(splitHMenu, &QMenu::triggered, this, &Dockwidgegraphic::splitInto);

    splitVMenu = new QMenu(this);
    splitVMenu->setTitle(tr("Split vertically into"));
    connect(splitVMenu, &QMenu::triggered, this, &Dockwidgegraphic::splitInto);

    menu = new QMenu(Name, this);
    menu->addAction(toggleViewAction());

    menu->addSeparator();
    menu->addMenu(PicVelocityMenu);
    menu->addMenu(PicDensityMenu);
    menu->addSeparator();
    menu->addActions(areaActions->actions());
    menu->addSeparator();
    menu->addMenu(splitHMenu);
    menu->addMenu(splitVMenu);
    menu->addMenu(tabMenu);
    menu->addSeparator();


    menu->addAction(verticalTitleBarAction);

    connect(menu, &QMenu::aboutToShow, this, &Dockwidgegraphic::updateContextMenu);

//        QAction *RightArrow = new QAction(this);
//        RightArrow->setCheckable(true);
//        connect(RightArrow, &QAction::triggered, this, &Dockwidgegraphic::placeRight);
//        rightAction->setShortcut(Qt::CTRL | Qt::Key_E);
//        rightAction->setShortcut(Qt::CTRL | Qt::Key_W);

//        QAction *UpArrow = new QAction(this);
//        UpArrow->setCheckable(true);
//        connect(UpArrow, &QAction::triggered, this, &Dockwidgegraphic::placeTop);
//        this->topAction->setShortcut(Qt::CTRL | Qt::ArrowType::UpArrow);
//        areaActions->addAction(RightArrow);
//        areaActions->addAction(UpArrow);

//        toggleViewAction()->setShortcut(Qt::CTRL | Qt::Key_R);
}

void Dockwidgegraphic::updateContextMenu()
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
    QList<Dockwidgegraphic*> dock_list = mainWindow->findChildren<Dockwidgegraphic*>();
    foreach (Dockwidgegraphic *dock, dock_list) {
        if (this->objectName()!=dock->objectName()){
            tabMenu->addAction(dock->objectName());
            splitHMenu->addAction(dock->objectName());
            splitVMenu->addAction(dock->objectName());
        }
    }

    PicVelocityMenu->clear();
    PicDensityMenu->clear();

    QString workingPath = Imagefolder;
    QDir dir(workingPath);
    dir.setSorting(QDir::Name);
    QStringList dirList = dir.entryList();
    foreach(QString s,dirList)
    {
      if( QString::compare(s, "Velocity", Qt::CaseInsensitive)==1){
          PicVelocityMenu->addAction(s);
      }

      if( QString::compare(s, "Density", Qt::CaseInsensitive)==1){
          PicDensityMenu->addAction(s);
      }
    }
}

static Dockwidgegraphic *findByName(const QMainWindow *mainWindow, const QString &name)
{
    foreach (Dockwidgegraphic *dock, mainWindow->findChildren<Dockwidgegraphic*>()) {
        if (name == dock->objectName())
            return dock;
    }
    return nullptr;
}



void Dockwidgegraphic::UpdateImage(int ImageNum){
    if (ImageList.size() > ImageNum ){
        QString ImageCurrent = Imagefolder + ImageFolderselected + "/" + ImageList[ImageNum];
        qDebug() << ImageCurrent;
        GraphicsScene->clear();
        QImage image(ImageCurrent);
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        GraphicsScene->addItem(item);
    }

}

void Dockwidgegraphic::splitInto(QAction *action)
{
    Dockwidgegraphic *target = findByName(mainWindow, action->text());
    if (!target)
        return;

    const Qt::Orientation o = action->parent() == splitHMenu
        ? Qt::Horizontal : Qt::Vertical;
    mainWindow->splitDockWidget(target, this, o);
}

void Dockwidgegraphic::tabInto(QAction *action)
{
    if (Dockwidgegraphic *target = findByName(mainWindow, action->text()))
        mainWindow->tabifyDockWidget(target, this);
}


void Dockwidgegraphic::VeloctyFolderImage(QAction *action)
{

    qDebug() << action->text();
    ImageFolderselected = action->text();
    qDebug() << Imagefolder + ImageFolderselected;

    QString workingPath = Imagefolder + ImageFolderselected;
    QDir dir(workingPath);
    dir.setSorting(QDir::Name);
    QStringList List = dir.entryList();
    ImageList.clear();
    foreach(QString s,List)
    {
      if( QString::compare(s, ".", Qt::CaseSensitive)!=1&& QString::compare(s, "..", Qt::CaseSensitive)!=1){
          ImageList.append(s);
      }

      if( QString::compare(s, "Density", Qt::CaseInsensitive)==1){
          PicDensityMenu->addAction(s);
      }
//      qDebug() << s;

    }
    if(~ImageList.empty()){

        UpdateImage(1);
//        qDebug() << Imagefolder + ImageFolderselected + "/" + ImageList[1];
    }
//    if (Dockwidgegraphic *target = findByName(mainWindow, action->text()))
//        mainWindow->tabifyDockWidget(target, this);
}



void Dockwidgegraphic::DensityFolderImage(QAction *action)
{

    qDebug() << action->text();
    ImageFolderselected = action->text();
    qDebug() << Imagefolder + ImageFolderselected;

    QString workingPath = Imagefolder + ImageFolderselected;
    QDir dir(workingPath);
    dir.setSorting(QDir::Name);
    QStringList List = dir.entryList();
    ImageList.clear();
    foreach(QString s,List)
    {
      if( QString::compare(s, ".", Qt::CaseSensitive)!=1&& QString::compare(s, "..", Qt::CaseSensitive)!=1){
          ImageList.append(s);
      }

//      if( QString::compare(s, "Density", Qt::CaseInsensitive)==1){
//          PicDensityMenu->addAction(s);
//      }

    }
    if(~ImageList.empty()){
        UpdateImage(1);
    }

}



void Dockwidgegraphic::setImageFolder(QString folder){
    Imagefolder = folder;
}

#ifndef QT_NO_CONTEXTMENU
void Dockwidgegraphic::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
    menu->exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU

void Dockwidgegraphic::resizeEvent(QResizeEvent *e)
{

    QDockWidget::resizeEvent(e);
}

void Dockwidgegraphic::allow(Qt::DockWidgetArea area, bool a)
{
    Qt::DockWidgetAreas areas = allowedAreas();
    areas = a ? areas | area : areas & ~area;
    setAllowedAreas(areas);

    if (areaActions->isEnabled()) {
        rightAction->setEnabled(areas & Qt::RightDockWidgetArea);
        topAction->setEnabled(areas & Qt::TopDockWidgetArea);
    }
}

void Dockwidgegraphic::place(Qt::DockWidgetArea area, bool p)
{
    if (!p)
        return;

    mainWindow->addDockWidget(area, this);

}


void Dockwidgegraphic::placeRight(bool p)
{ place(Qt::RightDockWidgetArea, p); }

void Dockwidgegraphic::placeTop(bool p)
{ place(Qt::TopDockWidgetArea, p); }

void Dockwidgegraphic::changeVerticalTitleBar(bool on)
{
    setFeatures(on ? features() | DockWidgetVerticalTitleBar
                    : features() & ~DockWidgetVerticalTitleBar);
}
