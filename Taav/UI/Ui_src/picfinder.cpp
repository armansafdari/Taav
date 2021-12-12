#include "picfinder.h"
#include <QDebug>


PicFinder::PicFinder(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&PicFinder::TimerFinished);

    lastLenth =0;
}

void PicFinder::TimerFinished()
{
    dir_Density.refresh();
    dir_velocity.refresh();
    QStringList dirList_Density = dir_Density.entryList();
    QStringList dirList_velocity = dir_velocity.entryList();
    if(lastLenth !=dirList_Density.length()) {
        emit changeMaxOfSlider( dirList_Density.length(),dirList_Density,dirList_velocity);
        lastLenth = dirList_Density.length();

    }

}

void PicFinder::StartSearch()
{

    dir_Density.setPath(workingPath_Density);
    QStringList filters;
    filters << "*.gif" << "*.ppm";
    dir_Density.setNameFilters(filters);
    dir_Density.setSorting(QDir::Name);

    dir_velocity.setPath(workingPath_velocity);
    dir_velocity.setNameFilters(filters);
    dir_velocity.setSorting(QDir::Name);


    timer->start(2000);
}

void PicFinder::StopSearch()
{
    qDebug() << "stopSearch";
    timer->stop();
}

void PicFinder::UpdatePath(QString s)
{
    workingPath_Density = s + "/results/Image/Density_1";
    workingPath_velocity = s + "/results/Image/Velocity_1";
}
