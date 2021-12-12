#include "mainwindow.h"
#include <QApplication>
#include<QFile>
#include<QTextStream>


int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    MainWindow mainWin;
    mainWin.resize(1200, 800);
    mainWin.show();

    return a.exec();
}
