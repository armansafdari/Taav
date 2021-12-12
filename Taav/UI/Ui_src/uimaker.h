#ifndef UIMAKER_H
#define UIMAKER_H

#include<QMainWindow>
#include <QSplitter>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineSeries>
#include <QtCharts>
#include <QAction>
#include <QLineEdit>
#include <textnode.h>
#include <treemaker.h>
#include <QDir>

#include "picfinder.h"
#include "dockwidgegraphic.h"
#include "dockwidgetchart.h"

#include "graphic_view_zoom.h"


using namespace QtCharts;
class ToolBar;
QT_FORWARD_DECLARE_CLASS(QMenu)

class uiMaker : public QObject
{
    Q_OBJECT
public:
    explicit uiMaker(QMainWindow* _mainwindow,QObject *parent = nullptr);
    int makeIt();
    QString saveTheTree();
    QList<textNode *> rootTextNodeList;

    QPushButton *start;
    QPushButton *stop;
//    QLineEdit *NumberOfRacks;
    QLineEdit *path;

    void loadSettingFunction(QString s);
    void saveSettingFunction(QString s);
    QSlider *slider;

    QGraphicsPixmapItem *pixMap1;
    QGraphicsPixmapItem *pixMap2;

    void setPicInt(int i);
    void setWorkDirinTree(QString str);


private:
    int startPuaseState = 0;
    QMainWindow *MainWindow;
    QWidget *downObject;
    QSplitter *spliterup;
    QGraphicsScene *scene;
    QGraphicsView *view;

    QGraphicsScene *scene2;
    QGraphicsView *view2;
    QTextEdit *txt;
    QProgressBar *progressBar ;
    QLineEdit *lineEditFrame;


//    QPushButton *previousFrame;
//    QPushButton *nextFrame;
//    QPushButton *firstFrame;
//    QPushButton *previousSecond;
//    QPushButton *nextSecond;
//    QPushButton *lastFrmae;

    QTreeWidget *treeEnv;
    QTreeWidget *treeMaterials;
    QTreeWidget *treeGeomtry;
    QTreeWidget *treeSimulation;
    QTreeWidget *treePost;



    treeMaker *treemaker;

    QAction *newProject;
    QAction *loadProject;
//    QAction *loadSetting;
    QAction *saveSetting;
    QAction *saveSettingAs;

    QAction *ImportXMLSetting;
    QAction *ExportXMLSetting;

    QAction *exit;

    void fillCurrentTree(node *,QDomNode elem);
    QDomElement docElem;

    PicFinder *finder;

//////////////////////////////////////////////////////
public:
    QString ProjectPath;
//    QString WorkFolder="/media/cpu/DATA/Palabos/palabos-v2.0r0/Taav_project_new/solver/";
    QString ImageFolder="results/Image/";
    QString WorkFolder="";
    QLineEdit *NumberOfRacks = new QLineEdit("1");
//    QLineEdit *path=new QLineEdit("./");


private:
    QString fileName="untitled";
    QTabWidget *tabStructTree;
    QDockWidget *dockWidgetStructTree;

    QList<ToolBar*> toolBars;
    QMenu *dockWidgetMenu;


    QList<QDockWidget *> extraDockWidgetsGraphic;
    QList<QDockWidget *> extraDockWidgetsChart;
    QList<QLineSeries *> Chartseries;
    QList<QGraphicsScene *> GraphicsScenes;
    QGraphicsView *TempGraphicsView;


    QMenu *destroyDockWidgetMenuGraphic;
    QMenu *destroyDockWidgetMenuChart;
    QTextEdit* TextOutPut;
    QDockWidget *dwMessageBox;

    QLineSeries *series;


    QPushButton *previousFrame;
    QPushButton *nextFrame;
    QPushButton *firstFrame;
    QPushButton *lastFrmae;
    QPushButton *jumppreviousFrame;
    QPushButton *jumpnextFrame;

    QWidget *containerMiddle;
    QGridLayout *layoutMiddle;
    QToolBar *toolBar;

    int maxFreq =10;
    int slidersteptolabel = 1;
    int sliderMinTime = 0;
    int sliderMaxTime = 1000;
    QList<QLabel*> Labels;
    int NumberofLabels = 5;

    QTimer *timer;


    void setupMenuBar();
    void setupDockWidgetsChart();
    void setupDockWidgetsGraphic();
    void setupDockWidgetsStatus();
    void setupTreeStructure();
    void createTreeStructure();
    void updateSlider();
    void SetupSlider(int, int,int);
    void resetDockWidgets();

    friend class Dockwidgegraphic;
public slots:
    void about();
    void setupDockWidgetsMessage();
    void createDockWidgetGraphic();
    void destroyDockWidgetGraphic(QAction *action);

    void createDockWidgetChart();
    void destroyDockWidgetChart(QAction *action);


///////////////////////////////////////////////////////
signals:
    void exitClick();
    void saveClick(QString);
    void openClick(QString);

    void newProjectClick(QString);
    void loadProjectClick(QString);
    void saveSettingClick();

    void clickStart(int a, int R, QString filename);
    void clickPause();
    void clickStop();

    void startSearch();
    void stopSearch();
public slots:
//    void logTextSet(QString str);
    void PauseUI();
    void StartUI();
    void StopUI();
    void changeProgressBarUi(int i);
    void UpdateMessageBoxUi(QString str);
    void AppendMessageBoxUi(QString str);
    void newProjectSlot();
    void loadProjectSlot();
    void saveProjectSlot();
    void saveAsProjectSlot();

    void ImportXMLSettingSlot();
    void ExportXMLSettingSlot();



public:
    void setPic(QString s1,QString s2);


private:
    QStringList StringList_Velocity;
    QStringList StringList_Density;
    QChart *chart ;


public slots:
    void newPicFind(int max,QStringList l1,QStringList l2);


};

#endif // UIMAKER_H
