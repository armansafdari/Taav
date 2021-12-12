#ifndef PICFINDER_H
#define PICFINDER_H

#include <QObject>
#include <QTimer>
#include "QDir"

class PicFinder : public QObject
{
    Q_OBJECT
private:
    QTimer *timer;
    QString workingPath_Density;
    QString workingPath_velocity;
    QDir dir_Density;
    QDir dir_velocity;


    int lastLenth;
public:
    explicit PicFinder(QObject *parent = nullptr);

signals:
    void changeMaxOfSlider(int a,QStringList list1,QStringList list2);
public slots:
    void TimerFinished();
    void StartSearch();
    void StopSearch();

    void UpdatePath(QString s);
};

#endif // PICFINDER_H
