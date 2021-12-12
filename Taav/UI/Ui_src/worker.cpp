#include "worker.h"
#include "QDebug"


Worker::Worker(QObject *parent) : QObject(parent)
{


//    qDebug()<< "Number of Racks in Worker Constructor" << NumberOfRacks->text()<<path->text();


}

void Worker::makeTimer()
{
    timer = new QTimer;
    timer->moveToThread(this->thread());
    connect(timer,&QTimer::timeout,this,&Worker::timerFunction);

}

void Worker::clickStart_Pause(int PushButtonStatus, int _NumberOfRacks, QString _XMLfilename)
{

//    qDebug() << this->thread() << timer->thread();

    qDebug() << PushButtonStatus;
    qDebug() << _NumberOfRacks;
//    emit StartUI();
    if(PushButtonStatus==0){

        XMLfilename = _XMLfilename;
        QFileInfo fi(XMLfilename);
        WorkFolder  = fi.path() + "/";
        NumberOfRacks = _NumberOfRacks;
        TimeItr = 1000;
        timer->start(TimeItr);


        qDebug()<< "timer->interval() " << timer->interval();
        deletePreResults();
        StartTheProgram();
        emit changeProgressBarworker(readProccessorFile());
        emit UpdateMessageBoxworker("program started!!!");
        emit AppendMessageBoxworker(XMLfilename);

        emit StartUI();

    }
    else if(PushButtonStatus==1){

        qDebug() << "start clicked ; filename " << XMLfilename;
        timer->stop();

        emit PauseUI();
    }



}


void Worker::clickStop()
{
    qDebug() << "stop clicked";
    emit PauseUI();
    timer->stop();

    readProccessorFile();

    QString Command = "kill " + PID ;
    int retunKillProcess = system(qPrintable(Command));
    qDebug() << qPrintable(Command) << " : " <<  retunKillProcess;

    readOutPutFile();
    if (TextOld!=TextNew){
    emit UpdateMessageBoxworker(TextNew); //You can emit this signal where ever you want
     TextOld = TextNew;
    }


}



void Worker::StartTheProgram(){

     QString ComamndLine = "mpirun -np " + QString::number(NumberOfRacks) + " ./solver "
                            + XMLfilename + " > " + WorkFolder + "output.txt &";

    qDebug() << qPrintable(ComamndLine);

    int returnCode = system(qPrintable(ComamndLine));
    qDebug() << returnCode;
}


void Worker::timerFunction()
{
    emit changeProgressBarworker(readProccessorFile());
    readOutPutFile();
    if (TextOld!=TextNew){
        QString tmpText = TextNew;
        tmpText.remove(TextOld);
    emit AppendMessageBoxworker(tmpText);
     TextOld = TextNew;
    }

    if (readProccessorFile() == 100 ){
        emit PauseUI();
        timer->stop();
    }

    if(TimeItr != timer->interval()){
        timer->stop();
        timer->start(TimeItr);
    }

}


void Worker::readOutPutFile(){

    qDebug() << "readOutPutFile = " << WorkFolder + "output.txt" ;
    QFile file1(WorkFolder + "output.txt");
    if(!file1.open(QIODevice::ReadOnly)) {
//        QMessageBox::information(nullptr, "error", file1.errorString());
        return;
    }

    QTextStream in(&file1);
    TextNew = in.readAll();
    file1.close();

}


int Worker::readProccessorFile(){

    QFile file(WorkFolder + "processPID.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        return 0;

    }

    QTextStream in(&file);

    QString Time;
    in >> PID >> Time >> Percentage;
    file.close();
    if(Time.toFloat() < 1000 || Time.isEmpty() ){
        Time = "5000";
    }
    TimeItr = Time.split(".")[0].toInt();
    return Percentage.toInt();
}

void Worker::readControlFile(){

    QFile file(WorkFolder + "Control.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr, "error", file.errorString());
    }

    QTextStream in(&file);
    in >> processStatus >> NumberOfRacks;

    file.close();

}

void Worker::writeControlFile(){

    QFile Ofs(WorkFolder + "Control.txt");
    if(!Ofs.open(QIODevice::WriteOnly)) {
        QMessageBox::information(nullptr, "error", Ofs.errorString());
    }
    QTextStream Out(&Ofs);
    Out << processStatus << " " << NumberOfRacks;

    Ofs.close();


}
void Worker::writeProccessorFile(){



}

void Worker::deletePreResults(){
    QDir dir(WorkFolder);
    dir.setSorting(QDir::Name);
    QStringList dirList = dir.entryList();
    QDir removeFolder(WorkFolder+ "results");
    removeFolder.removeRecursively();
    removeFolder.setPath(WorkFolder+ "checkpoint");
    removeFolder.removeRecursively();

}
