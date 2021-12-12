#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include "QTimer"
#include <QLineEdit>
#include <QtCharts>

class Worker : public QObject
{
    Q_OBJECT
private:
    QTimer *timer;
    QLineSeries *series;
    QString WorkFolder;
    QString XMLfilename;
    QString TextOld="";
    QString TextNew="";
    QString PID;
    int TimeItr=0;
    QString Percentage=nullptr;
    QString processStatus="";
    int NumberOfRacks=1;

public:
//    explicit Worker(QLineEdit *NumberOfRacks,QString ProjectPath, QObject *parent = nullptr);
    explicit Worker(QObject *parent = nullptr);
    void makeTimer();
    void StartTheProgram();
    void readOutPutFile();
    void readControlFile();
    int readProccessorFile();

    void writeControlFile();
    void writeProccessorFile();
    void deletePreResults();

signals:
    void LogTextAppend(QString);
    void StartUI();
    void PauseUI();
    void changeProgressBarworker(int i);
    void UpdateMessageBoxworker(QString str);
    void AppendMessageBoxworker(QString str);
    void addLineSeries(QLineSeries *series);
    void updateChart();
public slots:
    void clickStart_Pause(int a,int R,QString filename);
    void clickStop();
    void timerFunction();

};

#endif // WORKER_H
