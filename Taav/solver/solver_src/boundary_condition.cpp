#include "boundary_condition.h"




template<class BlockLatticeT, class Boundary,class T>
void BoundaryConditonFunction(BlockLatticeT& lattice,
                              Boundary* boundaryCondition,
                              Parameters& Param){

plb::plint nx = Param.lx;
plb::plint ny = Param.ly;


plb::pcout << " nx = " << nx << " || ny = " << ny << std::endl;

if (Param.FlagPeriodicX ){
lattice.periodicity().toggle(0,true);
}
else {

switch_flags_2( Param.BoundaryConditionTypeXP.compare("velocity") == 0 ,
        Param.BoundaryConditionTypeXP.compare("pressure") == 0 )
{
    case flags_2(T,F):
    //pcout<< " Boundary  =  velocity 1 " << endl;
          //boundaryCondition->addVelocityBoundary0P(plb::Box2D (nx, nx, 0, ny-1), lattice);
      boundaryCondition->setVelocityConditionOnBlockBoundaries (
        lattice, plb::Box2D (nx-1, nx-1, 1, ny-1) );

        setBoundaryVelocity(lattice, plb::Box2D (nx-1, nx-1, 1, ny-1) , plb::Array<T,2>(0.1,0.) );
      break;
    case flags_2(F,T):
      boundaryCondition->addPressureBoundary0P(plb::Box2D (nx-1, nx-1, 1, ny-1) , lattice);
      break;
}

switch_flags_2( Param.BoundaryConditionTypeXN.compare("velocity") == 0 ,
        Param.BoundaryConditionTypeXN.compare("pressure") == 0 )
{
    case flags_2(T,F):
    //pcout<< " Boundary  =  velocity 2" << endl;
          //boundaryCondition->addVelocityBoundary0N(Box2D (0, 0, 0, ny-1), lattice);
      boundaryCondition->setVelocityConditionOnBlockBoundaries (
        lattice, plb::Box2D (0, 0, 0, ny));
        setBoundaryVelocity(lattice, plb::Box2D (0, 0, 0, ny) , plb::Array<T,2>(0.,0.) );
      break;
    case flags_2(F,T):
      boundaryCondition->addPressureBoundary0N(plb::Box2D (0, 0, 0, ny), lattice);
      break;
}
}

if (Param.FlagPeriodicY ){
lattice.periodicity().toggle(1,true);
}
else {

switch_flags_2( Param.BoundaryConditionTypeYP.compare("velocity") == 0 ,
                Param.BoundaryConditionTypeYP.compare("pressure") == 0 )
{
    case flags_2(T,F):
    //pcout<< " Boundary  =  velocity 3" << endl;
          //boundaryCondition->addVelocityBoundary1P(Box2D (0, nx-1, ny-1, ny-1), lattice);
      boundaryCondition->setVelocityConditionOnBlockBoundaries (
        lattice, plb::Box2D (1, nx-1, ny-1, ny-1));
        setBoundaryVelocity(lattice, plb::Box2D (1, nx-1, ny-1, ny-1) , plb::Array<T,2>(0.0,0.) );
      break;
    case flags_2(F,T):
      boundaryCondition->addPressureBoundary1P(plb::Box2D (1, nx-1, ny-1, ny-1), lattice);
      break;
}

switch_flags_2( Param.BoundaryConditionTypeYN.compare("velocity") == 0 ,
        Param.BoundaryConditionTypeYN.compare("pressure") == 0 )
{
    case flags_2(T,F):
    //pcout<< " Boundary  =  velocity 4" << endl;
          //boundaryCondition->addVelocityBoundary1N(plb::Box2D (0, nx-1, 0, 0), lattice);
      boundaryCondition->setVelocityConditionOnBlockBoundaries (
        lattice, plb::Box2D(1, nx-1, 0, 0));
        setBoundaryVelocity(lattice, plb::Box2D(1, nx-1, 0, 0), plb::Array<T,2>(0.0,0.) );
      break;
    case flags_2(F,T):
      boundaryCondition->addPressureBoundary1N(plb::Box2D (1, nx-1, 0, 0), lattice);
      break;
}
}


//pcout << "velocityBCYPx  = " << Param.velocityBCYPx << " parameters.getLatticeU()  = "  <<  parameters.getLatticeU()  << endl;

//boundaryCondition->setVelocityConditionOnBlockBoundaries(lattice);

//T u = atof(Param.velocityBCYPx.c_str())*0;

//setBoundaryVelocity(lattice, lattice.getBoundingBox(), Array<T,2>(0.,0.) );
//initializeAtEquilibrium(lattice, lattice.getBoundingBox(), 1., Array<T,2>(0.,0.) );

//~ T u = parameters.getLatticeU() ;

////setBoundaryVelocity(lattice, plb::Box2D(1, Param.lx-2, Param.ly-1, Param.ly-1), Array<T,2>(u,0.) );
////initializeAtEquilibrium(lattice, plb::Box2D(1, Param.lx-2, Param.ly-1, Param.ly-1), 1., Array<T,2>(-u,0.) );

////setBoundaryVelocity(lattice, plb::Box2D(1, Param.lx-2, 0, 0), Array<T,2>(-u,0.) );
////initializeAtEquilibrium(lattice, plb::Box2D(1, Param.lx-2, 0, 0), 1., Array<T,2>(-u,0.) );

//lattice.initialize();
}


