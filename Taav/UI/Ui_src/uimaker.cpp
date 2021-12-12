#include "uimaker.h"
#include "dockwidgegraphic.h"
#include "dockwidgetchart.h"

#include<QFile>
#include<QMenuBar>
#include<QDockWidget>
#include<QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QPushButton>
#include <QTextEdit>
#include <QProgressBar>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QSlider>
#include <QHBoxLayout>
#include <QTreeWidgetItem>

#include <QGraphicsPixmapItem>
#include <pdockwidget.h>
#include <QDebug>

#include <QLineEdit>
#include <QFileDialog>



uiMaker::uiMaker(QMainWindow* m ,QObject *parent) : QObject(parent)
{
    //MainWindow is the mainwindow pointer
    MainWindow = m;


    qDebug() << "uiMaker is working";


    QFile File(":/styleSheet.qss");
    File.open(QFile::ReadOnly);
    QString newStylesheet = QLatin1String(File.readAll());
    MainWindow->setStyleSheet(newStylesheet);
    MainWindow->setObjectName("MainWindow");
    MainWindow->setWindowTitle(fileName + " - Taav" );

    setupTreeStructure();
    setupMenuBar();
    setupDockWidgetsMessage();
    setupDockWidgetsStatus();

}

int uiMaker::makeIt( )
{

//    path = treemaker->WorkingDir;

    //--------------------------------------------
    connect(slider,&QSlider::valueChanged,[=](int value){
        QString s = QString::number(value);
        lineEditFrame->setText(s);

    });
    connect(lineEditFrame,&QLineEdit::editingFinished,[=](){
        if(slider->value()!=lineEditFrame->text().toInt())
            slider->setValue(lineEditFrame->text().toInt());
    });

    connect(previousFrame,&QPushButton::clicked,[=](){
        slider->setValue(slider->value()-1);
    });
    connect(nextFrame,&QPushButton::clicked,[=](){
        slider->setValue(slider->value()+1);
    });
    connect(firstFrame,&QPushButton::clicked,[=](){
        slider->setValue(slider->minimum());
    });
    connect(lastFrmae,&QPushButton::clicked,[=](){
        slider->setValue(slider->maximum());
    });



//------------------------------------------------
    finder = new PicFinder;
    QThread *thread = new QThread;
    finder->moveToThread(thread);
    thread->start();

    connect(this,&uiMaker::startSearch,finder,&PicFinder::StartSearch);
    connect(this,&uiMaker::stopSearch,finder,&PicFinder::StopSearch);
    connect(finder,&PicFinder::changeMaxOfSlider,this,&uiMaker::newPicFind);

//----------------------------------------
    return 1;
}

void uiMaker::loadSettingFunction(QString fileName)
{
//    finder->UpdatePath(path->text());

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cant read file";
        return;
    }

    resetDockWidgets();
    setupDockWidgetsStatus();
    setupTreeStructure();
    createTreeStructure();
    setupDockWidgetsGraphic();
    setupDockWidgetsChart();



    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qDebug() << "can't set Content";
        file.close();
        return;
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    docElem = doc.documentElement();


    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    for (int i =0;i< rootTextNodeList.length() ; i++) {
        fillCurrentTree(rootTextNodeList.at(i),docElem.childNodes().at(i));
    }

    start->setDisabled(false);
    stop->setDisabled(false);


}

void uiMaker::setPic(QString s1,QString s2)
{
//    qDebug() << "*****setting pictures:"<<path->text() + "/results/Image/Velocity_1/" + s2;
//    QImage img1(QImage(path->text() + "/results/Image/Density_1/" + s1));
//    pixMap1->setPixmap(QPixmap::fromImage(img1));

//    QImage img2(QImage(path->text() + "/results/Image/Velocity_1/" + s2));
//    pixMap2->setPixmap(QPixmap::fromImage(img2));
}

void uiMaker::newPicFind(int max,QStringList list_Density,QStringList list_Velocity)
{
    slider->setMaximum(max-1);
    StringList_Velocity = list_Velocity;
    StringList_Density = list_Density;
    setPic(StringList_Density.last(),StringList_Velocity.last());
}


//------------------------------UNDER CONTRUCTION---------------------------------
//--------------------------------------------------------------------------------
void uiMaker::fillCurrentTree(node *w,QDomNode elem)
{
//    qDebug() <<"+++++++++++++"<< w->getName() <<"+++++++++Filling Start";
////    qDebug() << elem.tagName() <<x.elementsByTagName("value").at(0).toElement().text();
//    qDebug() << elem.toElement().tagName() << elem.toElement().elementsByTagName("value").at(0).toElement().text();


    QStack<node*> stackNodes;
    stackNodes.push(w);
    node *parentNode;
    node *childNode;
    //=================
    QStack<QDomNode> stackDomNode;
    stackDomNode.push(elem);
    QDomNode parentDomNode;
    QDomNode childDomNode;

    while(!stackNodes.isEmpty()){
        parentNode = stackNodes.pop();
        parentDomNode = stackDomNode.pop();

        QString tagValue = parentDomNode.toElement().elementsByTagName("value").at(0).toElement().text();

//        qDebug() << "Widget:" <<parentNode->getName() <<"Loaded Tree:" <<parentDomNode.toElement().tagName() << tagValue;

        if (tagValue!=""){
            parentNode->pSetData(tagValue);
        }


        for(int i = 0; i < parentNode->childNodes.count() ; i++){
            int lenth = parentNode->childNodes.count()-1;
            childNode = parentNode->childNodes.at(lenth - i);
            stackNodes.push(childNode);

            childDomNode = parentDomNode.childNodes().at(lenth - i +1);
            stackDomNode.push(childDomNode);
        }
    }

//    qDebug() <<"+++++++++++++"<< w->getName()<< "++++++++++Filling Stop";
}



void uiMaker::PauseUI()
{
    start->setText("Start");
    startPuaseState = 0;
    treemaker->setEnable(true);
//    emit stopSearch();

}

void uiMaker::StartUI()
{
    start->setText("Pause");
    startPuaseState = 1;
    treemaker->setEnable(false);
//    emit startSearch();


//    slider->setRange(treemaker->lineEditStart->text().toInt(),
//                          treemaker->lineEditFinish->text().toInt()*treemaker->lineEditFrequency->text().toInt());
//    setPicInt(0);
//    qDebug() << "+++++++++++++++++++=";


}


void uiMaker::StopUI()
{

    treemaker->setEnable(true);


//    slider->setRange(treemaker->lineEditStart->text().toInt(),
//                          treemaker->lineEditFinish->text().toInt()*treemaker->lineEditFrequency->text().toInt());
//    setPicInt(0);
//    qDebug() << "+++++++++++++++++++=";


}

void uiMaker::changeProgressBarUi(int i)
{
    progressBar->setValue(i);
}
void uiMaker::UpdateMessageBoxUi(QString str)
{
//    qDebug() << " the uiMaker called logTextset = " << str;
    TextOutPut->clear();
    TextOutPut->setText(str);

}
void uiMaker::AppendMessageBoxUi(QString str)
{
//    qDebug() << " the uiMaker called logTextset Append = " << str;
    TextOutPut->append(str);

}

void uiMaker::setWorkDirinTree(QString str){

//   path = treemaker->WorkingDir;
//   path->setText(str);

}

void uiMaker::newProjectSlot()
{

    QString fileName = QFileDialog::getSaveFileName(MainWindow, tr("Create new file"),
        ProjectPath, tr("XML files (*.xml)"));

    ProjectPath = fileName;

    if (!fileName.isEmpty()) {

        QFileInfo fi(ProjectPath);
        WorkFolder  = fi.path() + "/";
        if (fi.suffix() != "xml"){
          ProjectPath = fi.path() + "/" + fi.completeBaseName() + ".xml";
          QFile().rename(fileName,ProjectPath);
        }

    MainWindow->setWindowTitle(ProjectPath + " - Taav" );

    resetDockWidgets();
    setupDockWidgetsStatus();
    setupTreeStructure();
    createTreeStructure();
    setupDockWidgetsGraphic();
    setupDockWidgetsChart();

    start->setDisabled(false);
    stop->setDisabled(false);

    saveProjectSlot();

    }
}

void uiMaker::loadProjectSlot()
{
    qDebug() << "Load Project";
    QString fileName = QFileDialog::getOpenFileName(MainWindow,tr("Open File"));

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cant read file";
        return;
    }

    ProjectPath = fileName;
    QFileInfo fi(ProjectPath);
    WorkFolder  = fi.path() + "/";
    MainWindow->setWindowTitle(ProjectPath + " - Taav" );

    qDebug() << ProjectPath;

    loadSettingFunction(fileName);


}

void uiMaker::saveProjectSlot()
{
    qDebug() << "save Setting";

    QFile file(ProjectPath);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream << saveTheTree();
    file.close();

}

void uiMaker::saveAsProjectSlot()
{


    QString fileName = QFileDialog::getSaveFileName(MainWindow, tr("Save new file as"),
        ProjectPath, tr("XML files (*.xml)"));



    if (!fileName.isEmpty()) {
        ProjectPath = fileName;

        QFileInfo fi(ProjectPath);
        WorkFolder  = fi.path() + "/";
        if (fi.suffix() != "xml"){
          ProjectPath = fi.path() + "/" + fi.completeBaseName() + ".xml";
          QFile().rename(fileName,ProjectPath);
        }
        qDebug() << WorkFolder;
        QFile file(ProjectPath);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            stream << saveTheTree();
            file.close();
        }

//    path->setText(fileName);
//    finder->UpdatePath(path->text());
//    treemaker->WorkingDir->setText(WorkFolder);
     MainWindow->setWindowTitle(ProjectPath + " - Taav" );
    }

//    emit saveClick(fileName);
}


void uiMaker::ImportXMLSettingSlot()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr,tr("Import XML File"));

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cant read file";
        return;
    }
    loadSettingFunction(fileName);
    treemaker->WorkingDir->setText(WorkFolder);

}

void uiMaker::ExportXMLSettingSlot()
{
    QString fileName = QFileDialog::getSaveFileName(MainWindow, tr("Export XML file"),
        ProjectPath, tr("XML files (*.xml)"));

    if (!fileName.isEmpty()) {
        QFileInfo fi(fileName);
        if (fi.suffix() != "xml"){
          QString fileNameNew = fi.path() + "/" + fi.completeBaseName() + ".xml";
          QFile().rename(fileName,fileNameNew);
        }
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            stream << saveTheTree();
            file.close();
        }
    }

}



void uiMaker::setupTreeStructure()
{

  tabStructTree = new QTabWidget();

  dockWidgetStructTree = new QDockWidget;
  dockWidgetStructTree->setAllowedAreas(Qt::LeftDockWidgetArea);
  dockWidgetStructTree->setFeatures(QDockWidget::DockWidgetClosable);

  const QString name = "Simulation structure ";
  dockWidgetStructTree->setObjectName(name);
  dockWidgetStructTree->setWindowTitle(name);
  dockWidgetStructTree->setFloating(false);
  dockWidgetStructTree->setWidget(tabStructTree);
//    dockWidgetStructTree->setFeatures(QDockWidget::Reserved);
  dockWidgetStructTree->setMinimumWidth(400);
  dockWidgetStructTree->setMaximumWidth(400);

  MainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidgetStructTree);

}



void uiMaker::createTreeStructure(){

    treemaker = new treeMaker(MainWindow);
    treemaker->startMaking();

    treeEnv = new QTreeWidget;
    treeMaterials = new QTreeWidget;
    treeGeomtry = new QTreeWidget;
    treeSimulation = new QTreeWidget;
    treePost = new QTreeWidget;

    rootTextNodeList.append(treemaker->treeEnv(treeEnv));
    rootTextNodeList.append(treemaker->treeMaterials(treeMaterials));
    rootTextNodeList.append(treemaker->treeGeomtry(treeGeomtry));
    rootTextNodeList.append(treemaker->treeSimulation(treeSimulation));
    rootTextNodeList.append(treemaker->treePost(treePost));

    tabStructTree = new QTabWidget();
    QFont font = tabStructTree->font();
    font.setPointSize(8);
    tabStructTree->setFont(font);

    tabStructTree->addTab(treeEnv,"Environment");
    tabStructTree->addTab(treeMaterials,"Materials");
    tabStructTree->addTab(treeGeomtry,"Geometry");
    tabStructTree->addTab(treeSimulation,"Simulation");
    tabStructTree->addTab(treePost,"Post-processing");
    this->dockWidgetStructTree->setWidget(tabStructTree);
    treemaker->WorkingDir->setText(WorkFolder);

    SetupSlider(treemaker->lineEditStart->text().toInt(),
                treemaker->lineEditFinish->text().toInt(),
                treemaker->lineEditFrequency->text().toInt());


    connect(treemaker->lineEditStart,&QLineEdit::editingFinished,[=](){
              if(sliderMinTime!=treemaker->lineEditStart->text().toInt()){
                  sliderMinTime = treemaker->lineEditStart->text().toInt();
                  updateSlider();}});

    connect(treemaker->lineEditFinish,&QLineEdit::editingFinished,[=](){
              if(sliderMaxTime!=treemaker->lineEditFinish->text().toInt()){
                  sliderMaxTime = treemaker->lineEditFinish->text().toInt();
                  updateSlider();}});

    connect(treemaker->lineEditFrequency,&QLineEdit::editingFinished,[=](){
              if(maxFreq!=treemaker->lineEditFrequency->text().toInt()){
                  maxFreq = treemaker->lineEditFrequency->text().toInt();
                  updateSlider();}});
}


void uiMaker::setupMenuBar()
{

    QMenuBar *menu = MainWindow->menuBar();
    QMenu *file = new QMenu("&File");

    //----------connect hasho bezanam--------------------
    newProject = new QAction("&New Project");
    newProject->setShortcuts(QKeySequence::New);
    file->addAction(newProject);
    connect(newProject, &QAction::triggered,this,&uiMaker::newProjectSlot);

    loadProject = new QAction("&Load Project");
    loadProject->setShortcuts(QKeySequence::Open);
    file->addAction(loadProject);
    connect(loadProject, &QAction::triggered,this,&uiMaker::loadProjectSlot);

    saveSetting = new QAction("&Save Setting");
    saveSetting->setShortcuts(QKeySequence::Save);
    file->addAction(saveSetting);
    connect(saveSetting, &QAction::triggered,this,&uiMaker::saveProjectSlot);

    saveSettingAs = new QAction("&Save Setting As");
    saveSettingAs->setShortcuts(QKeySequence::SaveAs);
    file->addAction(saveSettingAs);
    connect(saveSettingAs, &QAction::triggered,this,&uiMaker::saveAsProjectSlot);


    file->addSeparator();
    ImportXMLSetting = new QAction("&Import XML ...");
    file->addAction(ImportXMLSetting);
    connect(ImportXMLSetting, &QAction::triggered,this,&uiMaker::ImportXMLSettingSlot);

    ExportXMLSetting = new QAction("&Export XML ...");
    file->addAction(ExportXMLSetting);
    connect(ExportXMLSetting, &QAction::triggered,this,&uiMaker::ExportXMLSettingSlot);


    file->addSeparator();

    exit = new QAction("&Exit");
    exit->setShortcuts(QKeySequence::Quit);
    file->addAction(exit);
    connect(exit, &QAction::triggered,this,&uiMaker::exitClick);

    menu->addMenu(file);


    dockWidgetMenu = MainWindow->menuBar()->addMenu(tr("&Views"));
//    setupDockWidgetsGraphic();
//    setupDockWidgetsChart();



    QMenu *aboutMenu = new QMenu("&Help");
    aboutMenu->addAction("&Help");
//    aboutMenu->addAction("&About");
//    aboutMenu->addAction(tr("&About"), this, &uiMaker::about);

//    QMenu *aboutMenu = MainWindow->menuBar()->addMenu(tr("About"));
    QAction *aboutAct = aboutMenu->addAction(tr("&About"), this, &uiMaker::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    menu->addMenu(aboutMenu);
}



void uiMaker::about()
{
  static const char message[] =
      "<p><b>Qt Main Window Example</b></p>"

      "<p>This is a demonstration of the QMainWindow, QToolBar and "
      "QDockWidget classes.</p>"

      "<p>The tool bar and dock widgets can be dragged around and rearranged "
      "using the mouse or via the menu.</p>"

      "<p>Each dock widget contains a colored frame and a context "
      "(right-click) menu.</p>"
      ;

  QMessageBox::about(MainWindow, tr("About MainWindows"), message);
}






void uiMaker::setupDockWidgetsGraphic()
{

  destroyDockWidgetMenuGraphic = new QMenu(tr("Remove graphic view "));
  destroyDockWidgetMenuGraphic->setEnabled(false);
  connect(destroyDockWidgetMenuGraphic, &QMenu::triggered, this, &uiMaker::destroyDockWidgetGraphic);

  dockWidgetMenu->addSeparator();



  QAction *addWidgetGraphic = new QAction("Add graphic view ...");
  addWidgetGraphic->setShortcut(Qt::Key_G | Qt::CTRL);
  dockWidgetMenu->addAction(addWidgetGraphic);
  connect(addWidgetGraphic, &QAction::triggered,this,&uiMaker::createDockWidgetGraphic);

  dockWidgetMenu->addMenu(destroyDockWidgetMenuGraphic);
}

void uiMaker::createDockWidgetGraphic()
{

  QString name;

  int DockWidgetsGraphicNum=0;
  bool NameFlag =true;
  while(NameFlag){
      name ="graphic view " + QString::number(++DockWidgetsGraphicNum) ;
      NameFlag = false;
      foreach (QDockWidget *dock, extraDockWidgetsGraphic)
          if (name == dock->objectName())
              NameFlag = true;
  }

  Dockwidgegraphic *swatch = new Dockwidgegraphic(name, MainWindow, Qt::WindowFlags(false));
  qDebug() << WorkFolder+ImageFolder;
  swatch->setImageFolder(WorkFolder+ImageFolder);
  MainWindow->addDockWidget(Qt::RightDockWidgetArea, swatch);
  extraDockWidgetsGraphic.append(swatch);
  connect(slider,&QSlider::valueChanged,[=](int value){
            swatch->UpdateImage(value+1);});


  destroyDockWidgetMenuGraphic->setEnabled(true);
  destroyDockWidgetMenuGraphic->addAction(new QAction(name, this));


}

void uiMaker::destroyDockWidgetGraphic(QAction *action)
{
  int index = destroyDockWidgetMenuGraphic->actions().indexOf(action);
  delete extraDockWidgetsGraphic.takeAt(index);
  destroyDockWidgetMenuGraphic->removeAction(action);
  action->deleteLater();

  if (destroyDockWidgetMenuGraphic->isEmpty())
      destroyDockWidgetMenuGraphic->setEnabled(false);
}




void uiMaker::setupDockWidgetsChart()
{

  destroyDockWidgetMenuChart = new QMenu(tr("Remove chart "));
  destroyDockWidgetMenuChart->setEnabled(false);
  connect(destroyDockWidgetMenuChart, &QMenu::triggered, this, &uiMaker::destroyDockWidgetChart);

  dockWidgetMenu->addSeparator();

  QAction *addWidgetChart = new QAction("Add chart view ...");
  addWidgetChart->setShortcut(Qt::Key_J | Qt::CTRL);
  dockWidgetMenu->addAction(addWidgetChart);
  connect(addWidgetChart, &QAction::triggered,this,&uiMaker::createDockWidgetChart);
  dockWidgetMenu->addMenu(destroyDockWidgetMenuChart);
}

void uiMaker::createDockWidgetChart()
{

  QString name;

  int DockWidgetsChartNum=0;
  bool NameFlag =true;
  while(NameFlag){
      name = "Chart view " + QString::number(++DockWidgetsChartNum) ;
      NameFlag = false;
      foreach (QDockWidget *dock, extraDockWidgetsChart)
          if (name == dock->objectName())
              NameFlag = true;
  }

  dockwidgetchart *dwdockChart = new dockwidgetchart(name, MainWindow, Qt::WindowFlags(false));
  dwdockChart->setChartFile(WorkFolder+"SimulationResults.dat");
  dwdockChart->setAllowedAreas(Qt::RightDockWidgetArea | Qt::TopDockWidgetArea );
  MainWindow->addDockWidget(Qt::RightDockWidgetArea, dwdockChart);
  dwdockChart->setObjectName(name);
  dwdockChart->setWindowTitle(name);
  dwdockChart->setFeatures(QDockWidget::Reserved);
  dwdockChart->setMinimumHeight(200);
  dwdockChart->setMinimumWidth(400);



  MainWindow->addDockWidget(Qt::RightDockWidgetArea, dwdockChart);
  extraDockWidgetsChart.append(dwdockChart);
  destroyDockWidgetMenuChart->setEnabled(true);
  destroyDockWidgetMenuChart->addAction(new QAction(name, this));

}

void uiMaker::destroyDockWidgetChart(QAction *action)
{
  int index = destroyDockWidgetMenuChart->actions().indexOf(action);
  delete extraDockWidgetsChart.takeAt(index);
  destroyDockWidgetMenuChart->removeAction(action);
  action->deleteLater();

  if (destroyDockWidgetMenuChart->isEmpty())
      destroyDockWidgetMenuChart->setEnabled(false);
}


void uiMaker::setupDockWidgetsMessage()
{

    TextOutPut = new QTextEdit();
    TextOutPut->setReadOnly(true);
    TextOutPut->setText("program is started ... ");

    dwMessageBox = new QDockWidget;
    dwMessageBox->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea);

    const QString name = "Message" ;
    dwMessageBox->setObjectName(name);
    dwMessageBox->setWindowTitle(name);
    dwMessageBox->setWidget(TextOutPut);
    dwMessageBox->setFeatures(QDockWidget::Reserved);
    dwMessageBox->setMinimumHeight(200);
    dwMessageBox->setMinimumWidth(400);
//    dwMessageBox->setMaximumHeight(400);

    MainWindow->addDockWidget(Qt::RightDockWidgetArea, dwMessageBox);

}


void uiMaker::setupDockWidgetsStatus()
{

      toolBar = new QToolBar();

      const QString name = "Status View";
      toolBar->setObjectName(name);
      toolBar->setWindowTitle(name);
      toolBar->setFloatable(false);
      toolBar->setMovable(false);
      toolBar->setMinimumHeight(70);
      toolBar->setMaximumHeight(70);
      MainWindow->addToolBar(Qt::BottomToolBarArea,toolBar);

      //_--------------------------------------------
      QWidget *containerBottom = new QWidget;

      toolBar->addWidget(containerBottom);
      //    containerBottom->setFixedWidth(300);
      QHBoxLayout *bottomLayout = new QHBoxLayout;
      bottomLayout->setMargin(0);
      containerBottom->setLayout(bottomLayout);

      QWidget *containerRight = new QWidget;
      containerMiddle = new QWidget;
      QWidget *containerleft = new QWidget;
      //______________________________________________________________

      bottomLayout->addWidget(containerleft,0,Qt::AlignLeft);
      QGridLayout *gridlayoutLeft = new QGridLayout;
      gridlayoutLeft->setSpacing(1);
      containerleft->setLayout(gridlayoutLeft);
      start = new QPushButton("Start");
      start->setDisabled(true);
      connect(start,&QPushButton::clicked,this,
              [=](){
          saveProjectSlot();
          emit clickStart(startPuaseState,NumberOfRacks->text().toInt(),ProjectPath);}
      );

      stop = new QPushButton("Stop");
      connect(stop,&QPushButton::clicked,this,&uiMaker::clickStop);
      connect(stop,&QPushButton::clicked,this,&uiMaker::StopUI);
      stop->setDisabled(true);


      QLabel *label = new QLabel("  Processors: ");
      QFont f( "Arial", 9);
      label->setFont( f);

      NumberOfRacks = new QLineEdit("1");
      QFont f1( "Arial", 8);
      NumberOfRacks->setFont(f1);
//      NumberOfRacks->setFixedWidth(35);
//      NumberOfRacks->setFixedHeight(20);
      NumberOfRacks->setValidator( new QIntValidator(0, 1000, this) );
      NumberOfRacks->setFixedSize(QSize(40,20));

      connect(NumberOfRacks,&QLineEdit::editingFinished,[=](){
        if(NumberOfRacks->text().toInt() == 0)
            NumberOfRacks->setText("1");
        });

      progressBar = new QProgressBar;
      progressBar->setRange(0,100);
      progressBar->setValue(0);
      gridlayoutLeft->addWidget(start,0,0,1,1);
      gridlayoutLeft->addWidget(stop,0,1,1,1);
      gridlayoutLeft->addWidget(progressBar,1,0,1,4);
      gridlayoutLeft->addWidget(label,0,2,1,1);
      gridlayoutLeft->addWidget(NumberOfRacks,0,3,1,1);



      bottomLayout->addWidget(containerMiddle);
      layoutMiddle = new QGridLayout;
      containerMiddle->setLayout(layoutMiddle);
      slider = new QSlider(Qt::Horizontal);
//      slider->setRange(1, 5);

      slider->setFocusPolicy(Qt::StrongFocus);
      slider->setTickPosition(QSlider::TicksBothSides);
      slider->setTickInterval(1);
      slider->setSingleStep(1);




      slidersteptolabel = (sliderMaxTime-sliderMinTime) / (maxFreq);
      slider->setMinimum(0);
      slider->setMaximum(0);

      layoutMiddle->addWidget(slider, 0, 0, 1, NumberofLabels);
      for(int i=0; i<NumberofLabels;i++){
        QLabel *label = new QLabel("", MainWindow);
        layoutMiddle->addWidget(label, 1, i, 1, 1);
        Labels.append(label);
      }

//    slider->setRange(10,5);








      //containerMiddle->setStyleSheet("* { background-color: blue }");
      QSizePolicy sizePolicy;
      sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
      containerMiddle->setSizePolicy(sizePolicy);
//      layoutMiddle->addWidget(slider);
      //-----------------------------------------------------------------------

      bottomLayout->addWidget(containerRight,0,Qt::AlignRight);
      QGridLayout *gridlayoutRight = new QGridLayout;
      gridlayoutRight->setSpacing(1);
      containerRight->setFixedWidth(200);
      containerRight->setLayout(gridlayoutRight);

      QPixmap pixmap;QIcon icon;QSize size(14,14);

      previousFrame = new QPushButton();
      previousFrame->setIcon(QIcon(QPixmap(":/img/n2.png")));previousFrame->setIconSize(size);
      previousFrame->setFixedWidth(49);
      previousFrame->setShortcut(Qt::Key_Left);
      connect(previousFrame,&QPushButton::clicked,[=](){
          slider->setValue(slider->value()-1);
      });
//      timer = new QTimer(this);
//      connect(timer, SIGNAL(timeout()), this, SLOT( previousFrametimer() ) );


      nextFrame = new QPushButton();
      nextFrame->setIcon(QIcon(QPixmap(":/img/p2.png")));nextFrame->setIconSize(size);
      nextFrame->setFixedWidth(49);
      nextFrame->setShortcut(Qt::Key_Right);
      connect(nextFrame,&QPushButton::clicked,[=](){
          slider->setValue(slider->value()+1);

      });
//      timer = new QTimer(this);
//      connect(timer, SIGNAL(timeout()), this, SLOT( nextFrametimer() ) );


//        lineEditFrame = new QLineEdit;
//        lineEditFrame->setInputMask("00000");
//        lineEditFrame->setText("0");


        lineEditFrame = new QLineEdit("0");
        lineEditFrame->setFont(QFont( "Arial", 9));
        lineEditFrame->setValidator( new QIntValidator(0, 1000000000, this) );
//        lineEditFrame->setFixedSize(QSize(40,20));


        firstFrame = new QPushButton();
        firstFrame->setIcon(QIcon(QPixmap(":/img/d2.png")));firstFrame->setIconSize(size);
        firstFrame->setShortcut(Qt::Key_Left | Qt::CTRL);

        jumppreviousFrame = new QPushButton();
        jumppreviousFrame->setIcon(QIcon(QPixmap(":/img/p1.png")));jumppreviousFrame->setIconSize(size);

        jumpnextFrame = new QPushButton();
        jumpnextFrame->setIcon(QIcon(QPixmap(":/img/n1.png")));jumpnextFrame->setIconSize(size);

        lastFrmae = new QPushButton();
        lastFrmae->setIcon(QIcon(QPixmap(":/img/d1.png")));lastFrmae->setIconSize(size);
        lastFrmae->setShortcut(Qt::Key_Right | Qt::CTRL);

        gridlayoutRight->addWidget(previousFrame,0,0,1,1);
        gridlayoutRight->addWidget(nextFrame,0,1,1,1);
        gridlayoutRight->addWidget(lineEditFrame,0,2,1,2);
        gridlayoutRight->addWidget(firstFrame,1,0,1,1);
        gridlayoutRight->addWidget(jumppreviousFrame,1,1,1,1);
        gridlayoutRight->addWidget(jumpnextFrame,1,2,1,1);
        gridlayoutRight->addWidget(lastFrmae,1,3,1,1);

        connect(lineEditFrame,&QLineEdit::editingFinished,[=](){
          if(slider->value()!=lineEditFrame->text().toInt()/slidersteptolabel -sliderMinTime)
              slider->setValue(lineEditFrame->text().toInt()/slidersteptolabel -sliderMinTime);});


        connect(slider,&QSlider::valueChanged,[=](int value){
                  QString s = QString::number(value*slidersteptolabel + sliderMinTime);
                  lineEditFrame->setText(s);});
        connect(firstFrame,&QPushButton::clicked,[=](){
            slider->setValue(slider->minimum());
        });
        connect(lastFrmae,&QPushButton::clicked,[=](){
            slider->setValue(slider->maximum());
        });

        connect(jumpnextFrame,&QPushButton::clicked,[=](){
            slider->setValue(slider->value()+2);
        });
        connect(jumppreviousFrame,&QPushButton::clicked,[=](){
            slider->setValue(slider->value()-2);
        });
        qDebug() << NumberOfRacks->text() ;



//        connect(this,&MainWindow::clickStart,worker,&Worker::clickStart_Pause);
//        connect(this,&MainWindow::clickStop,worker,&Worker::clickStop);
//        connect(worker,&Worker::LogTextAppend,this,&MainWindow::logTextSet);

//        connect(worker,&Worker::PauseUI,this,&MainWindow::PauseUI);
//    //    connect(worker,&Worker::PauseUI,this,&MainWindow::ProcessStop);
//        connect(worker,&Worker::StartUI,this,&MainWindow::StartUI);
//    //    connect(worker,&Worker::StartUI,this,&MainWindow::ProcessStart);

//        connect(worker,&Worker::changeProgressBar,this,&MainWindow::changeProgressBar);


        MainWindow->statusBar()->showMessage(tr("Status Bar is here."));

}

void uiMaker::updateSlider(){


  slidersteptolabel = abs((sliderMaxTime-sliderMinTime)) / (maxFreq);
  int steplabel = abs((sliderMaxTime-sliderMinTime)) / (NumberofLabels);
  slider->setMinimum(0);
  slider->setMaximum(maxFreq);


  for(int i=0; i<NumberofLabels;i++){
      if (sliderMaxTime <=  sliderMinTime){
          Labels[i]->clear();
      }
      else{
          Labels[i]->setText( QString::number(i*steplabel+sliderMinTime));
      }
  }

  qDebug() << "void uiMaker::updateSlider(){" ;
  slider->setValue(0);
  lineEditFrame->setText(QString::number(sliderMinTime));
}


void uiMaker::SetupSlider(int startframe,int lastframe, int stepframe){


  sliderMinTime = startframe;
  sliderMaxTime = lastframe;
  maxFreq = stepframe;

  updateSlider();
 qDebug() << treemaker->lineEditFinish->text().toInt();

}

QString uiMaker::saveTheTree()
{
    QByteArray xmlData("<?xml version='1.0'?>");

    QDomDocument doc;
    doc.setContent(xmlData);
    QDomElement root = doc.createElement("projectTree");
    doc.appendChild(root);



    node *childWorkingNode = nullptr;
    node *workingNode = nullptr;

    QDomElement workingDom;

    for(int i = 0;i<rootTextNodeList.size();i++)
    {
       node *rootNode = rootTextNodeList.at(i);

       QStack<node*> stackNode;
       stackNode.push(rootNode);



       QDomElement parentElement = doc.createElement(rootNode->getName());
       QDomElement parentValu = doc.createElement("value");
       QDomText parentText = doc.createTextNode(rootNode->getData());

       parentValu.appendChild(parentText);
       parentElement.appendChild(parentValu);
       root.appendChild(parentElement);


       QStack<QDomElement> stackDom;
       stackDom.push(parentElement);

       qDebug() << stackNode.count();


       while(!stackNode.isEmpty())
       {
           workingNode = stackNode.pop();
           workingDom = stackDom.pop();
           //some work on working node

           //------------------------
           for(int j=0 ; j<workingNode->childNodes.count() ; j++)
           {
               childWorkingNode = workingNode->childNodes.at(j);
               stackNode.push(childWorkingNode);

               QDomElement childElement = doc.createElement(childWorkingNode->getName());
               QDomElement childValu = doc.createElement("value");
               QDomText childText = doc.createTextNode(childWorkingNode->getData());

               stackDom.push(childElement);

               childValu.appendChild(childText);
               childElement.appendChild(childValu);
               workingDom.appendChild(childElement);
           }
       }
    }



    return doc.toString();
}



void uiMaker::resetDockWidgets(){

//    delete dockWidgetStructTree;
//    delete tabStructTree;
    delete dockWidgetStructTree;

    for (int i=0; i<extraDockWidgetsGraphic.size(); ++i){
              extraDockWidgetsGraphic[i]->deleteLater();
    }
    for (int i=0; i<extraDockWidgetsChart.size(); ++i){
              extraDockWidgetsChart[i]->deleteLater();
    }

    extraDockWidgetsGraphic.clear();
    extraDockWidgetsChart.clear();
    dockWidgetMenu->clear();
    toolBar->clear();
    rootTextNodeList.clear();
    TextOutPut->clear();
    Labels.clear();


}
