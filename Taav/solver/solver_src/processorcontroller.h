#ifndef PROCESSORCONTROLLER_H
#define PROCESSORCONTROLLER_H

#include "palabos2D.h"
#include "palabos2D.hh"
#include "Ui_Parameter_Struct.h"
#include "Convert_Script_to_number.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <bits/stdc++.h>
#include <iostream>
#include <sys/types.h>

void ProcessStart(std::string WorkFolder) {
    std::string fileNameStream = WorkFolder + "processPID.txt";
    plb::plb_ofstream ofs(fileNameStream.c_str());
    ofs <<  getpid() << std::endl;
}

template <class Time>
void ProcesstakesperItr(Time timeperitr, std::string WorkFolder) {
    std::string fileNameStream = WorkFolder + "processPID.txt";
    plb::plb_ofstream ofs(fileNameStream.c_str(), std::ios_base::app);
    ofs << timeperitr;
}

void ControlStart(plb::plint a, std::string WorkFolder) {
    plb::plint NumberOfRacks = plb::global::mpi().getSize() ;
    std::string fileNameStream = WorkFolder + "Control.txt";
    plb::plb_ofstream ofs(fileNameStream.c_str());
    ofs <<  a << " " << NumberOfRacks << std::endl;
}

void Updatepercentage(double SimulationPercent, std::string WorkFolder){

    std::string fileNameStream = WorkFolder + "processPID.txt";
    plb::plb_ifstream in(fileNameStream.c_str());
    plb::plint a;
    double SimulaTimePerItr, SimulationPercentPrevious;
    if(in.is_open())
    in >>  a >> SimulaTimePerItr >> SimulationPercentPrevious;

    plb::plb_ofstream ofs(fileNameStream.c_str());
    ofs <<  a << " " << SimulaTimePerItr << " " << SimulationPercent <<  std::endl;
}


plb::plint ControlStop(plb::plint& NumberOfRacks, std::string WorkFolder) {

    std::string fileNameStream = WorkFolder + "Control.txt";
    std::ifstream in(fileNameStream.c_str());
    plb::plint flag=0;

    if(in.is_open()){
    //~ in.open();
     while(!in.eof())
    {
        in >> flag >> NumberOfRacks;
    }
    in.close();
    }

    return flag;
}

void createFolder(std::string  foldername){

if (plb::global::mpi().isMainProcessor()) {
    mkdir(foldername.c_str(), 0777);
}
}


#endif // PROCESSORCONTROLLER_H
