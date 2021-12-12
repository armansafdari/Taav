//#ifndef BOUNDARY_CONDITION_H
//#define BOUNDARY_CONDITION_H

#include "palabos2D.h"
#include "palabos2D.hh"
#include "Ui_Parameter_Struct.h"
#include "switch_flags.h"


template<class BlockLatticeT, class Boundary>
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

    plb::Box2D XPBoundary (nx-1, nx-1, 2, ny-2) ;
    plb::Box2D XNBoundary (0, 0, 2, ny-2) ;


switch_flags_2( Param.BoundaryConditionXP.compare("velocity") == 0 ,
                Param.BoundaryConditionXP.compare("pressure") == 0)
{
    case flags_2(T,F):
        switch_flags_5( Param.BoundaryConditionVelocityTypeXP.compare("dirichlet") == 0 ,
                        Param.BoundaryConditionVelocityTypeXP.compare("outflow") == 0 ,
                        Param.BoundaryConditionVelocityTypeXP.compare("neumann") == 0 ,
                        Param.BoundaryConditionVelocityTypeXP.compare("freeslip") == 0 ,
                        Param.BoundaryConditionVelocityTypeXP.compare("normalOutflow") == 0)
        {
        case flags_5(T,F,F,F,F):
       // plb::pcout<< " Boundary  =  velocity 1 " << std::endl;
              //boundaryCondition->addVelocityBoundary0P(plb::Box2D (nx, nx, 0, ny-1), lattice);
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, XPBoundary, plb::boundary::dirichlet );
            //setBoundaryVelocity(lattice, plb::Box2D (nx-1, nx-1, 1, ny-1) , plb::Array<T,2>(0.1,0.) );
          break;

        case flags_5(F,T,F,F,F):

        // plb::pcout<< " Boundary  =  velocity 1 " << std::endl;
              //boundaryCondition->addVelocityBoundary0P(plb::Box2D (nx, nx, 0, ny-1), lattice);
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, XPBoundary, plb::boundary::outflow );
            //setBoundaryVelocity(lattice, plb::Box2D (nx-1, nx-1, 1, ny-1) , plb::Array<T,2>(0.1,0.) );
          break;

        case flags_5(F,F,T,F,F):

        // plb::pcout<< " Boundary  =  velocity 1 " << std::endl;
              //boundaryCondition->addVelocityBoundary0P(plb::Box2D (nx, nx, 0, ny-1), lattice);
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, XPBoundary, plb::boundary::neumann );
            //setBoundaryVelocity(lattice, plb::Box2D (nx-1, nx-1, 1, ny-1) , plb::Array<T,2>(0.1,0.) );
          break;

        case flags_5(F,F,F,T,F):

        // plb::pcout<< " Boundary  =  velocity 1 " << std::endl;
              //boundaryCondition->addVelocityBoundary0P(plb::Box2D (nx, nx, 0, ny-1), lattice);
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, XPBoundary , plb::boundary::freeslip );
            //setBoundaryVelocity(lattice, plb::Box2D (nx-1, nx-1, 1, ny-1) , plb::Array<T,2>(0.1,0.) );
          break;

        case flags_5(F,F,F,F,T):

        // plb::pcout<< " Boundary  =  velocity 1 " << std::endl;
              //boundaryCondition->addVelocityBoundary0P(plb::Box2D (nx, nx, 0, ny-1), lattice);
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, XPBoundary , plb::boundary::normalOutflow);
            //setBoundaryVelocity(lattice, plb::Box2D (nx-1, nx-1, 1, ny-1) , plb::Array<T,2>(0.1,0.) );
          break;

        }

    case flags_2(F,T):
        switch_flags_2( Param.BoundaryConditionPressureTypeXP.compare("dirichlet") == 0 ,
                        Param.BoundaryConditionPressureTypeXP.compare("neumann") == 0 )
        {
        case flags_2(T,F):

     //      boundaryCondition->addPressureBoundary0P(plb::Box2D (nx-1, nx-1, 1, ny-1) , lattice);
          boundaryCondition->setPressureConditionOnBlockBoundaries(
                                  lattice,XPBoundary, plb::boundary::dirichlet);
                      break;
        case flags_2(F,T):

    //      boundaryCondition->addPressureBoundary0P(plb::Box2D (nx-1, nx-1, 1, ny-1) , lattice);
          boundaryCondition->setPressureConditionOnBlockBoundaries(
                      lattice, XPBoundary, plb::boundary::neumann);
          break;
        }


}

switch_flags_2( Param.BoundaryConditionXN.compare("velocity") == 0 ,
                Param.BoundaryConditionXN.compare("pressure") == 0)
{
    case flags_2(T,F):
        switch_flags_5( Param.BoundaryConditionVelocityTypeXN.compare("dirichlet") == 0 ,
                        Param.BoundaryConditionVelocityTypeXN.compare("outflow") == 0 ,
                        Param.BoundaryConditionVelocityTypeXN.compare("neumann") == 0 ,
                        Param.BoundaryConditionVelocityTypeXN.compare("freeslip") == 0 ,
                        Param.BoundaryConditionVelocityTypeXN.compare("normalOutflow") == 0)
        {
        case flags_5(T,F,F,F,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, XNBoundary, plb::boundary::dirichlet );
          break;

        case flags_5(F,T,F,F,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, XNBoundary, plb::boundary::outflow );
          break;

        case flags_5(F,F,T,F,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, XNBoundary, plb::boundary::neumann );
          break;

        case flags_5(F,F,F,T,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, XNBoundary , plb::boundary::freeslip );
          break;

        case flags_5(F,F,F,F,T):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, XNBoundary, plb::boundary::normalOutflow);
          break;

        }

    case flags_2(F,T):
        switch_flags_2( Param.BoundaryConditionPressureTypeXN.compare("dirichlet") == 0 ,
                        Param.BoundaryConditionPressureTypeXN.compare("neumann") == 0 )
        {
        case flags_2(T,F):

          boundaryCondition->setPressureConditionOnBlockBoundaries(
                                  lattice, XNBoundary , plb::boundary::dirichlet);
                      break;
        case flags_2(F,T):
          boundaryCondition->setPressureConditionOnBlockBoundaries(
                      lattice, XNBoundary , plb::boundary::neumann);
          break;
        }
}

}

if (Param.FlagPeriodicY ){
lattice.periodicity().toggle(1,true);
}
else {

    plb::Box2D YPBoundary (0, nx-1, ny-1, ny-1) ;
    plb::Box2D YNBoundary (0, nx-1, 0, 0);


switch_flags_2( Param.BoundaryConditionYP.compare("velocity") == 0 ,
                Param.BoundaryConditionYP.compare("pressure") == 0)
{
    case flags_2(T,F):
        switch_flags_5( Param.BoundaryConditionVelocityTypeYP.compare("dirichlet") == 0 ,
                        Param.BoundaryConditionVelocityTypeYP.compare("outflow") == 0 ,
                        Param.BoundaryConditionVelocityTypeYP.compare("neumann") == 0 ,
                        Param.BoundaryConditionVelocityTypeYP.compare("freeslip") == 0 ,
                        Param.BoundaryConditionVelocityTypeYP.compare("normalOutflow") == 0)
        {
        case flags_5(T,F,F,F,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, YPBoundary, plb::boundary::dirichlet );
          break;

        case flags_5(F,T,F,F,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, YPBoundary, plb::boundary::outflow );
          break;

        case flags_5(F,F,T,F,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, YPBoundary, plb::boundary::neumann );
          break;

        case flags_5(F,F,F,T,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, YPBoundary , plb::boundary::freeslip );
          break;

        case flags_5(F,F,F,F,T):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, YPBoundary , plb::boundary::normalOutflow);
          break;

        }

    case flags_2(F,T):
        switch_flags_2( Param.BoundaryConditionPressureTypeYP.compare("dirichlet") == 0 ,
                        Param.BoundaryConditionPressureTypeYP.compare("neumann") == 0 )
        {
        case flags_2(T,F):
          boundaryCondition->setPressureConditionOnBlockBoundaries(
                                  lattice,YPBoundary, plb::boundary::dirichlet);
                      break;
        case flags_2(F,T):
          boundaryCondition->setPressureConditionOnBlockBoundaries(
                      lattice, YPBoundary, plb::boundary::neumann);
          break;
        }
}

switch_flags_2( Param.BoundaryConditionYN.compare("velocity") == 0 ,
                Param.BoundaryConditionYN.compare("pressure") == 0)
{
    case flags_2(T,F):
        switch_flags_5( Param.BoundaryConditionVelocityTypeYN.compare("dirichlet") == 0 ,
                        Param.BoundaryConditionVelocityTypeYN.compare("outflow") == 0 ,
                        Param.BoundaryConditionVelocityTypeYN.compare("neumann") == 0 ,
                        Param.BoundaryConditionVelocityTypeYN.compare("freeslip") == 0 ,
                        Param.BoundaryConditionVelocityTypeYN.compare("normalOutflow") == 0)
        {
        case flags_5(T,F,F,F,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, YNBoundary, plb::boundary::dirichlet );
          break;

        case flags_5(F,T,F,F,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, YNBoundary, plb::boundary::outflow );
          break;

        case flags_5(F,F,T,F,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, YNBoundary, plb::boundary::neumann );
          break;

        case flags_5(F,F,F,T,F):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, YNBoundary , plb::boundary::freeslip );
          break;

        case flags_5(F,F,F,F,T):
          boundaryCondition->setVelocityConditionOnBlockBoundaries (
            lattice, YNBoundary, plb::boundary::normalOutflow);
          break;
        }

    case flags_2(F,T):
        switch_flags_2( Param.BoundaryConditionPressureTypeYN.compare("dirichlet") == 0 ,
                        Param.BoundaryConditionPressureTypeYN.compare("neumann") == 0 )
        {
        case flags_2(T,F):
           boundaryCondition->setPressureConditionOnBlockBoundaries(
                                  lattice, YNBoundary , plb::boundary::dirichlet);
          break;
        case flags_2(F,T):
          boundaryCondition->setPressureConditionOnBlockBoundaries(
                      lattice, YNBoundary , plb::boundary::neumann);
          break;
        }
}
}

}

//#endif // BOUNDARY_CONDITION_H
