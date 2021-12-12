#ifndef PARAMETES_H
#define PARAMETES_H

#include "palabos2D.h"
#include "palabos2D.hh"
#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <sys/stat.h>

//using namespace plb;
//using namespace std;




std::vector<std::string> split (std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

/////////////  file_exist Function  //////////////////////////

inline bool file_exist (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}
/////////////////////////////////////////////////////////


int CheckPoint_fun( std::string Filename){

std::ifstream in;
in.open(Filename.c_str());
int iT_checkpoint=0;

 while(!in.eof())
    in >> iT_checkpoint ;
in.close();
return iT_checkpoint;

}

struct Parameters
{
    std::string WorkFolder;
    std::string DimensionType;
    plb::plint lx;
    plb::plint ly;
    //plb::plint lz;
    double tau;

    std::string LBMmodel;
    std::string LBMDiscriptor;
    double cSmago;

    bool FlagExternalForce;
    std::string ForceX;
    std::string ForceY;

    bool FlagBoundaryCondition;
    std::string FilenameBoundaryCondition;

    bool FlagPeriodicX;
    std::string BoundaryConditionXP;
    std::string BoundaryConditionVelocityTypeXP;
    std::string BoundaryConditionPressureTypeXP;

    std::string BoundaryConditionXN;
    std::string BoundaryConditionVelocityTypeXN;
    std::string BoundaryConditionPressureTypeXN;

    bool FlagPeriodicY;
    std::string BoundaryConditionYP;
    std::string BoundaryConditionVelocityTypeYP;
    std::string BoundaryConditionPressureTypeYP;

    std::string BoundaryConditionYN;
    std::string BoundaryConditionVelocityTypeYN;
    std::string BoundaryConditionPressureTypeYN;


    bool ObjectsInsideDomainFlag;
    bool LoadfilewallboundaryFlag;
    std::string FilenameObjectsInsideDomain_xml;
    std::string WallBoundary_script;
    std::string NoDynamics_script;



    bool FlagInitialConditionDensity = false;
    bool FlagInitialConditionVelocity = false;
    std::string FilenameInitialConditionDensity ;
    std::string FilenameInitialConditionVelocity;
    std::string velocityX;
    std::string velocityY;
    std::string Density;



    std::string fluidType;
    double fluidDensityPhysical;
    double fluidViscosityPhysical;



    bool MomentumObjectsFlag;
    std::string MomentumObjectsName;
    std::string MomentumObjectsFileName;
    std::string MomentumObjectsScript;


    bool STLfileFlag = false;
    std::string STLfileAddress;
    std::string ShapeType;
    std::string EnforcedxRotation;
    std::string EnforcedyRotation;
    std::string EnforcedzRotation;

    std::string EnforcedxTransition;
    std::string EnforcedyTransition;
    std::string EnforcedzTransition;

    std::string EnforcedxScale;
    std::string EnforcedyScale;
    std::string EnforcedzScale;

    std::string FixedxRotation;
    std::string FixedyRotation;
    std::string FixedzRotation;

    std::string FixedxTransition;
    std::string FixedyTransition;
    std::string FixedzTransition;

    std::string FixedxScale;
    std::string FixedyScale;
    std::string FixedzScale;


    plb::plint Start_simulation;
    plb::plint Finish_simulation;
    plb::plint IterFrequency;

    bool FlagSaveData = true;
    std::string FolderNameSaveData = "results/";
    plb::plint FrequencySaveDataText = 1;
    plb::plint FrequencySaveDataImage = 1;
    plb::plint FrequencySaveDataVTK = 1;

    bool FlagCheckPoint=false;
    std::string FolderNameCheckPoint="checkpoint/";
    plb::plint  FrequencyCheckPoint;

    bool FlagUnitChanger = true;
    double mass_coefficient;
    double length_coefficient;
    double time_coefficient;


    bool FlagTextFile;
    plb::plint precision;
    bool FlagCaptureDomainText;
    std::string CaptureDomainScriptText;
    bool FlagVelocityText;
    bool FlagVorticityText = false;
    bool FlagDensityText;


    bool FlagVtkFile;
    bool FlagCaptureDomainVtk;
    std::string CaptureDomainScriptVtk;
    bool FlagVelocityVtk;
    bool FlagVorticityVtk = false;
    bool FlagDensityVtk;

    bool FlagImageFile;
    bool FlagCaptureDomainImage;
    std::string CaptureDomainScriptImage;
    std::string ColormapVelocityImage;
    std::string ColormapVorticityImage;
    std::string ColormapDensityImage;
    bool FlagVelocityImage;
    bool FlagVorticityImage = false;
    bool FlagDensityImage;

    std::string geometry_fname;

    Parameters(std::string xmlFname)
    {
        plb::XMLreader document(xmlFname);

        /// Environment
        document["projectTree"]["Environment"]["WorkFolder"]["value"].read(WorkFolder);
        //~ document["projectTree"]["Environment"]["Dimension"]["value"].read(DimensionType);
        document["projectTree"]["Environment"]["Parameters"]["X"]["value"].read(lx);
        document["projectTree"]["Environment"]["Parameters"]["Y"]["value"].read(ly);
//        document["projectTree"]["Environment"]["Parameters"]["Z"]["value"].read(lz);
        document["projectTree"]["Environment"]["Parameters"]["Tau"]["value"].read(tau);
        document["projectTree"]["Environment"]["BlockDynamic"]["value"].read(LBMmodel);
        if (LBMmodel=="BGKLES"){
             document["projectTree"]["Environment"]["BlockDynamic"]["cSmago"]["value"].read(cSmago);
        }

        document["projectTree"]["Environment"]["LBMScheme"]["value"].read(LBMDiscriptor);
        document["projectTree"]["Environment"]["LBMScheme"]["ExternalForce"]["value"].read(FlagExternalForce);
        if (FlagExternalForce) {
            document["projectTree"]["Environment"]["LBMScheme"]["ExternalForce"]["X"]["value"].read(ForceX);
            document["projectTree"]["Environment"]["LBMScheme"]["ExternalForce"]["Y"]["value"].read(ForceY);
        }

        document["projectTree"]["Environment"]["BoundaryCondition"]["value"].read(FlagBoundaryCondition);
        if (FlagBoundaryCondition) {
            document["projectTree"]["Environment"]["BoundaryCondition"]["Address"]["value"].read(FilenameBoundaryCondition);
        }
        else {
            document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicX"]["value"].read(FlagPeriodicX);
            if (~FlagPeriodicX) {
                document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicX"]["XP"]["value"].read(BoundaryConditionXP);
                if (BoundaryConditionXP=="velocity"){
                    document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicX"]["XP"]["Velocitytype"]["value"].read(BoundaryConditionVelocityTypeXP);
                }
                if (BoundaryConditionXP=="pressure"){
                    document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicX"]["XP"]["Pressuretype"]["value"].read(BoundaryConditionPressureTypeXP);
                }

                document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicX"]["XN"]["value"].read(BoundaryConditionXN);
                if (BoundaryConditionXN=="velocity"){
                    document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicX"]["XN"]["Velocitytype"]["value"].read(BoundaryConditionVelocityTypeXN);
                }
                if (BoundaryConditionXN=="pressure"){
                    document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicX"]["XN"]["Pressuretype"]["value"].read(BoundaryConditionPressureTypeXN);
                }
            }

            document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicY"]["value"].read(FlagPeriodicY);
             {
                document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicY"]["YP"]["value"].read(BoundaryConditionYP);
                if (BoundaryConditionYP=="velocity"){
                    document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicY"]["YP"]["Velocitytype"]["value"].read(BoundaryConditionVelocityTypeYP);
                }
                if (BoundaryConditionYP=="pressure"){
                    document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicY"]["YP"]["Pressuretype"]["value"].read(BoundaryConditionPressureTypeYP);
                }

                document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicY"]["YN"]["value"].read(BoundaryConditionYN);
                if (BoundaryConditionYN=="velocity"){
                    document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicY"]["YN"]["Velocitytype"]["value"].read(BoundaryConditionVelocityTypeYN);
                }
                if (BoundaryConditionYP=="pressure"){
                    document["projectTree"]["Environment"]["BoundaryCondition"]["PeriodicY"]["YN"]["Pressuretype"]["value"].read(BoundaryConditionPressureTypeYN);
                }
            }

            document["projectTree"]["Environment"]["BoundaryCondition"]["Objectindomain"]["value"].read(ObjectsInsideDomainFlag);
            if (ObjectsInsideDomainFlag) {
                        document["projectTree"]["Environment"]["BoundaryCondition"]["Objectindomain"]["LoadaFile"]["value"].read(LoadfilewallboundaryFlag);
                        if (LoadfilewallboundaryFlag){
                        document["projectTree"]["Environment"]["BoundaryCondition"]["Objectindomain"]["LoadaFile"]["Address"]["value"].read(FilenameObjectsInsideDomain_xml);
                        }
                        else{
                        document["projectTree"]["Environment"]["BoundaryCondition"]["Objectindomain"]["LoadaFile"]["Bouncebackboundary"]["value"].read(WallBoundary_script);
                        document["projectTree"]["Environment"]["BoundaryCondition"]["Objectindomain"]["LoadaFile"]["Nodynamicsarea"]["value"].read(NoDynamics_script);
                        }
            }

        }

        document["projectTree"]["Environment"]["InitialCondition"]["Densityfromafile"]["value"].read(FlagInitialConditionDensity);
        if (FlagInitialConditionDensity) {
            document["projectTree"]["Environment"]["InitialCondition"]["Densityfromafile"]["Address"]["value"].read(FilenameInitialConditionDensity);
        }
        else {
            document["projectTree"]["Environment"]["InitialCondition"]["Densityfromafile"]["Densityscript"]["value"].read(Density);
        }
         document["projectTree"]["Environment"]["InitialCondition"]["Velocityfromafile"]["value"].read(FlagInitialConditionVelocity);
         if (FlagInitialConditionVelocity) {
             document["projectTree"]["Environment"]["InitialCondition"]["Velocityfromafile"]["Address"]["value"].read(FilenameInitialConditionVelocity);
         }
         else {
             document["projectTree"]["Environment"]["InitialCondition"]["Velocityfromafile"]["X"]["value"].read(velocityX);
             document["projectTree"]["Environment"]["InitialCondition"]["Velocityfromafile"]["Y"]["value"].read(velocityY);
         }

        document["projectTree"]["Materials"]["Fluidname"]["value"].read(fluidType);
        document["projectTree"]["Materials"]["Fluidproperties"]["Density"]["value"].read(fluidDensityPhysical);
        document["projectTree"]["Materials"]["Fluidproperties"]["Kinematicviscosity"]["value"].read(fluidViscosityPhysical);



        document["projectTree"]["Geometry"]["MomentumObject"]["value"].read(MomentumObjectsFlag);
        if (MomentumObjectsFlag) {
            document["projectTree"]["Geometry"]["MomentumObject"]["Objectname"]["value"].read(MomentumObjectsName);
            document["projectTree"]["Geometry"]["MomentumObject"]["Objectlocation"]["value"].read(MomentumObjectsScript);
        }

        document["projectTree"]["Geometry"]["Shape"]["value"].read(STLfileFlag);
        document["projectTree"]["Geometry"]["Shape"]["Address"]["value"].read(STLfileAddress);
        document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["value"].read(ShapeType);
        if (ShapeType=="Enforced"){
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Enforced"]["Rotation"]["X"]["value"].read(EnforcedxRotation);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Enforced"]["Rotation"]["Y"]["value"].read(EnforcedyRotation);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Enforced"]["Rotation"]["Z"]["value"].read(EnforcedzRotation);

             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Enforced"]["Transition"]["X"]["value"].read(EnforcedxTransition);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Enforced"]["Transition"]["Y"]["value"].read(EnforcedyTransition);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Enforced"]["Transition"]["Z"]["value"].read(EnforcedzTransition);

             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Enforced"]["Scale"]["X"]["value"].read(EnforcedxScale);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Enforced"]["Scale"]["Y"]["value"].read(EnforcedyScale);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Enforced"]["Scale"]["Z"]["value"].read(EnforcedzScale);
        }
        if (ShapeType=="Fixed"){
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Fixed"]["Rotation"]["X"]["value"].read(FixedxRotation);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Fixed"]["Rotation"]["Y"]["value"].read(FixedyRotation);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Fixed"]["Rotation"]["Z"]["value"].read(FixedzRotation);

             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Fixed"]["Transition"]["X"]["value"].read(FixedxTransition);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Fixed"]["Transition"]["Y"]["value"].read(FixedyTransition);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Fixed"]["Transition"]["Z"]["value"].read(FixedzTransition);

             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Fixed"]["Scale"]["X"]["value"].read(FixedxScale);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Fixed"]["Scale"]["Y"]["value"].read(FixedyScale);
             document["projectTree"]["Geometry"]["Shape"]["Address"]["Behaviortype"]["Fixed"]["Scale"]["Z"]["value"].read(FixedzScale);
        }

        document["projectTree"]["Simulation"]["Time"]["Start"]["value"].read(Start_simulation);
        document["projectTree"]["Simulation"]["Time"]["Finish"]["value"].read(Finish_simulation);
        document["projectTree"]["Simulation"]["Time"]["Outputsfrequency"]["value"].read(IterFrequency);

        document["projectTree"]["Simulation"]["Savedata"]["value"].read(FlagSaveData);
        if (FlagSaveData){
            plb::pcout << "it is a test." << std::endl;
            //~ document["projectTree"]["Simulation"]["Savedata"]["FolderAddress"]["value"].read(FolderNameSaveData);
            document["projectTree"]["Simulation"]["Savedata"]["Textfrequency"]["value"].read(FrequencySaveDataText);
            document["projectTree"]["Simulation"]["Savedata"]["Imagefrequency"]["value"].read(FrequencySaveDataImage);
            document["projectTree"]["Simulation"]["Savedata"]["VTKfrequency"]["value"].read(FrequencySaveDataVTK);
        }

        document["projectTree"]["Simulation"]["Checkpoint"]["value"].read(FlagCheckPoint);
        if (FlagCheckPoint){
            //~ document["projectTree"]["Simulation"]["Checkpoint"]["FolderAddress"]["value"].read(FolderNameCheckPoint);
            document["projectTree"]["Simulation"]["Checkpoint"]["Checkpointfrequency"]["value"].read(FrequencyCheckPoint);
        }

        //~ document["projectTree"]["PostProcessing"]["Units"]["value"].read(FlagUnitChanger);
        document["projectTree"]["PostProcessing"]["Units"]["Masscoefficient"]["value"].read(mass_coefficient);
        document["projectTree"]["PostProcessing"]["Units"]["Lengthcoefficient"]["value"].read(length_coefficient);
        document["projectTree"]["PostProcessing"]["Units"]["Timecoefficient"]["value"].read(time_coefficient);


        document["projectTree"]["PostProcessing"]["Textfile"]["value"].read(FlagTextFile);
        if (FlagTextFile){
            document["projectTree"]["PostProcessing"]["Textfile"]["Customdomains"]["value"].read(FlagCaptureDomainText);
            if (FlagCaptureDomainText){
                document["projectTree"]["PostProcessing"]["Textfile"]["Customdomains"]["Domain"]["value"].read(CaptureDomainScriptText);
            }
            document["projectTree"]["PostProcessing"]["Textfile"]["Velocity"]["value"].read(FlagVelocityText);
            document["projectTree"]["PostProcessing"]["Textfile"]["Precision"]["value"].read(precision);
            //~ document["projectTree"]["PostProcessing"]["Textfile"]["Vorticity"]["value"].read(FlagVorticityText);
            document["projectTree"]["PostProcessing"]["Textfile"]["Density"]["value"].read(FlagDensityText);
        }

        document["projectTree"]["PostProcessing"]["VTKfile"]["value"].read(FlagVtkFile);
        if (FlagVtkFile){
            document["projectTree"]["PostProcessing"]["VTKfile"]["Customdomains"]["value"].read(FlagCaptureDomainVtk);
            if (FlagCaptureDomainVtk){
                document["projectTree"]["PostProcessing"]["VTKfile"]["Customdomains"]["Domain"]["value"].read(CaptureDomainScriptVtk);
            }
            document["projectTree"]["PostProcessing"]["VTKfile"]["Velocity"]["value"].read(FlagVelocityVtk);
            //~ document["projectTree"]["PostProcessing"]["VTKfile"]["Vorticity"]["value"].read(FlagVorticityVtk);
            document["projectTree"]["PostProcessing"]["VTKfile"]["Density"]["value"].read(FlagDensityVtk);
        }

        document["projectTree"]["PostProcessing"]["Imagefile"]["value"].read(FlagImageFile);
        if (FlagImageFile){
            document["projectTree"]["PostProcessing"]["Imagefile"]["Customdomains"]["value"].read(FlagCaptureDomainImage);
            if (FlagCaptureDomainImage){
                document["projectTree"]["PostProcessing"]["Imagefile"]["Customdomains"]["Domain"]["value"].read(CaptureDomainScriptImage);
            }
            document["projectTree"]["PostProcessing"]["Imagefile"]["Velocity"]["value"].read(FlagVelocityImage);
            document["projectTree"]["PostProcessing"]["Imagefile"]["Velocity"]["Colortype"]["value"].read(ColormapVelocityImage);
            //~ document["projectTree"]["PostProcessing"]["Imagefile"]["Vorticity"]["value"].read(FlagVorticityImage);
            //~ document["projectTree"]["PostProcessing"]["Imagefile"]["Vorticity"]["Colortype"]["value"].read(ColormapVorticityImage);
            document["projectTree"]["PostProcessing"]["Imagefile"]["Density"]["value"].read(FlagDensityImage);
            document["projectTree"]["PostProcessing"]["Imagefile"]["Density"]["Colortype"]["value"].read(ColormapDensityImage);
        }

       print_parameters();
    }


    void print_parameters()
    {

    plb::pcout << "   WorkFolder = " <<   WorkFolder << std::endl;
    plb::pcout << "   DimensionType = " <<  DimensionType << std::endl;
    plb::pcout << "   lx = " << lx << std::endl;
    plb::pcout << "   ly = " << ly << std::endl;
    //plb::pcout << "  lz = " << lz << std::endl;
    plb::pcout << "   tau = " <<  tau << std::endl;

    plb::pcout << "   LBMmodel = " <<  LBMmodel << std::endl;
    plb::pcout << "   LBMDiscriptor = " <<  LBMDiscriptor << std::endl;

    if (LBMmodel=="LBGK"){
        plb::pcout << "   cSmago = " <<  cSmago << std::endl;
    }
    plb::pcout << "   FlagExternal = " <<  FlagExternalForce << std::endl;


    if (FlagExternalForce) {
        plb::pcout << "   ForceX = " <<  ForceX << std::endl;
        plb::pcout << "   ForceY = " <<  ForceY << std::endl;
    }


    plb::pcout << "   FlagBoundaryCondition = " <<  FlagBoundaryCondition << std::endl;

    if (FlagBoundaryCondition) {
        plb::pcout << "   FilenameBoundaryCondition = " <<  FilenameBoundaryCondition << std::endl;
        }
        else {
            plb::pcout << "   FlagPeriodicX = " <<  FlagPeriodicX << std::endl;
            if (FlagPeriodicX==0) {
                plb::pcout << "   BoundaryConditionXP = " <<  BoundaryConditionXP << std::endl;
                if (BoundaryConditionXP=="velocity"){
                    plb::pcout << "   BoundaryConditionVelocityTypeXP = " <<  BoundaryConditionVelocityTypeXP << std::endl;
                }
                if (BoundaryConditionXP=="pressure"){
                    plb::pcout << "   BoundaryConditionPressureTypeXP = " <<  BoundaryConditionPressureTypeXP << std::endl;
                }

                plb::pcout << "   BoundaryConditionXN = " <<  BoundaryConditionXN << std::endl;
                if (BoundaryConditionXN=="velocity"){
                    plb::pcout << "   BoundaryConditionVelocityTypeXN = " <<  BoundaryConditionVelocityTypeXN << std::endl;

                }
                if (BoundaryConditionXP=="pressure"){
                    plb::pcout << "   BoundaryConditionPressureTypeXN = " <<  BoundaryConditionPressureTypeXN << std::endl;
                }
            }


            plb::pcout << "   FlagPeriodicY = " <<  FlagPeriodicY << std::endl;
            if (FlagPeriodicY==0) {
                plb::pcout << "   BoundaryConditionTypeYP = " <<  BoundaryConditionYP << std::endl;
            if (BoundaryConditionYP=="velocity"){
                plb::pcout << "   BoundaryConditionVelocityTypeYP = " <<  BoundaryConditionVelocityTypeYP << std::endl;

            }
            if (BoundaryConditionYP=="pressure"){
                plb::pcout << "   BoundaryConditionPressureTypeYP = " <<  BoundaryConditionPressureTypeYP << std::endl;
            }


            plb::pcout << "   BoundaryConditionTypeYN = " <<  BoundaryConditionYN << std::endl;
            if (BoundaryConditionYN=="velocity"){
                plb::pcout << "   BoundaryConditionVelocityTypeYN = " <<  BoundaryConditionVelocityTypeYN << std::endl;

            }
                if (BoundaryConditionYP=="pressure"){
                    plb::pcout << "   BoundaryConditionPressureTypeYN = " <<  BoundaryConditionPressureTypeYN << std::endl;
                }
            }


            plb::pcout << "   ObjectsInsideDomainFlag = " <<  ObjectsInsideDomainFlag << std::endl;
            if (ObjectsInsideDomainFlag) {
                plb::pcout << "   LoadfilewallboundaryFlag = " <<  LoadfilewallboundaryFlag  << std::endl ;
                if (LoadfilewallboundaryFlag) {
                    plb::pcout << "   FilenamewallBoundaryCondition_xml  = " <<  FilenameObjectsInsideDomain_xml << std::endl;
                }
                else {
                        plb::pcout << "   WallBoundary_script = " <<  WallBoundary_script << std::endl;
                        plb::pcout << "   NoDynamics_script = " <<  NoDynamics_script << std::endl;
                    }
            }
    }



    plb::pcout << "   FlagInitialConditionDensity = " <<  FlagInitialConditionDensity << std::endl;
    if (FlagInitialConditionDensity) {
        plb::pcout << "   FilenameInitialConditionDensity = " <<  FilenameInitialConditionDensity << std::endl;
    }
    else {
        plb::pcout << "   Density = " <<  Density << std::endl;
    }

    plb::pcout << "   FlagInitialConditionVelocity = " <<  FlagInitialConditionVelocity << std::endl;
    if (FlagInitialConditionVelocity) {
        plb::pcout << "   FilenameInitialConditionVelocity = " <<  FilenameInitialConditionVelocity << std::endl;
    }
    else {
        plb::pcout << "   velocityX = " <<  velocityX << std::endl;
        plb::pcout << "   velocityY = " <<  velocityY << std::endl;
    }


    plb::pcout << "   fluidType = " <<  fluidType << std::endl;
    plb::pcout << "   fluidDensityPhysical = " <<  fluidDensityPhysical << std::endl;
    plb::pcout << "   fluidViscosityPhysical = " <<  fluidViscosityPhysical << std::endl;

    plb::pcout << "   MomentumObjectsFlag = " <<  MomentumObjectsFlag << std::endl;
    if (MomentumObjectsFlag) {
    plb::pcout << "   MomentumObjectsName = " <<  MomentumObjectsName << std::endl;
    plb::pcout << "   MomentumObjectsFileName = " <<  MomentumObjectsFileName << std::endl;
    plb::pcout << "   MomentumObjectsScript = " <<  MomentumObjectsScript << std::endl;
    }


    plb::pcout << "   STLfileFlag = " <<  STLfileFlag << std::endl;
    if (STLfileFlag){
    plb::pcout << "   STLfileAddress = " <<  STLfileAddress << std::endl;
    plb::pcout << "   ShapeType = " <<  ShapeType << std::endl;

    if (ShapeType=="Enforced"){
        plb::pcout << "   EnforcedxRotation = " <<  EnforcedxRotation << std::endl;
        plb::pcout << "   EnforcedyRotation = " <<  EnforcedyRotation << std::endl;
        plb::pcout << "   EnforcedzRotation = " <<  EnforcedzRotation << std::endl;

        plb::pcout << "   EnforcedxTransition = " <<  EnforcedxTransition << std::endl;
        plb::pcout << "   EnforcedyTransition = " <<  EnforcedyTransition << std::endl;
        plb::pcout << "   EnforcedzTransition = " <<  EnforcedzTransition << std::endl;

        plb::pcout << "   EnforcedxScale = " <<  EnforcedxScale << std::endl;
        plb::pcout << "   EnforcedyScale = " <<  EnforcedyScale << std::endl;
        plb::pcout << "   EnforcedzScale = " <<  EnforcedzScale << std::endl;
    }
    if (ShapeType=="Fixed"){

        plb::pcout << "   FixedxRotation = " <<  FixedxRotation << std::endl;
        plb::pcout << "   FixedyRotation = " <<  FixedyRotation << std::endl;
        plb::pcout << "   FixedzRotation = " <<  FixedzRotation << std::endl;

        plb::pcout << "   FixedxTransition = " <<  FixedxTransition << std::endl;
        plb::pcout << "   FixedyTransition = " <<  FixedyTransition << std::endl;
        plb::pcout << "   FixedzTransition = " <<  FixedzTransition << std::endl;

        plb::pcout << "   FixedxScale = " <<  FixedxScale << std::endl;
        plb::pcout << "   FixedyScale = " <<  FixedyScale << std::endl;
        plb::pcout << "   FixedzScale = " <<  FixedzScale << std::endl;
    }
    }



    plb::pcout << "   Start_simulation = " << Start_simulation << std::endl;
    plb::pcout << "   Start_simulation = " << Finish_simulation << std::endl;
    plb::pcout << "   IterFrequency = " << IterFrequency << std::endl;

    plb::pcout << "   FlagSaveData = " <<  FlagSaveData << std::endl;
    if (FlagSaveData){

        plb::pcout << "   FolderNameSaveData = " <<  FolderNameSaveData << std::endl;
        plb::pcout << "   FrequencySaveDataText = " << FrequencySaveDataText << std::endl;
        plb::pcout << "   FrequencySaveDataImage = " << FrequencySaveDataImage << std::endl;
        plb::pcout << "   FrequencySaveDataVTK = " << FrequencySaveDataVTK << std::endl;
    }


    plb::pcout << "   FlagCheckPoint = " <<  FlagCheckPoint << std::endl;
    if (FlagCheckPoint){

        plb::pcout << "   FolderNameCheckPoint = " <<  FolderNameCheckPoint << std::endl;
        plb::pcout << "   FrequencyCheckPoint = " <<  FrequencyCheckPoint << std::endl;
    }

    plb::pcout << "   FlagUnitChanger = " <<  FlagUnitChanger << std::endl;
    if (FlagUnitChanger){
        plb::pcout << "   mass_coefficient = " <<  mass_coefficient << std::endl;
        plb::pcout << "   length_coefficient = " <<  length_coefficient << std::endl;
        plb::pcout << "   time_coefficient = " <<  time_coefficient << std::endl;
    }

    plb::pcout << "   FlagTextFile = " <<  FlagTextFile << std::endl;
    if (FlagTextFile){
        plb::pcout << "   precision = " <<  precision << std::endl;
        plb::pcout << "   FlagCaptureDomainText = " <<  FlagCaptureDomainText<< std::endl;
        if (FlagTextFile){
            plb::pcout << "   CaptureDomainScriptText = " <<  CaptureDomainScriptText << std::endl;
        }
        plb::pcout << "   FlagVelocityText = " <<  FlagVelocityText << std::endl;
        plb::pcout << "   FlagVorticityText = " <<  FlagVorticityText << std::endl;
        plb::pcout << "   FlagDensityText = " <<  FlagDensityText << std::endl;
    }

    plb::pcout << "   FlagVtkFile = " <<  FlagVtkFile << std::endl;
    if (FlagVtkFile){
        plb::pcout << "   FlagCaptureDomainVtk = " <<  FlagCaptureDomainVtk<< std::endl;
        if (FlagTextFile){
            plb::pcout << "   CaptureDomainScriptVtk = " <<  CaptureDomainScriptVtk << std::endl;
        }
        plb::pcout << "   FlagVelocityVtk = " <<  FlagVelocityVtk << std::endl;
        plb::pcout << "   FlagVorticityVtk = " <<  FlagVorticityVtk << std::endl;
        plb::pcout << "   FlagDensityVtk = " <<  FlagDensityVtk << std::endl;
    }

    plb::pcout << "   FlagImageFile = " <<  FlagImageFile << std::endl;
    if (FlagImageFile){
        plb::pcout << "   FlagCaptureDomainImage = " <<  FlagCaptureDomainImage << std::endl;
        if (FlagTextFile){
            plb::pcout << "   CaptureDomainScriptImage = " <<  CaptureDomainScriptImage << std::endl;
        }
        plb::pcout << "   FlagVelocityImage = " <<  FlagVelocityImage << std::endl;
        plb::pcout << "   FlagVorticityImage = " <<  FlagVorticityImage << std::endl;
        plb::pcout << "   FlagDensityImage = " <<  FlagDensityImage << std::endl;
    }
}
};




#endif // PARAMETES_H
