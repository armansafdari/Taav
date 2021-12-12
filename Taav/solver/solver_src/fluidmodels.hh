#ifndef FLUIDMODELS_HH
#define FLUIDMODELS_HH
#include <unistd.h>

#include "initial_condition.h"
#include "switch_flags.h"
#include "processorcontroller.h"
#include "boundary_condition.h"
#include "bounceback_boundary.h"
#include "postprocessingfunctions.h"

#define DESCRIPTOR descriptors::D2Q9Descriptor


//template<typename T, template<typename LBMDescriptor> class Descriptor>
template<typename T, template<typename LBMDescriptor> class Descriptor,class BlockLatticeT>
void Function_D2Q9(BlockLatticeT& lattice, Parameters& Param)
{

    plb::global::directories().setOutputDir(Param.WorkFolder.c_str());

    T nx = Param.lx;
    T ny = Param.ly;

//    plb::MultiBlockLattice2D<double, Descriptor> lattice (
//              nx, ny,new plb::BGKdynamics<double, Descriptor>(1.0/Param.tau) );

    plb::OnLatticeBoundaryCondition2D<T,Descriptor>*
        boundaryCondition = plb::createLocalBoundaryCondition2D<T,Descriptor>();
        //~ boundaryCondition = createInterpBoundaryCondition2D<T,Descriptor>();

    BoundaryConditonFunction(lattice,boundaryCondition,Param);

    WallInsideDomainFunction<T,Descriptor>(lattice,Param);

    std::vector<MomentumExchangeObjects<T, Descriptor>* > Objects;
    if (Param.MomentumObjectsFlag){
        MomentumExchangeInsideDomain<T,Descriptor>(lattice,Param, Objects);
    }
    InitialConditionFunction<T,Descriptor>(lattice,Param);

    std::vector<plb::Box2D> TextFileArea;
    std::vector<plb::Box2D> VtkFileArea;
    std::vector<plb::Box2D> ImageFileArea;

    createFolder(Param.WorkFolder + Param.FolderNameSaveData );


    if(Param.FlagTextFile && Param.FlagSaveData){

        createFolder(Param.WorkFolder + Param.FolderNameSaveData  + "Text/");

        if (Param.FlagCaptureDomainText)
            GeneratePostProcessingArea(Param.CaptureDomainScriptText.c_str(), TextFileArea);
        if (TextFileArea.empty())
            TextFileArea.push_back(lattice.getBoundingBox());
        else
            TextFileArea.push_back(lattice.getBoundingBox());
    }

    if(Param.FlagVtkFile && Param.FlagSaveData){
        createFolder(Param.WorkFolder + Param.FolderNameSaveData  + "Vtk/");

        if (Param.FlagCaptureDomainVtk){
            GeneratePostProcessingArea(Param.CaptureDomainScriptVtk.c_str(), VtkFileArea);
        if (VtkFileArea.empty())
            VtkFileArea.push_back(lattice.getBoundingBox());
        }
        else
            VtkFileArea.push_back(lattice.getBoundingBox());
    }

    if(Param.FlagImageFile &&Param.FlagSaveData){
        createFolder(Param.WorkFolder + Param.FolderNameSaveData + "Image/");

        if (Param.FlagCaptureDomainImage){
            GeneratePostProcessingArea(Param.CaptureDomainScriptImage.c_str(), ImageFileArea);
            if (ImageFileArea.empty())
                ImageFileArea.push_back(lattice.getBoundingBox());
        }
        else
            ImageFileArea.push_back(lattice.getBoundingBox());

        CreateImageFolders(ImageFileArea, Param);
    }


    plb::plint startiteration = 0;
    plb::plint NumberOfRacks = 0 ;
    plb::plint checkload = ControlStop(NumberOfRacks,Param.WorkFolder);
    createFolder(Param.WorkFolder  + Param.FolderNameCheckPoint);

    if(NumberOfRacks==plb::global::mpi().getSize()){
        LoadCheckPointBlock(lattice,Param,startiteration,checkload);
    }

    plb::plint LengthItertime = static_cast<plb::plint> (std::abs( Param.Start_simulation- Param.Finish_simulation));
//    plb::pcout << " Param.Itertime = " << LengthItertime<< std::endl;
//    plb::pcout << " Param.IterFrequency = " << Param.IterFrequency << std::endl;
//    plb::pcout << " Param.FrequencySaveDataImage = " << Param.FrequencySaveDataImage << std::endl;
//    plb::pcout << " Param.FrequencySaveDataText = " << Param.FrequencySaveDataText << std::endl ;
//    plb::pcout << " Param.FrequencySaveDataVTK = " << Param.FrequencySaveDataVTK << std::endl;

    const plb::plint logT       = LengthItertime / Param.IterFrequency;
    const plb::plint imSave     = LengthItertime / Param.FrequencySaveDataImage;
    const plb::plint TextSave   = LengthItertime / Param.FrequencySaveDataText;
    const plb::plint vtkSave    = LengthItertime / Param.FrequencySaveDataVTK;
    const plb::plint checkPoint = LengthItertime / Param.FrequencyCheckPoint;
    const plb::plint simulationPercentage = LengthItertime / 20 ;
    const plb::plint DatarecordItr = LengthItertime / 100 ;




    plb::plint interuptCheck    = 1000;
    plb::plint CheckFlag = 0;


    std::vector<std::string> DataName;
    DataName.push_back(":: Time");
    DataName.push_back("Energy");
    DataName.push_back("Stability");

    CreateSimulationFile(Param,DataName);

    T previousIterationTime = T();

    ControlStart(static_cast<plb::plint>(0),Param.WorkFolder);
    // Main loop over time iterations.
    for (plb::plint iT=startiteration; iT < Param.Finish_simulation+1; ++iT) {
        plb::global::timer("mainLoop").restart();

        if (Param.FlagImageFile && Param.FlagSaveData) {
            if (iT%imSave==0 ){
            plb::pcout << "Saving Image ..." << std::endl;
            writeImages(lattice, ImageFileArea, iT, Param);
            plb::pcout << std::endl;
            }
        }

        if ( Param.FlagVtkFile && Param.FlagSaveData) {
            if (iT%vtkSave==0){
            plb::pcout << "Saving VTK file ..." << std::endl;
            writeVTK(lattice, VtkFileArea, iT, Param);
            }
        }

        if (Param.FlagTextFile && Param.FlagSaveData) {
            if(iT%TextSave==0 ){
            plb::pcout << "Saving Text file ..." << std::endl;
            writetxt(lattice, TextFileArea, iT, Param);
            }
        }

        if (iT%logT==0) {
            plb::pcout << "step " << iT;
        }

        if (iT%interuptCheck==0 && iT>0 && CheckFlag > 5) {

            plb::plint check = ControlStop(NumberOfRacks,Param.WorkFolder);

            if(check==1){
                    SaveCheckPointBlock(lattice,Param, iT);
                    plb::pcout << " \n  the program has stopped." << iT;
                    exit(-1);
                  break;
            }
            if(check==2){
                    plb::pcout << " \n  the program has stopped." << iT;
                    exit(-1);
                  break;
            }
        }

        // Lattice Boltzmann iteration step.
        lattice.collideAndStream();

        if (iT%logT==0) {
            plb::pcout << "; av energy="
                  << std::setprecision(10) << getStoredAverageEnergy(lattice)
                  << "; av rho="
                  << getStoredAverageDensity(lattice) << std::endl;
            plb::pcout << "Time spent during previous iteration: "
                  << previousIterationTime   <<std::endl;

            if (Param.MomentumObjectsFlag){
            for(plb::plint l=0; l< Objects.size();++l){
                plb::Array<T,Descriptor<T>::d> forceIds = Objects[l]->GetForces();
                plb::pcout << " drag ( " <<Objects[l]->GetName() <<  " )=" << lattice.getInternalStatistics().getSum(forceIds[0])
                           << "; lift ( " <<Objects[l]->GetName() <<  " )=" << lattice.getInternalStatistics().getSum(forceIds[1])
                           << std::endl;
            }

            }
            if (std::isnan(static_cast<T> (computeAverage(*computeDensity(lattice))))) {
                   exit(-1);;
             }
        }


        if(iT%DatarecordItr==0){

            std::vector<T> Data;
            Data.push_back(iT);
            Data.push_back(getStoredAverageEnergy(lattice));
            Data.push_back(getStoredAverageDensity(lattice));

            UpdateSimulationFile(Param,Data);
        }

        previousIterationTime = plb::global::timer("mainLoop").stop();

        if (CheckFlag <= 5 ){
            if (CheckFlag < 5)
                CheckFlag++;
            else {
//            ProcesstakesperItr(previousIterationTime, Param.WorkFolder);
            interuptCheck = static_cast<T>( 10 / static_cast<T>(previousIterationTime) + 1);
        //~ plb::pcout << "interuptCheck" << interuptCheck << std::endl;
            ProcesstakesperItr((previousIterationTime*logT+1)*1000, Param.WorkFolder);
            CheckFlag=10;
           }

        }


        if (iT%checkPoint==0 && Param.FlagCheckPoint && iT>0) {
            SaveCheckPointBlock(lattice,Param, iT);
            plb::pcout << std::endl;
        }
        if (iT%simulationPercentage==0 && iT >0) {
            T  SimulationPercent = static_cast<T> (static_cast<T> (iT) / static_cast<T> (LengthItertime) * 100);
            plb::pcout << "Simulation percentage = " << SimulationPercent  <<  "%" << std::endl;
            Updatepercentage(SimulationPercent,Param.WorkFolder);
        }
    }

    delete boundaryCondition;
    ControlStart(static_cast<plb::plint>(1),Param.WorkFolder);
    Updatepercentage(100,Param.WorkFolder);
}


#endif // FLUIDMODELS_HH
