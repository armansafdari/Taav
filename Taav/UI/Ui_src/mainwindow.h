#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<worker.h>
#include<uimaker.h>
#include <picfinder.h>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Worker *worker;
    uiMaker *uimaker;



//    QString saveTheTree();

public slots:
    void savefile(QString s);
    void openfile(QString s);
    void newProject(QString);
    void loadProject(QString);
    void saveSetting();

//    void NewProject();
//    void saveLayout();
//    void loadLayout();
//    void about();

    void ProcessStart();
    void ProcessStop();


};

#endif // MAINWINDOW_H
