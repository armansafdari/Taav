#include "mainwindow.h"

#include<QLayout>

#include <textnode.h>
#include<QtDebug>
#include<QDir>
#include<QFile>
#include<QDomDocument>
#include<node.h>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    uimaker = new uiMaker(this);
//    uimaker->makeIt();


    QThread *thread = new QThread;
//    worker = new Worker(uimaker->NumberOfRacks,uimaker->ProjectPath);
    worker = new Worker();

    worker->moveToThread(thread);
    worker->makeTimer();
    thread->start();
//    picture finder class







    connect(uimaker,&uiMaker::exitClick,this,&MainWindow::close);
    connect(uimaker,&uiMaker::openClick,this,&MainWindow::openfile);
    connect(uimaker,&uiMaker::saveClick,this,&MainWindow::savefile);
    connect(uimaker,&uiMaker::newProjectClick,this,&MainWindow::newProject);
    connect(uimaker,&uiMaker::loadProjectClick,this,&MainWindow::loadProject);
    connect(uimaker,&uiMaker::saveSettingClick,this,&MainWindow::saveSetting);



    //worker class


    connect(uimaker,&uiMaker::clickStart,worker,&Worker::clickStart_Pause);
    connect(uimaker,&uiMaker::clickStop,worker,&Worker::clickStop);

    connect(worker,&Worker::PauseUI,uimaker,&uiMaker::PauseUI);
    connect(worker,&Worker::PauseUI,this,&MainWindow::ProcessStop);
    connect(worker,&Worker::StartUI,uimaker,&uiMaker::StartUI);
    connect(worker,&Worker::StartUI,this,&MainWindow::ProcessStart);

    connect(worker,&Worker::UpdateMessageBoxworker,uimaker,&uiMaker::UpdateMessageBoxUi);
    connect(worker,&Worker::AppendMessageBoxworker,uimaker,&uiMaker::AppendMessageBoxUi);

    connect(worker,&Worker::changeProgressBarworker,uimaker,&uiMaker::changeProgressBarUi);


//    connect(worker,&Worker::LogTextAppend,uimaker,&uiMaker::logTextSet);
}

MainWindow::~MainWindow()
{

}

//QString MainWindow::saveTheTree()
//{
//    QByteArray xmlData("<?xml version='1.0'?>");

//    QDomDocument doc;
//    doc.setContent(xmlData);
//    QDomElement root = doc.createElement("projectTree");
//    doc.appendChild(root);



//    node *childWorkingNode = nullptr;
//    node *workingNode = nullptr;

//    QDomElement workingDom;

//    for(int i = 0;i<uimaker->rootTextNodeList.size();i++)
//    {
//       node *rootNode = uimaker->rootTextNodeList.at(i);

//       QStack<node*> stackNode;
//       stackNode.push(rootNode);



//       QDomElement parentElement = doc.createElement(rootNode->getName());
//       QDomElement parentValu = doc.createElement("value");
//       QDomText parentText = doc.createTextNode(rootNode->getData());

//       parentValu.appendChild(parentText);
//       parentElement.appendChild(parentValu);
//       root.appendChild(parentElement);


//       QStack<QDomElement> stackDom;
//       stackDom.push(parentElement);

//       qDebug() << stackNode.count();


//       while(!stackNode.isEmpty())
//       {
//           workingNode = stackNode.pop();
//           workingDom = stackDom.pop();
//           //some work on working node




//           //------------------------
//           for(int j=0 ; j<workingNode->childNodes.count() ; j++)
//           {
//               childWorkingNode = workingNode->childNodes.at(j);
//               stackNode.push(childWorkingNode);

//               QDomElement childElement = doc.createElement(childWorkingNode->getName());
//               QDomElement childValu = doc.createElement("value");
//               QDomText childText = doc.createTextNode(childWorkingNode->getData());

//               stackDom.push(childElement);

//               childValu.appendChild(childText);
//               childElement.appendChild(childValu);
//               workingDom.appendChild(childElement);





//           }
//       }
//    }



//    return doc.toString();
//}

void MainWindow::openfile(QString s)
{
    //nothing
}

void MainWindow::newProject(QString s)
{
//    qDebug() << "the new project MainWindow." << s;
//    QFile file(s);
//    file.open(QIODevice::WriteOnly);
//    QTextStream stream(&file);
//    stream << saveTheTree();
//    file.close();
    qDebug() << s;
}

void MainWindow::loadProject(QString)
{
    qDebug() << "load project MainWindow.";

}

void MainWindow::saveSetting()
{
     qDebug() << "save project MainWindow.";
//    QFile file(uimaker->path->text());
//    file.open(QIODevice::WriteOnly);
//    QTextStream stream(&file);
//    stream << saveTheTree();
//    file.close();
}

void MainWindow::ProcessStart()
{

}

void MainWindow::ProcessStop()
{

}

void MainWindow::savefile(QString s)
{


//    QFile file(s);
//    file.open(QIODevice::WriteOnly);
//    QTextStream stream(&file);
//    stream << saveTheTree();
//    file.close();
    qDebug() << s;
}
