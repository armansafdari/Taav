#include "Initial_condition.h"
#include "switch_flags.h"
#include "processorcontroller.h"




using namespace plb;
using namespace plb::descriptors;
using namespace std;

template<typename T, template<typename LBMDescriptor> class Descriptor>
void FunctionBGK_D2Q9(Parameters Param)
{
	
    global::directories().setOutputDir(Param.WorkFolder.c_str());

    T nx = Param.lx;
    T ny = Param.ly;

    MultiBlockLattice2D<double, Descriptor> lattice (
              nx, ny,new BGKdynamics<double, Descriptor>(1.0/Param.tau) );

    
    OnLatticeBoundaryCondition2D<T,Descriptor>*
        boundaryCondition = createLocalBoundaryCondition2D<T,Descriptor>();
        //~ boundaryCondition = createInterpBoundaryCondition2D<T,plb::descriptors::D2Q9Descriptor>();

    #include "boundary_Condition_Field.h"
		
   
    setBoundaryVelocity (
            lattice, lattice.getBoundingBox(),
            InitialConditionVelocity<T>(Param) );
    setBoundaryDensity (
            lattice, lattice.getBoundingBox(),
            InitialConditionDensity<T>(Param) );
    initializeAtEquilibrium (
            lattice, lattice.getBoundingBox(),
            InitialConditionVelocityAndDensity<T>(Param) );		 
 
    lattice.initialize();
    
    pcout << " Param.MaxItertime = " << Param.MaxItertime << endl;
    pcout << " Param.IterFrequency = " << Param.IterFrequency << endl;
    pcout << " Param.FrequencySaveDataImage = " << Param.FrequencySaveDataImage << endl;
    pcout << " Param.FrequencySaveDataText = " << Param.FrequencySaveDataText << endl ;
    pcout << " Param.FrequencySaveDataVTK = " << Param.FrequencySaveDataVTK << endl;
    
    const plint maxT       = Param.MaxItertime;
    const plint logT       = Param.MaxItertime / Param.IterFrequency;
    const plint imSave     = Param.MaxItertime / Param.FrequencySaveDataImage;
    const plint TextSave   = Param.MaxItertime / Param.FrequencySaveDataText;
    const plint vtkSave    = Param.MaxItertime / Param.FrequencySaveDataVTK;

    T previousIterationTime = T();
    bool a=false;
    ControlStart(a);

    // Main loop over time iterations.
    for (plint iT=0; iT < maxT; ++iT) {
        global::timer("mainLoop").restart();

        if (iT%imSave==0) {
            pcout << "Saving Gif ..." << endl;
            writeGif(lattice, iT);
            pcout << endl;
        }

        if (iT%vtkSave==0 && iT>0) {
            pcout << "Saving VTK file ..." << endl;
            writeVTK(lattice, Param, iT);
        }

        if (iT%logT==0) {
            pcout << "step " << iT;
        }

        if (a) {
            pcout << "the program has stopped, now." << iT;
        }

        // Lattice Boltzmann iteration step.
        lattice.collideAndStream();

        if (iT%logT==0) {
            pcout << "; av energy="
                  << setprecision(10) << getStoredAverageEnergy(lattice)
                  << "; av rho="
                  << getStoredAverageDensity(lattice) << endl;
            pcout << "Time spent during previous iteration: "
                  << previousIterationTime << endl;
        }

        previousIterationTime = global::timer("mainLoop").stop();
    }

    delete boundaryCondition;
}



void FunctionBGK_D2Q9_External_Force(Parameters Param)
{
    global::directories().setOutputDir("./tmp/");


    typedef double T;
    
    IncomprFlowParam <T> parameters(
            1e-2,  // uMax
            100.,  // Re
            Param.lx,       // N
            1,        // lx
            1        // ly
    );
    const T logT     = 0.1;
    const T imSave   = 0.2;
    const T vtkSave  = 1.;
    const T maxT     = 0.4;

    writeLogFile(parameters, "2D cavity");

    MultiBlockLattice2D<double, plb::descriptors::ForcedD2Q9Descriptor> lattice (
              parameters.getNx(), parameters.getNy(),
              new BGKdynamics<double, plb::descriptors::ForcedD2Q9Descriptor>(parameters.getOmega()) );

    ////if ( Param.LBMmodel.compare("BGK") == 0,  Param.LBMDiscriptor.compare("D2Q9") == 0) {
    ////T maxT = 10;
    ////pcout << "maxT  = " << maxT << endl;
    ////MultiBlockLattice2D<T, DESCRIPTOR> lattice (
              ////parameters.getNx(), parameters.getNy(),
              ////new BGKdynamics<T,DESCRIPTOR>(parameters.getOmega()) );
    ////}
    
    
    OnLatticeBoundaryCondition2D<T,plb::descriptors::ForcedD2Q9Descriptor>*
        //boundaryCondition = createLocalBoundaryCondition2D<T,DESCRIPTOR>();
        boundaryCondition = createInterpBoundaryCondition2D<T,plb::descriptors::ForcedD2Q9Descriptor>();

    ////cavitySetup(lattice, parameters, *boundaryCondition);

    T previousIterationTime = T();

    // Main loop over time iterations.
    for (plint iT=0; iT*parameters.getDeltaT()<maxT; ++iT) {
        global::timer("mainLoop").restart();

        if (iT%parameters.nStep(imSave)==0 && iT>0) {
            pcout << "Saving Gif ..." << endl;
            writeGif(lattice, iT);
            pcout << endl;
        }

        if (iT%parameters.nStep(vtkSave)==0 && iT>0) {
            pcout << "Saving VTK file ..." << endl;
            writeVTK(lattice, Param, iT);
        }

        if (iT%parameters.nStep(logT)==0) {
            pcout << "step " << iT
                  << "; t=" << iT*parameters.getDeltaT();
        }

        // Lattice Boltzmann iteration step.
        lattice.collideAndStream();

        if (iT%parameters.nStep(logT)==0) {
            pcout << "; av energy="
                  << setprecision(10) << getStoredAverageEnergy(lattice)
                  << "; av rho="
                  << getStoredAverageDensity(lattice) << endl;
            pcout << "Time spent during previous iteration: "
                  << previousIterationTime << endl;
        }

        previousIterationTime = global::timer("mainLoop").stop();
    }

    delete boundaryCondition;
}




void FunctionBGKLES_D2Q9(Parameters Param)
{
    //~ global::directories().setOutputDir("./tmp/");
    global::directories().setOutputDir(Param.WorkFolder.c_str());


    //#define DESCRIPTOR plb::descriptors::D2Q9Descriptor
    
    IncomprFlowParam <T> parameters(
            1e-2,  // uMax
            100.,  // Re
            Param.lx,       // N
            1,        // lx
            1        // ly
    );
    const T logT     = 0.1;
    const T imSave   = 0.2;
    const T vtkSave  = 1.;
    const T maxT     = 0.4;

    
    writeLogFile(parameters, "2D cavity");

    MultiBlockLattice2D<double, plb::descriptors::D2Q9Descriptor> lattice (
              parameters.getNx(), parameters.getNy(),
              new SmagorinskyBGKdynamics<double,plb::descriptors::D2Q9Descriptor>(parameters.getOmega(), Param.cSmago));
    
    
    
    OnLatticeBoundaryCondition2D<T,DESCRIPTOR>*
        //boundaryCondition = createLocalBoundaryCondition2D<T,DESCRIPTOR>();
        boundaryCondition = createInterpBoundaryCondition2D<T,DESCRIPTOR>();

    ////cavitySetup(lattice, parameters, *boundaryCondition);

    T previousIterationTime = T();

    // Main loop over time iterations.
    for (plint iT=0; iT*parameters.getDeltaT()<maxT; ++iT) {
        global::timer("mainLoop").restart();

        if (iT%parameters.nStep(imSave)==0 && iT>0) {
            pcout << "Saving Gif ..." << endl;
            writeGif(lattice, iT);
            pcout << endl;
        }

        if (iT%parameters.nStep(vtkSave)==0 && iT>0) {
            pcout << "Saving VTK file ..." << endl;
            writeVTK(lattice, Param, iT);
        }

        if (iT%parameters.nStep(logT)==0) {
            pcout << "step " << iT
                  << "; t=" << iT*parameters.getDeltaT();
        }

        // Lattice Boltzmann iteration step.
        lattice.collideAndStream();

        if (iT%parameters.nStep(logT)==0) {
            pcout << "; av energy="
                  << setprecision(10) << getStoredAverageEnergy(lattice)
                  << "; av rho="
                  << getStoredAverageDensity(lattice) << endl;
            pcout << "Time spent during previous iteration: "
                  << previousIterationTime << endl;
        }

        previousIterationTime = global::timer("mainLoop").stop();
    }

    delete boundaryCondition;
}


void FunctionBGKLES_D2Q9_External_Force(Parameters Param)
{
    //~ global::directories().setOutputDir("./tmp/");
    global::directories().setOutputDir(Param.WorkFolder.c_str());
    
    
    //#define DESCRIPTOR plb::descriptors::D2Q9Descriptor
    
    IncomprFlowParam <T> parameters(
            1e-2,  // uMax
            100.,  // Re
            Param.lx,       // N
            1,        // lx
            1        // ly
    );
    const T logT     = 0.1;
    const T imSave   = 0.2;
    const T vtkSave  = 1.;
    const T maxT     = 0.4;

    
    //writeLogFile(parameters, "2D cavity");

    MultiBlockLattice2D<double, plb::descriptors::D2Q9Descriptor> lattice (
              parameters.getNx(), parameters.getNy(),
              new SmagorinskyBGKdynamics<double,plb::descriptors::D2Q9Descriptor>(parameters.getOmega(), Param.cSmago));
    
    
    
    OnLatticeBoundaryCondition2D<T,DESCRIPTOR>*
        //boundaryCondition = createLocalBoundaryCondition2D<T,DESCRIPTOR>();
        boundaryCondition = createInterpBoundaryCondition2D<T,DESCRIPTOR>();

    ////cavitySetup(lattice, parameters, *boundaryCondition);

    T previousIterationTime = T();

    // Main loop over time iterations.
    for (plint iT=0; iT*parameters.getDeltaT()<maxT; ++iT) {
        global::timer("mainLoop").restart();

        if (iT%parameters.nStep(imSave)==0 && iT>0) {
            pcout << "Saving Gif ..." << endl;
            writeGif(lattice, iT);
            pcout << endl;
        }

        if (iT%parameters.nStep(vtkSave)==0 && iT>0) {
            pcout << "Saving VTK file ..." << endl;
            writeVTK(lattice, Param, iT);
        }

        if (iT%parameters.nStep(logT)==0) {
            pcout << "step " << iT
                  << "; t=" << iT*parameters.getDeltaT();
        }

        // Lattice Boltzmann iteration step.
        lattice.collideAndStream();

        if (iT%parameters.nStep(logT)==0) {
            pcout << "; av energy="
                  << setprecision(10) << getStoredAverageEnergy(lattice)
                  << "; av rho="
                  << getStoredAverageDensity(lattice) << endl;
            pcout << "Time spent during previous iteration: "
                  << previousIterationTime << endl;
        }

        previousIterationTime = global::timer("mainLoop").stop();
    }

    delete boundaryCondition;
}








