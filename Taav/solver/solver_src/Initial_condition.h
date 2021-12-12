#ifndef INITIAL_CONDITION_H
#define INITIAL_CONDITION_H



#include "palabos2D.h"
#include "palabos2D.hh"

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "switch_flags.h"
//#include "exprtk.hpp"
#include "Convert_Script_to_number.cpp"

#include "Ui_Parameter_Struct.h"
//#include "multiblockField.hh"

using namespace plb;
using namespace std;

typedef double T;
#define DESCRIPTOR descriptors::D2Q9Descriptor




/////////////* ************* Class SetCustomBoundaryVelocityFunctional2D ******************* */

//////template<typename T, template<typename U> class plb::descriptors::D2Q9Descriptor, class VelocityFunction>
//////class Initializer : public OneCellIndexedFunctional2D<T,plb::descriptors::D2Q9Descriptor>
//////{
//////public:
    //////Initializer(VelocityFunction f_);
    //////virtual Initializer<T,plb::descriptors::D2Q9Descriptor,VelocityFunction>* clone() const;
    //////virtual void execute(plint iX, plint iY, Cell<T,plb::descriptors::D2Q9Descriptor>& cell) const;
    //////virtual void setscale(int dxScale, int dtScale);
//////private:
    //////VelocityFunction f;
    //////T velocityScale;
//////};


///////* ************* Class SetConstBoundaryVelocityFunctional2D ******************* */

//////template<typename T, template<typename U> class plb::descriptors::D2Q9Descriptor>
//////Initializer<T,plb::descriptors::D2Q9Descriptor>::Initializer (
        //////Array<T,plb::descriptors::D2Q9Descriptor<T>::d> velocity )
    //////: u(velocity)
//////{ }

//////template<typename T, template<typename U> class plb::descriptors::D2Q9Descriptor>
//////void Initializer<T,plb::descriptors::D2Q9Descriptor>::process (
        //////Box2D domain, BlockLattice2D<T,plb::descriptors::D2Q9Descriptor>& lattice )
//////{
    //////int dimDx = 1;
    //////int dimDt = -1;
    //////T scaleFactor = scaleFromReference(this->getDxScale(), dimDx,
                                       //////this->getDtScale(), dimDt);
    //////Array<T,2> scaledU = u*scaleFactor;
    //////for (plint iX=domain.x0; iX<=domain.x1; ++iX) {
        //////for (plint iY=domain.y0; iY<=domain.y1; ++iY) {
            //////lattice.get(iX,iY).defineVelocity(scaledU);
        //////}
    //////}
//////}

//////template<typename T, template<typename U> class plb::descriptors::D2Q9Descriptor>
//////Initializer<T,plb::descriptors::D2Q9Descriptor>*
    //////Initializer<T,plb::descriptors::D2Q9Descriptor>::clone() const
//////{
    //////return new Initializer<T,plb::descriptors::D2Q9Descriptor>(*this);
//////}

//////template<typename T, template<typename U> class plb::descriptors::D2Q9Descriptor>
//////BlockDomain::DomainT Initializer<T,plb::descriptors::D2Q9Descriptor>::appliesTo() const
//////{
    //////// Boundary condition needs to be set on envelope nodes as well to ensure
    ////////   proper behavior.
    //////return BlockDomain::bulkAndEnvelope;
//////}

//////template<typename T, template<typename U> class plb::descriptors::D2Q9Descriptor>
//////void Initializer<T,plb::descriptors::D2Q9Descriptor>::getTypeOfModification (
        //////std::vector<modif::ModifT>& modified) const
//////{
    //////modified[0] = modif::staticVariables;
//////}


double velocityFunctionX(plint iX , plint iY ,Parameters Param){
    return  Script_to_Number((double)iX , (double) iY , Param.velocityX.c_str()) ; 
}

double velocityFunctionY(plint iX , plint iY ,Parameters Param){
    
    return  Script_to_Number((double)iX , (double) iY , Param.velocityY.c_str());;
    
}
double DensityFunction(plint iX , plint iY ,Parameters Param){
    
    return  Script_to_Number((double)iX , (double) iY , Param.Density.c_str());;
    
}

/// A functional, used to initialize the velocity for the boundary conditions
template<typename T>
class InitialConditionVelocity {
public:
    InitialConditionVelocity(Parameters param_)
        : Param(param_)
    { }
    void operator()(plint iX, plint iY, Array<T,2>& u) const {
	
	    u[0] = velocityFunctionX(iX, iY, Param);
        u[1] = velocityFunctionY(iX, iY, Param);
    }
private:
    Parameters Param;
};

/// A functional, used to initialize a pressure boundary to constant density
template<typename T>
class InitialConditionDensity {
public:
    InitialConditionDensity(Parameters param_)
        : Param(param_)
    { }
    T operator()(plint iX, plint iY) const {
        return DensityFunction(iX,iY,Param);
    }
private:
    Parameters Param;
};


/// A functional, used to create an initial condition for the density and velocity
template<typename T>
class InitialConditionVelocityAndDensity {
public:
    InitialConditionVelocityAndDensity(Parameters param_)
        : Param(param_)
    { }
    void operator()(plint iX, plint iY, T& rho, Array<T,2>& u) const {
        rho = DensityFunction(iX,iY,Param);
        u[0] = velocityFunctionX(iX, iY , Param);
        u[1] = velocityFunctionY(iX, iY , Param);
    }
private:
    Parameters Param;
};


//////void cavitySetup( MultiBlockLattice2D<T,DESCRIPTOR>& lattice,
                  //////IncomprFlowParam<T> const& parameters,
                  //////OnLatticeBoundaryCondition2D<T,DESCRIPTOR>& boundaryCondition )
//////{
    //////const plint nx = parameters.getNx();
    //////const plint ny = parameters.getNy();

    //////boundaryCondition.setVelocityConditionOnBlockBoundaries(lattice);

    //////setBoundaryVelocity(lattice, lattice.getBoundingBox(), Array<T,2>(0.,0.) );
    //////initializeAtEquilibrium(lattice, lattice.getBoundingBox(), 1., Array<T,2>(0.,0.) );

    //////T u = parameters.getLatticeU();
    //////setBoundaryVelocity(lattice, Box2D(1, nx-2, ny-1, ny-1), Array<T,2>(u,0.) );
    //////initializeAtEquilibrium(lattice, Box2D(1, nx-2, ny-1, ny-1), 1., Array<T,2>(u,0.) );

    //////lattice.initialize();
//////}

template<class BlockLatticeT>
void writeGif(BlockLatticeT& lattice, plint iter)
{
    const plint imSize = 600;

    ImageWriter<T> imageWriter("leeloo");
    imageWriter.writeScaledGif(createFileName("uNorm", iter, 6),
                               *computeVelocityNorm(lattice),
                               imSize, imSize );
    imageWriter.writeScaledGif(createFileName("logUnorm", iter, 6),
                               *computeLog(*add(1.e-8,*computeVelocityNorm(lattice))),
                               imSize, imSize );
                               
    imageWriter.writeScaledGif(createFileName("Density", iter, 6),
                               *computeDensity(lattice),
                               imSize, imSize );
}

template<class BlockLatticeT>
void writeVTK(BlockLatticeT& lattice,
              Parameters Param, plint iter)
{
    T dx = Param.lx;
    T dt = 1;
    VtkImageOutput2D<T> vtkOut(createFileName("vtk", iter, 6), dx);
    vtkOut.writeData<float>(*computeVelocityNorm(lattice), "velocityNorm", dx/dt);
    vtkOut.writeData<2,float>(*computeVelocity(lattice), "velocity", dx/dt);
}

#endif 
