#ifndef POSTPROCESSINGFUNCTIONS_H
#define POSTPROCESSINGFUNCTIONS_H


#include "palabos2D.h"
#include "palabos2D.hh"

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "switch_flags.h"
#include "Convert_Script_to_number.h"
#include "Ui_Parameter_Struct.h"
#include "processorcontroller.h"



template< class BlockLatticeT>
void writeImages(BlockLatticeT& lattice, std::vector<plb::Box2D>& ImageFileArea, plb::plint iter, Parameters Param)
{
    const plb::plint imSize = 600;

    for (uint i=0 ; i < ImageFileArea.size(); ++i) {

        if( Param.FlagVelocityImage){
            plb::ImageWriter<double> imageWriter(Param.ColormapVelocityImage);
            imageWriter.writeScaledGif(plb::createFileName("results/Image/Velocity_" + Number_to_string(i) + "/VelocityNorm_" + Number_to_string(i)+ "_", iter, 6),
                                       *computeVelocityNorm(lattice,ImageFileArea[i]),
                                       imSize, imSize );
        }

//       if(Param.FlagVorticityImage){
//           plb::ImageWriter<double> imageWriter(Param.ColormapVorticityImage);
//           imageWriter.writeScaledGif(plb::createFileName("results/Image/Vorticity_" + Number_to_string(i) + "/Vorticity_" + Number_to_string(i)+ "_", iter, 6),
//                                      *computeNorm(*computeVorticity (*computeVelocity(lattice,ImageFileArea[i]))),
//                                      imSize, imSize );
//       }

        if (Param.FlagDensityImage){
            plb::ImageWriter<double> imageWriter(Param.ColormapDensityImage);
            imageWriter.writeScaledGif(plb::createFileName("results/Image/Density_" + Number_to_string(i) + "/Density_" + Number_to_string(i)+ "_", iter, 6),
                                       *computeDensity(lattice,ImageFileArea[i]),
                                       imSize, imSize );
        }

    }

}

template<class BlockLatticeT>
void writeVTK(BlockLatticeT& lattice, std::vector<plb::Box2D>& VtkFileArea, plb::plint iter, Parameters Param)
{
    double dx = Param.length_coefficient;
    double dt = Param.time_coefficient;
    double dm = Param.mass_coefficient;

    for (uint i=0 ; i < VtkFileArea.size(); ++i) {

        switch_flags_3( Param.FlagVelocityVtk, Param.FlagVorticityVtk,Param.FlagDensityVtk )
        {
            case flags_3(T,F,F):
            {
                std::string fileNameStream = "results/Vtk/VTK_" + Number_to_string(i) + "_" ;
                plb::VtkImageOutput2D<double> vtkOut(plb::createFileName(fileNameStream.c_str(), iter, 6), dx);
                vtkOut.writeData<float>(*computeVelocityNorm(lattice,VtkFileArea[i]), "velocityNorm", dx/dt);
                vtkOut.writeData<2,float>(*computeVelocity(lattice,VtkFileArea[i]), "velocity", dx/dt);

              break;
            }
            case flags_3(F,T,F):
            {
                std::string fileNameStream = "results/Vtk/VTK_" + Number_to_string(i) + "_" ;
                plb::VtkImageOutput2D<double> vtkOut(plb::createFileName(fileNameStream.c_str(), iter, 6), dx);
//                vtkOut.writeData<2,float>(*computeVorticity(*computeVelocity(lattice,VtkFileArea[i])), "vorticity", 1./dt);


              break;
            }
            case flags_3(F,F,T):
            {
                std::string fileNameStream = "results/Vtk/VTK_" + Number_to_string(i) + "_" ;
                plb::VtkImageOutput2D<double> vtkOut(plb::createFileName(fileNameStream.c_str(), iter, 6), dx);
                vtkOut.writeData<float>(*computeDensity(lattice,VtkFileArea[i]), "density", dm);

              break;
            }
            case flags_3(T,T,F):
            {
                std::string fileNameStream = "results/Vtk/VTK_" + Number_to_string(i) + "_" ;
                plb::VtkImageOutput2D<double> vtkOut(plb::createFileName(fileNameStream.c_str(), iter, 6), dx);
                vtkOut.writeData<float>(*computeVelocityNorm(lattice,VtkFileArea[i]), "velocityNorm", dx/dt);
                vtkOut.writeData<2,float>(*computeVelocity(lattice,VtkFileArea[i]), "velocity", dx/dt);
//                vtkOut.writeData<2,float>(*computeVorticity(*computeVelocity(lattice,VtkFileArea[i])), "vorticity", dx/dt);


                break;
            }
            case flags_3(F,T,T):
            {
                std::string fileNameStream = "results/Vtk/VTK_" + Number_to_string(i) + "_" ;
                plb::VtkImageOutput2D<double> vtkOut(plb::createFileName(fileNameStream.c_str(), iter, 6), dx);
//                vtkOut.writeData<2,float>(*computeVorticity(*computeVelocity(lattice,VtkFileArea[i])), "vorticity", dx/dt);
                vtkOut.writeData<float>(*computeDensity(lattice,VtkFileArea[i]), "density", dm);


                break;
            }
            case flags_3(T,F,T):
            {
                std::string fileNameStream = "results/Vtk/VTK_" + Number_to_string(i) + "_" ;
                plb::VtkImageOutput2D<double> vtkOut(plb::createFileName(fileNameStream.c_str(), iter, 6), dx);
                vtkOut.writeData<float>(*computeVelocityNorm(lattice,VtkFileArea[i]), "velocityNorm", dx/dt);
                vtkOut.writeData<2,float>(*computeVelocity(lattice,VtkFileArea[i]), "velocity", dx/dt);
                vtkOut.writeData<float>(*computeDensity(lattice,VtkFileArea[i]), "density", dm);


                break;
            }
            case flags_3(T,T,T):
            {
                std::string fileNameStream = "results/Vtk/VTK_" + Number_to_string(i) + "_" ;
                plb::VtkImageOutput2D<double> vtkOut(plb::createFileName(fileNameStream.c_str(), iter, 6), dx);
                vtkOut.writeData<float>(*computeVelocityNorm(lattice,VtkFileArea[i]), "velocityNorm", dx/dt);
                vtkOut.writeData<2,float>(*computeVelocity(lattice,VtkFileArea[i]), "velocity", dx/dt);
//                vtkOut.writeData<2,float>(*computeVorticity(*computeVelocity(lattice,VtkFileArea[i])), "vorticity", dx/dt);
                vtkOut.writeData<float>(*computeDensity(lattice,VtkFileArea[i]), "density", dm);


                break;
            }
        }

    }

}


template<class BlockLatticeT>
void writetxt(BlockLatticeT& lattice, std::vector<plb::Box2D>& TextFileArea, plb::plint iter, Parameters Param)
{

    double dx = Param.length_coefficient;
    double dt = Param.time_coefficient;
    double dm = Param.mass_coefficient;

    for (uint i=0 ; i < TextFileArea.size(); ++i) {

        if( Param.FlagVelocityText){

            std::stringstream ss;
            ss << iter;
            std::string Iteration = ss.str();
            std::string fileNameStream = Param.WorkFolder +"/results/Text/"+ "Velocity_"  + Number_to_string(i) + "_" + Iteration + ".txt";
            plb::plb_ofstream ofs(fileNameStream.c_str());
            ofs  << std::setprecision(Param.precision) << *multiply(dx/dt,*computeVelocity(lattice,TextFileArea[i])) << std::endl;
        }

       if(Param.FlagVorticityText){

           std::stringstream ss;
           ss << iter;
           std::string Iteration = ss.str();
           std::string fileNameStream = Param.WorkFolder +"/results/Text/" + "Vorticity_"   + Number_to_string(i) + "_" + Iteration + ".txt";
           plb::plb_ofstream ofs(fileNameStream.c_str());
           ofs  << std::setprecision(Param.precision) << *multiply(1.0/dt,*computeVorticity(*computeVelocity(lattice,TextFileArea[i]))) << std::endl;


       }

        if (Param.FlagDensityText){
            std::stringstream ss;
            ss << iter;
            std::string Iteration = ss.str();
            std::string fileNameStream = Param.WorkFolder +"/results/Text/"+ "Density_"   + Number_to_string(i) + "_" + Iteration + ".txt";
            plb::plb_ofstream ofs(fileNameStream.c_str());
            ofs  << std::setprecision(Param.precision) << *multiply(dm,*computeDensity(lattice,TextFileArea[i])) << std::endl;

        }

    }

}


void GeneratePostProcessingArea(std::string Str,std::vector<plb::Box2D>& TextFileArea)
{


    Str.erase( std::remove(Str.begin(), Str.end(), ']'), Str.end());
    Str.erase( std::remove(Str.begin(), Str.end(), '['), Str.end());
    Str.erase( std::remove(Str.begin(), Str.end(), ')'), Str.end());
    Str.erase( std::remove(Str.begin(), Str.end(), '('), Str.end());

    std::vector<std::string> TextFileAreaTmp = split (Str, ";;");
    for (uint i=0 ; i < TextFileAreaTmp.size(); ++i) {
        std::vector<std::string> domain = split (TextFileAreaTmp[i] , ",");
        TextFileArea.push_back(plb::Box2D(std::atoi(domain[0].c_str()), std::atoi(domain[1].c_str()), std::atoi(domain[2].c_str()), std::atoi(domain[3].c_str())));
    }

}

template<class BlockLatticeT>
void LoadCheckPointBlock(BlockLatticeT& lattice,Parameters& Param,plb::plint& StartIter,plb::plint check){

    std::string checkfileExist = Param.FolderNameCheckPoint + "LastCheckPoint.txt" ;


   if (std::fabs(check-1)<1e-14 && file_exist(checkfileExist.c_str())){

           StartIter = CheckPoint_fun(checkfileExist.c_str());
           plb::pcout << " Resumming at " <<  StartIter << " .........." << std::endl ;
           std::string filenametempChPoint = Param.FolderNameCheckPoint +  "Lattice_LastCheckPoint.dat";
           loadBinaryBlock(lattice, filenametempChPoint.c_str());
   }
}



////////////////  save Checkpoint //////////////////////////////////////
template<class BlockLatticeT>
void SaveCheckPointBlock(BlockLatticeT& lattice,Parameters& Param, plb::plint iT){

    plb::pcout << "\n >>> CheckPoint  at   " << iT << "  ...\n" << std::endl;

    std::string LastCheckPointfile =Param.WorkFolder + Param.FolderNameCheckPoint + "LastCheckPoint.txt";
    plb::plb_ofstream ofs1(LastCheckPointfile.c_str());
        ofs1  << iT ;

    std::string filenametempChPoint = Param.WorkFolder + Param.FolderNameCheckPoint +  "Lattice_LastCheckPoint.dat";
        saveBinaryBlock(lattice, filenametempChPoint.c_str());
}

template<class T>
void UpdateSimulationFile(Parameters& Param, std::vector<T> Data) {
    std::string fileNameStream = Param.WorkFolder + "SimulationResults.dat";
    plb::plb_ofstream ofs(fileNameStream.c_str(), std::ios_base::app);
    for (uint i=0;i<Data.size();++i)
    {
            ofs << Data[i] << " " ;
    }
    ofs << std::endl;

}


void CreateSimulationFile(Parameters& Param, std::vector<std::string> DataName){
    std::string fileNameStream = Param.WorkFolder + "SimulationResults.dat";
    plb::plb_ofstream ofs(fileNameStream.c_str());
    for (uint i=0;i<DataName.size();++i)
    {
            ofs << DataName[i] <<" "  ;
    }
    ofs << std::endl;
}



void CreateImageFolders(std::vector<plb::Box2D>& TextFileArea, Parameters& Param)
{
    if(plb::global::mpi().isMainProcessor()){
        for (uint i=0 ; i < TextFileArea.size(); ++i) {

            if( Param.FlagVelocityImage){
                std::string  foldername = Param.WorkFolder + "/results/Image/Velocity_" + Number_to_string(i) ;
                createFolder(foldername.c_str());
            }

           if(Param.FlagVorticityImage){
               std::string  foldername = Param.WorkFolder + "/results/Image/Vorticity_" + Number_to_string(i) ;
               createFolder(foldername.c_str());
           }

            if (Param.FlagDensityImage){
                std::string  foldername = Param.WorkFolder + "/results/Image/Density_" + Number_to_string(i) ;
                createFolder(foldername.c_str());
            }
        }
   }
}


#endif // POSTPROCESSINGFUNCTIONS_H
