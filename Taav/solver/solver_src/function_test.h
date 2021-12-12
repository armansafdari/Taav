#ifndef FUNCTION_TEST_H
#define FUNCTION_TEST_H




//#include "palabos2D.h"
//#include "palabos2D.hh"
//#include <cstdlib>
//#include <iostream>

//using namespace plb;
//using namespace std;

//// Use double-precision arithmetics
//typedef double T;
//// Use a grid which additionally to the f's stores two variables for
////   the external force term.
//#define DESCRIPTOR descriptors::ForcedShanChenD2Q9Descriptor

///// Initial condition: heavy fluid on top, light fluid on bottom.
///** This functional is going to be used as an argument to the function "applyIndexed",
// *  to setup the initial condition. For efficiency reasons, this approach should
// *  always be preferred over explicit space loops in end-user codes.
// */
//template<typename T, template<typename U> class Descriptor>
//class TwoLayerInitializer : public OneCellIndexedWithRandFunctional2D<T,Descriptor> {
//public:
//    TwoLayerInitializer(plint ny_, bool topLayer_)
//        : ny(ny_),
//          topLayer(topLayer_)
//    { }
//    TwoLayerInitializer<T,Descriptor>* clone() const {
//        return new TwoLayerInitializer<T,Descriptor>(*this);
//    }
//    virtual void execute(plint iX, plint iY, T rand_val, Cell<T,Descriptor>& cell) const {
//        T densityFluctuations = 1.e-2;
//        T almostNoFluid       = 1.e-4;
//        Array<T,2> zeroVelocity (0.,0.);

//        T rho = (T)1;
//        // Add a random perturbation to the initial condition to instantiate the
//        //   instability.
//        if ( (topLayer && iY>ny/2) || (!topLayer && iY <= ny/2) ) {
//            rho += rand_val * densityFluctuations;
//        }
//        else {
//            rho = almostNoFluid;
//        }

//        iniCellAtEquilibrium(cell, rho, zeroVelocity);
//    }
//private:
//    plint ny;
//    bool topLayer;
//};


//void rayleighTaylorSetup( MultiBlockLattice2D<T, DESCRIPTOR>& heavyFluid,
//                          MultiBlockLattice2D<T, DESCRIPTOR>& lightFluid,
//                          T rho0, T rho1,
//                          T force )
//{
//    // The setup is: periodicity along horizontal direction, bounce-back on top
//    // and bottom. The upper half is initially filled with fluid 1 + random noise,
//    // and the lower half with fluid 2. Only fluid 1 experiences a forces,
//    // directed downwards.
//    plint nx = heavyFluid.getNx();
//    plint ny = heavyFluid.getNy();

//    // Bounce-back on bottom wall (where the light fluid is, initially).
//    defineDynamics(heavyFluid, Box2D(0,nx-1, 0,0), new BounceBack<T, DESCRIPTOR>(rho0) );
//    defineDynamics(lightFluid, Box2D(0,nx-1, 0,0), new BounceBack<T, DESCRIPTOR>(rho1) );
//    // Bounce-back on top wall (where the heavy fluid is, initially).
//    defineDynamics(heavyFluid, Box2D(0,nx-1, ny-1,ny-1), new BounceBack<T, DESCRIPTOR>(rho1) );
//    defineDynamics(lightFluid, Box2D(0,nx-1, ny-1,ny-1), new BounceBack<T, DESCRIPTOR>(rho0) );

//    // Initialize top layer.
//    applyIndexed(heavyFluid, Box2D(0, nx-1, 0, ny-1),
//                 new TwoLayerInitializer<T,DESCRIPTOR>(ny, true) );
//    // Initialize bottom layer.
//    applyIndexed(lightFluid, Box2D(0, nx-1, 0, ny-1),
//                 new TwoLayerInitializer<T,DESCRIPTOR>(ny, false) );

//    // Let's have gravity acting on the heavy fluid only. This represents a situation
//    //   where the molecular mass of the light species is very small, and thus the
//    //   action of gravity on this species is negligible.
//    setExternalVector(heavyFluid, heavyFluid.getBoundingBox(),
//                      DESCRIPTOR<T>::ExternalField::forceBeginsAt, Array<T,2>(0.,-force));
//    setExternalVector(lightFluid, lightFluid.getBoundingBox(),
//                      DESCRIPTOR<T>::ExternalField::forceBeginsAt, Array<T,2>(0.,0.));

//    lightFluid.initialize();
//    heavyFluid.initialize();
//}

//void writeGifs(MultiBlockLattice2D<T, DESCRIPTOR>& heavyFluid,
//               MultiBlockLattice2D<T, DESCRIPTOR>& lightFluid, plint iT)
//{
//    ImageWriter<T> imageWriter("leeloo.map");
//    imageWriter.writeScaledGif(createFileName("rho_heavy_", iT, 6),
//                               *computeDensity(heavyFluid));
//    imageWriter.writeScaledGif(createFileName("rho_light_", iT, 6),
//                               *computeDensity(lightFluid));
//}

//void testFunction()
//{

//    global::directories().setOutputDir("./tmp/");
//    srand(global::mpi().getRank());

//    const T omega1 = 1.0;
//    const T omega2 = 1.0;
//    const plint nx   = 1200;
//    const plint ny   = 400;
//    const T G      = 1.2;
//    T force        = 0.15/(T)ny;
//    const plint maxIter  = 16000;
//    const plint saveIter = 100;
//    const plint statIter = 10;

//    // Use regularized BGK dynamics to improve numerical stability (but note that
//    //   BGK dynamics works well too).
//    MultiBlockLattice2D<T, DESCRIPTOR> heavyFluid (
//            nx,ny, new ExternalMomentRegularizedBGKdynamics<T, DESCRIPTOR>(omega1) );
//    MultiBlockLattice2D<T, DESCRIPTOR> lightFluid (
//            nx,ny, new ExternalMomentRegularizedBGKdynamics<T, DESCRIPTOR>(omega2) );

//    // Make x-direction periodic.
//    heavyFluid.periodicity().toggle(0,true);
//    lightFluid.periodicity().toggle(0,true);

//    T rho1 = 1.; // Fictitious density experienced by the partner fluid on a Bounce-Back node.
//    T rho0 = 0.; // Fictitious density experienced by the partner fluid on a Bounce-Back node.

//    // Store a pointer to all lattices (two in the present application) in a vector to
//    //   create the Shan/Chen coupling therm. The heavy fluid being at the first place
//    //   in the vector, the coupling term is going to be executed at the end of the call
//    //   to collideAndStream() or stream() for the heavy fluid.
//    vector<MultiBlockLattice2D<T, DESCRIPTOR>* > blockLattices;
//    blockLattices.push_back(&heavyFluid);
//    blockLattices.push_back(&lightFluid);

//    // The argument "constOmegaValues" to the Shan/Chen processor is optional,
//    //   and is used for efficiency reasons only. It tells the data processor
//    //   that the relaxation times are constant, and that their inverse must be
//    //   computed only once.
//    std::vector<T> constOmegaValues;
//    constOmegaValues.push_back(omega1);
//    constOmegaValues.push_back(omega2);
//    plint processorLevel = 1;
//    integrateProcessingFunctional (
//            new ShanChenMultiComponentProcessor2D<T,DESCRIPTOR>(G,constOmegaValues),
//            Box2D(0,nx-1,1,ny-2),
//            blockLattices,
//            processorLevel );

//    rayleighTaylorSetup(heavyFluid, lightFluid, rho0, rho1, force);

//    pcout << "Starting simulation" << endl;
//    // Main loop over time iterations.
//    for (plint iT=0; iT<maxIter; ++iT) {
//        if (iT%saveIter==0) {
//            writeGifs(heavyFluid, lightFluid, iT);
//        }

//        // Time iteration for the light fluid.
//        lightFluid.collideAndStream();
//        // Time iteration for the heavy fluid must come after the light fluid,
//        //   because the coupling is executed here. You should understand this as follows.
//        //   The effect of the coupling is to compute the interaction force between
//        //   species, and to precompute density and momentum for each species. This must
//        //   be executed *before* collide-and-streaming the fluids, because the collision
//        //   step needs to access all these values. In the present case, it is done after
//        //   both collide-and-stream step, which means, before the collide-and-stream of
//        //   the next iteration (it's the same if you are before or after; the important
//        //   point is not to be between the two collide-and-streams of the light and heavy
//        //   fluid. As for the initial condition, the coupling is initially performed once
//        //   during the function call to heavyFluid.initialize().
//        heavyFluid.collideAndStream();

//        if (iT%statIter==0) {
//            pcout << "Average density fluid one = "
//                  << getStoredAverageDensity<T>(heavyFluid);
//            pcout << ", average density fluid two = "
//                  << getStoredAverageDensity<T>(lightFluid) << endl;
//        }
//    }
//}































#include "palabos2D.h"
#include "palabos2D.hh"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "data_initialization.h"

using namespace plb;
using namespace plb::descriptors;
using namespace std;

typedef double T;
#define DESCRIPTOR D2Q9Descriptor

// constants specified in the article
T r0 = 0.1;
T omega_e = 299.5286;
T Re = 625.0;

// centers of the dipoles
Array<T,2> p1(.0, .1);
Array<T,2> p2(.0,-.1);


















T r1Sqr(T x, T y){
    return util::sqr(x-p1[0]) + util::sqr(y-p1[1]);
}

T r2Sqr(T x, T y){
    return util::sqr(x-p2[0]) + util::sqr(y-p2[1]);
}

/// computation of the horizontal velocity of the dipole
T computeU0(plint iX, plint iY, IncomprFlowParam<T> params){
    // convert and translate x to the [-1,1]x[-1,1] square
    T x = ((T)iX * params.getDeltaX() - 1.);
    T y = ((T)iY * params.getDeltaX() - 1.);
    PLB_ASSERT(-1.0 <= x && x <= 1.0);
    PLB_ASSERT(-1.0 <= y && y <= 1.0);
    return (-(y-p1[1])*std::exp(-(r1Sqr(x,y)/(r0*r0)))+(y-p2[1])*std::exp(-(r2Sqr(x,y)/(r0*r0))))*omega_e/2.0;
}

/// computation of the vertical
T computeV0(plint iX, plint iY, IncomprFlowParam<T> params){
    // convert and translate x to the [-1,1]x[-1,1] square

     T x = ((T)iX * params.getDeltaX() - 1.);
     T y = ((T)iY * params.getDeltaX() - 1.);

    PLB_ASSERT(-1.0 <= x && x <= 1.0);
    PLB_ASSERT(-1.0 <= y && y <= 1.0);
    return ((x-p1[0])*std::exp(-(r1Sqr(x,y)/(r0*r0)))-(x-p2[0])*std::exp(-(r2Sqr(x,y)/(r0*r0))))*omega_e/2.0;
}

T computeEnstrophy (MultiTensorField2D<T,2>& velocity)
{
    auto_ptr<MultiScalarField2D<T> > vorticity = computeVorticity(velocity);
    auto_ptr<MultiScalarField2D<T> > enstrophy =
        multiply((T)0.5, *multiply(*vorticity,*vorticity) );
    return computeBoundedAverage(*enstrophy);
}



template<typename T>
class dipoleVelocity {
    public:
        dipoleVelocity(IncomprFlowParam<T> params_): params(params_){}
        void operator()(plint iX, plint iY, T& rho, Array<T,2>& u) const {
            rho = 1.0;
            u[0] = computeU0(iX,iY,params)*params.getLatticeU();
            u[1] = computeV0(iX,iY,params)*params.getLatticeU();
        }
    private:
        IncomprFlowParam<T> params;
};

template<typename T>
void geometrySetup( MultiGridLattice2D<T,DESCRIPTOR>& lattice, ConvectiveRefinementParameters<T> const& parameters,
                  OnLatticeBoundaryCondition2D<T,DESCRIPTOR>& boundaryCondition )
{
    for (int iLevel = 0; iLevel < lattice.getNumLevels(); ++iLevel){
      boundaryCondition.setVelocityConditionOnBlockBoundaries(lattice.getComponent(iLevel));
      setBoundaryVelocity( lattice.getComponent(iLevel),
                         lattice.getComponent(iLevel).getBoundingBox(), Array<T,2>((T)0.,(T)0.) );

      initializeAtEquilibrium( lattice.getComponent(iLevel), lattice.getComponent(iLevel).getBoundingBox(),
                               dipoleVelocity<T>(parameters[iLevel]) );


    }

    lattice.initialize();
}

/// Data processor to compute the get the status of the cells
template<typename T, template<typename U> class Descriptor>
class ExtractStatsStatus : public BoxProcessingFunctional2D_LS<T,Descriptor,T>
{
public:
    ExtractStatsStatus(){}
    virtual void process(Box2D domain, BlockLattice2D<T,Descriptor>& lattice, ScalarField2D<T>& res);
    virtual ExtractStatsStatus<T,Descriptor>* clone() const {return new ExtractStatsStatus<T,Descriptor>();}
};



template<typename T, template<typename U> class Descriptor>
void initializeLatticeWithDipoleVelocity(MultiBlockLattice2D<T,Descriptor>& lattice,
                                         IncomprFlowParam<T> const& parameters){

    OnLatticeBoundaryCondition2D<T,DESCRIPTOR>*
        boundaryCondition = createLocalBoundaryCondition2D<T,DESCRIPTOR>();

    boundaryCondition->setVelocityConditionOnBlockBoundaries(lattice);
    setBoundaryVelocity( lattice,lattice.getBoundingBox(),Array<T,2>((T)0.,(T)0.) );

    initializeAtEquilibrium( lattice, lattice.getBoundingBox(), dipoleVelocity<T>(parameters) );
    lattice.initialize();
    delete boundaryCondition;
}


/// Produce a GIF snapshot of the velocity-norm.
template<typename T, template<typename U> class Descriptor>
void writeGifs(MultiBlockLattice2D<T,Descriptor> lattice, plint iter, string fName) {
    plint imSize = 300;
    ImageWriter<T> imageWriter("leeloo");
    imageWriter.writeScaledGif(createFileName(fName, iter, 6),
                               *computeVelocityNorm(lattice),imSize,imSize);
}


template<typename T, template<typename U> class Descriptor>
void writeGifs(MultiGridLattice2D<T,Descriptor> const& lattice, plint iter) {
    for (plint iLevel=0; iLevel<lattice.getNumLevels(); ++iLevel) {
        std::stringstream fname("level_");
        fname << "level_" << iLevel << "_";
        writeGifs(lattice.getComponent(iLevel), iter, fname.str());
    }
}

template<typename T, template<typename U> class Descriptor>
auto_ptr<MultiScalarField2D<T> > solvePressureGS(MultiBlockLattice2D<T,Descriptor>& lattice){
    std::auto_ptr<MultiScalarField2D<T> > density = computeDensity(lattice);
    std::auto_ptr<MultiTensorField2D<T,2> > velocity = computeVelocity(lattice);
    // initial pressure
    std::auto_ptr<MultiScalarField2D<T> > initialValue = multiply( DESCRIPTOR<T>::cs2, *add((T)-1., *density) );
    // the result holder
    std::auto_ptr<MultiScalarField2D<T> > result = multiply( DESCRIPTOR<T>::cs2, *add((T)-1., *density) );
    // right hand side of the equation
    std::auto_ptr<MultiScalarField2D<T> > rhs = computePoissonRHS(*velocity);  ;
    rhs = multiply( -(T)1, *rhs );

    global::timer("gaussSeidel").start();
    GaussSeidelSolver( *initialValue, *result, *rhs, lattice.getBoundingBox(), (T)1e-3, 1000000 );
    T timeGS = global::timer("gaussSeidel").stop();
    pcout << "Time GS = " << timeGS << std::endl;

    *result = *multiply(*result, DESCRIPTOR<T>::invCs2);
    *result = *add(*result, (T)1);
    plb_ofstream poisson(createFileName("GS",lattice.getNx(),6).c_str());
    // write result to the disk
    poisson << *result;

    // writing the image
    ImageWriter<T> imageWriter("leeloo");
    imageWriter.writeScaledPpm(createFileName("GS",lattice.getNx(),6),*result);

    return result;
}


template<class BlockLatticeT>
void writeVTK(BlockLatticeT& lattice,
              IncomprFlowParam<T> const& parameters, plint iLevel, plint iter)
{
    T dx = parameters.getDeltaX();
    T dt = parameters.getDeltaT();

    std::stringstream fname("level_");
        fname << "level_" << iLevel << "_";

    VtkImageOutput2D<T> vtkOut(createFileName(fname.str(), iter, 6), dx);
    vtkOut.writeData<float>(*computeVelocityNorm(lattice), "velocityNorm", dx/dt);
    vtkOut.writeData<2,float>(*computeVelocity(lattice), "velocity", dx/dt);
    vtkOut.writeData<float>(*computeDensity(lattice), "density", 1.);
}

template<typename Ty>
std::string stringify(Ty number) {
    std::ostringstream oss;
    if (!(oss << number)) {
        std::cout << "Error in function stringify" << std::endl;
        exit(1);
    }

    return oss.str();
}



int testFunction()
{

      global::directories().setOutputDir("./tmp/");
      global::setDefaultMultiScaleManager(new ConvectiveMultiScaleManager());

      plint referenceLevel = 0;
      plint behaviorLevel = referenceLevel;
      plint numLevel = 3;
      plint interpLevel = 2;

      plint NRef = 250 / util::roundToInt((util::twoToThePower(numLevel-1)));
      T uMaxRef = 0.005 * (util::twoToThePower(numLevel-1));

      plint N = 100;

      // variable to put to false if you have at least once computed the pressure field
      // from the Poisson equation
      bool computePoisson = false;

      // Rescaling of the velocity for
      T uMax = uMaxRef*(T)NRef/(T)N;
      IncomprFlowParam<T> baseParameters(
              (T) uMax,  // uMax
              (T) Re,    // Re
              N,         // N
              2.,        // lx
              2.         // ly
      );

      // different times in the simulation
      const T logT   = (T)0.1; // show the information
      const T imSave = (T)0.1; // save an image
      const T maxT   = (T)0.4; // final dimensionless time
      const T t0     = (T)0.5; // initial time to compute max of the enstrophy
      const T t1     = (T)0.6; // final time to compute the max of the enstrophy

      // we choose a convective refinement dx=dt and compute parameters for each level
      ConvectiveRefinementParameters<T> parameters (
                                              numLevel,referenceLevel,baseParameters );

      // main object for the grid refinement
      plint overlapWidth = 1;
      MultiGridManagement2D management(baseParameters.getNx(),baseParameters.getNy(), numLevel, overlapWidth,
          behaviorLevel);

      Box2D refinementLevel0(0,2*N,2*N/4,2*3*N/4); // refine the middle of the coarse domain
      Box2D refinementLevel1(4*N/3,2*N,2*N/4+N/12,2*3*N/4-N/12); // refine near the wall where the collision happens

      management.refine(0,refinementLevel0);
      management.refine(1,refinementLevel1.multiply(2));

      Dynamics<T,DESCRIPTOR>* backgroundDynamics = new BGKdynamics<T,DESCRIPTOR>(baseParameters.getOmega());

      // perform a naive parallelization along the x axis
      plint procNumber = global::mpi().getSize();
      plint yTiles = 1;
      plint xTiles = procNumber;
      // object that performs the parallelization of the management
      ParallellizeBySquares2D* parallelizer =
               new ParallellizeBySquares2D(management.getBulks(),
                                           management.getBoundingBox(interpLevel),xTiles,yTiles );
      management.parallelize(parallelizer);

      OnLatticeBoundaryCondition2D<T,DESCRIPTOR>*
          boundaryCondition = createLocalBoundaryCondition2D<T,DESCRIPTOR>();

      // Creating the multi-grid lattice
      MultiGridLattice2D<T,DESCRIPTOR> lattice =
          *MultiGridGenerator2D<T,DESCRIPTOR>::createRefinedLatticeCubicInterpolationFiltering(
          management, backgroundDynamics, behaviorLevel
      );


      T previousIterationTime = T();

      // Small code that allows us to visualize the refined domain as SVG
      ////////////////////////////////////////////////////////////////////////////
      std::vector<MultiScalarField2D<int> > dyn;
      std::vector<std::map<int, std::string> > idToName(numLevel);

      std::set<std::string> uniqueNames;
      for (plint iLevel = 0; iLevel<numLevel; ++iLevel){
          dyn.push_back( *extractDynamicsChain(lattice.getComponent(iLevel),idToName[iLevel]) );
          std::map<int, std::string>::iterator it = idToName[iLevel].begin();
          for (; it != idToName[iLevel].end(); ++it) {
              uniqueNames.insert(it->second);
          }
          pcout << getMultiBlockInfo(lattice.getComponent(iLevel));
      }

      std::map<std::string,int> nameToColor;
      std::set<std::string>::iterator it = uniqueNames.begin();
      int color=0;
      for (; it != uniqueNames.end(); ++it) {
          nameToColor[*it] = color++;
      }

      SVGWriter2D test(management);
      test.writeDomainsWithDynamicsInfo("dipole.svg", 3, dyn, idToName, nameToColor);
      ////////////////////////////////////////////////////////////////////////////

      // create the initial conditions
      geometrySetup(lattice, parameters, *boundaryCondition);

      // if necessary compute the pressure field
      if (computePoisson){
          MultiBlockLattice2D<T,DESCRIPTOR> dummy(
                              lattice.getComponent(0).getNx(),
                              lattice.getComponent(0).getNy(),
                              new BGKdynamics<T,DESCRIPTOR>(parameters[0].getOmega()));
          initializeLatticeWithDipoleVelocity(dummy,parameters[0]);
          std::auto_ptr<MultiScalarField2D<T> > press = solvePressureGS(dummy);

          for (plint iLevel=1; iLevel<numLevel; ++iLevel){
              MultiBlockLattice2D<T,DESCRIPTOR> dummyLattice(
                              lattice.getComponent(iLevel).getNx(),
                              lattice.getComponent(iLevel).getNy(),
                              new BGKdynamics<T,DESCRIPTOR>(parameters[iLevel].getOmega()));

              initializeLatticeWithDipoleVelocity(dummyLattice,parameters[iLevel]);
              auto_ptr<MultiScalarField2D<T> > p = solvePressureGS(dummyLattice);
          }
      }


      // read the file containing the poisson result
      for (plint iLevel=0; iLevel<lattice.getNumLevels(); ++iLevel){
          pcout << "Reading poisson file for level " <<
              createFileName("GS",lattice.getComponent(iLevel).getNx(),6) << std::endl;

          plb_ifstream poisson(createFileName("GS",lattice.getComponent(iLevel).getNx(),6).c_str());

          MultiScalarField2D<T> press(lattice.getComponent(iLevel));

          poisson >> press;
          std::auto_ptr<MultiTensorField2D<T,2> > vel = computeVelocity(lattice.getComponent(iLevel));
          std::auto_ptr<MultiTensorField2D<T,3> > S = computeStrainRate(*vel);
          recomposeFromFlowVariables(lattice.getComponent(iLevel), press, *vel, *S);
//          recomposeFromFileFlowVariables(lattice.getComponent(iLevel), press, *vel, *S);

      }



      // output for the enstrophy for post-processing
      std::string fname = "enstrophy"+stringify(N)+".dat";
      plb_ofstream out(fname.c_str());
      // to compute the max of enstrophy
      std::vector<T> enstrophy;

      // Main loop over time iterations.
      global::timer("total").restart();
      for (plint iT=0; iT*baseParameters.getDeltaT()<maxT; ++iT) {
          global::timer("mainLoop").restart();

          if (iT%baseParameters.nStep(imSave)==0 ) {
              pcout << "Saving Gif ..." << endl;
              // in order to compute statistics and view the lattice, we interpolate all levels to
              // the finest one
              writeVTK(*lattice.convertToLevel(interpLevel),parameters[interpLevel],interpLevel,iT);
              MultiBlockLattice2D<T,DESCRIPTOR> r = *lattice.convertToLevel(interpLevel);
              writeGifs(r,iT,"dipole");
          }

          if (iT%baseParameters.nStep(logT)==0) {
              pcout << "step " << iT << "; t=" << iT*baseParameters.getDeltaT();
          }

          lattice.collideAndStream();

          // Computation of the max of the enstrophy on the critic times
          if (iT*baseParameters.getDeltaT() >= t0 && iT*baseParameters.getDeltaT() <= t1) {
              MultiTensorField2D<T,2> velocity = *computeVelocity(*lattice.convertToLevel(interpLevel));
              T e = computeEnstrophy(velocity)/(parameters[numLevel-1].getDeltaT()*parameters[interpLevel].getDeltaT());
              enstrophy.push_back(4.0*e);
          }
          // Print
          if (iT%baseParameters.nStep(logT)==0) {
              MultiTensorField2D<T,2> velocity = *computeVelocity(*lattice.convertToLevel(interpLevel));
              T e = computeEnstrophy(velocity)/(parameters[interpLevel].getDeltaT()*parameters[interpLevel].getDeltaT());
              pcout << "; Entstrophy= " << 4.0*e;
              pcout << std::endl;
              out << iT*baseParameters.getDeltaT() << " " << 4.0*e << endl;
              pcout << "Previous iteration time = " << previousIterationTime << std::endl;

          }

          previousIterationTime = global::timer("mainLoop").stop();
      }

      previousIterationTime = global::timer("total").stop();
      pcout << "total time: " << previousIterationTime << std::endl;

      T maxEnsTime = T();
      T maxEnstrophy = T();
      for (unsigned iT = 0; iT < enstrophy.size(); ++iT) {
          if (maxEnstrophy < enstrophy[iT]) {
              maxEnstrophy = enstrophy[iT];
              maxEnsTime   = (T)iT *baseParameters.getDeltaT() + t0;
          }
      }
      pcout << "maximum enstrophy = " << maxEnstrophy << ", at time = " << maxEnsTime << std::endl;

      delete boundaryCondition;

    return 0;
}



////    std::vector<MultiBlockLattice2D<T,Descriptor>*> blocks;
////    blocks.push_back(&lattice);
////    plb::BlockLattice2D<T,Descriptor>& lattices = *dynamic_cast<BlockLattice2D<T,Descriptor>*>(blocks[0]);
////    Dot2D offset = lattices.getLocation();
////    cout << "offset.x = " << offset.x << "  ; offset.y = " << offset.y << endl;


////    std::vector<BlockLattice3D<T,Descriptor>*> lattices1 =&lattice;
////    Dot2D offset = lattices1.getLocation();
////    cout << "offset.x = " << offset.x << "  ; offset.y = " << offset.y << endl;

//    //~ std::vector<AtomicBlock2D*> blocks;
//    std::vector<MultiBlockLattice2D<double, Descriptor>*> blocks;
//    blocks.push_back(&lattice);
//    plb::AtomicBlock2D& latticesAtomic = *dynamic_cast<AtomicBlock2D*>(blocks[0]);
//    //plb::BlockLattice2D<T,Descriptor>& lattices = *dynamic_cast<BlockLattice2D<T,Descriptor>*>(blocks[0]);
//    //Dot2D offset = lattices.getLocation();
//    //cout << "offset.x = " << offset.x << "  ; offset.y = " << offset.y << endl;
//    //Dot2D offset = lattices.getLocation();
//    //cout << "offset.x = " << offset.x << "  ; offset.y = " << offset.y << endl;
//    //std::vector<BlockLattice3D<T,Descriptor>*> lattices1 =&lattice;

//    Dot2D offset = latticesAtomic.getLocation();
////    cout << "offset.x = " << offset.x << "  ; offset.y = " << offset.y << endl;

#endif // FUNCTION_TEST_H
