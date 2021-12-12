#ifndef INITIAL_CONDITION_H
#define INITIAL_CONDITION_H


#include "initial_condition.hh"
//#include "data_initialization.h"



////////////////////////////// Test



/* *************** Class RecomposeFromFlowVariablesFunctional2D ******************* */

template<typename T, template<typename U> class Descriptor>
class RecomposeFromFlowVariablesFunctional2DTest : public plb::BoxProcessingFunctional2D
{
public:
    RecomposeFromFlowVariablesFunctional2DTest(Parameters Param_):Param(Param_){}
    virtual void processGenericBlocks(plb::Box2D domain,
                                      std::vector<plb::AtomicBlock2D*> atomicBlocks);
    virtual RecomposeFromFlowVariablesFunctional2DTest<T,Descriptor>* clone() const;
    virtual plb::BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<plb::modif::ModifT>& modified) const;
private:
    Parameters Param;
};



/* ************* Class RecomposeFromFlowVariablesFunctional2D ******************* */

template<typename T, template<typename U> class Descriptor>
void RecomposeFromFlowVariablesFunctional2DTest<T,Descriptor>::processGenericBlocks (
        plb::Box2D domain, std::vector<plb::AtomicBlock2D*> atomicBlocks )
{
    plb::BlockLattice2D<T,Descriptor>& lattice =
        dynamic_cast<plb::BlockLattice2D<T,Descriptor>&>(*atomicBlocks[0]);
    plb::ScalarField2D<T> const& rhoField =
        dynamic_cast<plb::ScalarField2D<T> const&>(*atomicBlocks[1]);
    plb::TensorField2D<T,2> const& velField =
        dynamic_cast<plb::TensorField2D<T,2> const&>(*atomicBlocks[2]);

    plb::Dot2D offset1 = computeRelativeDisplacement(lattice, rhoField);
    plb::Dot2D offset2 = computeRelativeDisplacement(lattice, velField);


    std::vector<T> rawData(1+Descriptor<T>::d+plb::SymmetricTensor<T,Descriptor>::n
                            +Descriptor<T>::ExternalField::numScalars);

    for (plb::plint iX=domain.x0; iX<=domain.x1; ++iX) {
        for (plb::plint iY=domain.y0; iY<=domain.y1; ++iY) {
            plb::Cell<T,Descriptor>& cell = lattice.get(iX,iY);
            T rho               = rhoField.get(iX+offset1.x, iY+offset1.y);
            plb::Array<T,2> const& u = velField.get(iX+offset2.x, iY+offset2.y);

//            plb::Array<T,2>  u ;
//            u[0] = velocityFunctionX(iX+offset1.x, iY+offset1.y, Param);
//            u[1] = velocityFunctionY(iX+offset1.x, iY+offset1.y, Param);

            // Convert rho --> rhoBar.
            rawData[0] = Descriptor<T>::rhoBar(rho);

            // Convert u --> j
            rawData[1] = rho*u[0];
            rawData[2] = rho*u[1];

            rawData[3] = T();
            rawData[4] = T();
            rawData[5] = T();

            // Recompose the cell.
            plb::plint recomposeOrder = 0;
            cell.getDynamics().recompose(cell, rawData, recomposeOrder);
        }
    }
}

template<typename T, template<typename U> class Descriptor>
RecomposeFromFlowVariablesFunctional2DTest<T,Descriptor>*
    RecomposeFromFlowVariablesFunctional2DTest<T,Descriptor>::clone() const
{
    return new RecomposeFromFlowVariablesFunctional2DTest<T,Descriptor>(*this);
}

template<typename T, template<typename U> class Descriptor>
plb::BlockDomain::DomainT RecomposeFromFlowVariablesFunctional2DTest<T,Descriptor>::appliesTo() const
{
    return plb::BlockDomain::bulk;
}

template<typename T, template<typename U> class Descriptor>
void RecomposeFromFlowVariablesFunctional2DTest<T,Descriptor>::getTypeOfModification (
        std::vector<plb::modif::ModifT>& modified) const
{
    modified[0] = plb::modif::staticVariables;
    modified[1] = plb::modif::nothing;
}





////////////  Test   ///////////////////////////////








template<typename T, template<typename U> class Descriptor>
void recomposeFromFlowVariablesTest ( plb::MultiBlockLattice2D<T,Descriptor>& lattice,
                                      plb::MultiScalarField2D<T>& density,
                                      plb::MultiTensorField2D<T,2>& vel,
                                      Parameters Param, plb::Box2D domain )
{
    std::vector<plb::MultiBlock2D*> multiBlocks(3);
    multiBlocks[0] = &lattice;
    multiBlocks[1] = &density;
    multiBlocks[2] = &vel;

    applyProcessingFunctional( new RecomposeFromFlowVariablesFunctional2DTest<T,Descriptor>(Param), domain,
                               multiBlocks );
}


template<typename Type, template<typename U> class Descriptor,class BlockLatticeT>
void InitialConditionFunction(BlockLatticeT& lattice,
                              Parameters& Param){

//if (Param.FlagInitialConditionDensity)


  switch_flags_3(Param.FlagInitialConditionDensity, Param.FlagInitialConditionVelocity, Param.FlagExternalForce){

   case flags_3(F,F,F):
      {
          setBoundaryVelocity (
                  lattice, lattice.getBoundingBox(),
                  InitialConditionVelocity<Type>(Param) );
          setBoundaryDensity (
                  lattice, lattice.getBoundingBox(),
                  InitialConditionDensity<Type>(Param) );

          initializeAtEquilibrium (
                  lattice, lattice.getBoundingBox(),
                  InitialConditionVelocityAndDensity<Type>(Param) );
        }
          break;

      case flags_3(T,F,F):
      {
             setBoundaryVelocity (
                     lattice, lattice.getBoundingBox(),
                     InitialConditionVelocity<Type>(Param) );
             plb::MultiScalarField2D<Type> Density(lattice);

             plb::plb_ifstream inDensityfile(Param.FilenameInitialConditionDensity.c_str());
             if (inDensityfile.is_open()) {
                 inDensityfile >> Density;
                 setBoundaryDensity (
                              lattice, lattice.getBoundingBox(),
                              InitialConditionDensityScalarField<Type>(Param, &Density) );
                 initializeAtEquilibrium (
                      lattice, lattice.getBoundingBox(),
                      InitialConditionVelocityAndDensityScalarField<Type>(Param, &Density) );
               }
        }
          break;

      case flags_3(F,T,F):

        {

          plb::pcout << "Initialization by velocity file is not complete."<< std::endl;
          exit (EXIT_FAILURE);
             setBoundaryDensity (
                     lattice, lattice.getBoundingBox(),
                     InitialConditionDensity<Type>(Param) );

             plb::TensorField2D<Type,Descriptor<Type>::d> Velocity(Param.lx,Param.ly);

             plb::plb_ifstream inVelocityfile(Param.FilenameInitialConditionVelocity.c_str());
             if (inVelocityfile.is_open()) {
                 inVelocityfile >> Velocity;
             }
             setBoundaryVelocity (
                     lattice, lattice.getBoundingBox(),
                     InitialConditionVelocityTensorField<Type,Descriptor>(Param, &Velocity) );

             initializeAtEquilibrium (
                  lattice, lattice.getBoundingBox(),
                  InitialConditionVelocityTensorFieldAndDensity<Type,Descriptor>(Param, &Velocity) );
        }
        break;
    case flags_3(T,T,F):
      {
          plb::plb_ifstream inDensityfile(Param.FilenameInitialConditionDensity.c_str());
          plb::plb_ifstream inVelocityfile(Param.FilenameInitialConditionVelocity.c_str());

          if (inDensityfile.is_open() ) {
              plb::MultiScalarField2D<Type> Density(lattice);
              inDensityfile >> Density;

              std::auto_ptr<plb::MultiTensorField2D<Type,2> > vel = computeVelocity(lattice);
              inVelocityfile >> *vel;

              recomposeFromFlowVariablesTest(lattice,Density,*vel,Param, lattice.getBoundingBox());



          }

//          plb::plb_ifstream inVelocityfile(Param.FilenameInitialConditionVelocity.c_str());
//          if (inDensityfile.is_open()) {
//              inVelocityfile >> Velocity;
//          }


//          setBoundaryDensity (
//                      lattice, lattice.getBoundingBox(),
//                      InitialConditionDensityScalarField<T>(Param, Density) );

//          setBoundaryVelocity (
//                  lattice, lattice.getBoundingBox(),
//                  InitialConditionVelocityTensorField<T>(Param,Velocity) );

      }
          break;

      case flags_3(F,F,T):
         {
          setBoundaryVelocity (
                  lattice, lattice.getBoundingBox(),
                  InitialConditionVelocity<Type>(Param) );
          setBoundaryDensity (
                  lattice, lattice.getBoundingBox(),
                  InitialConditionDensity<Type>(Param) );

          setExternalVector(lattice, lattice.getBoundingBox(),
                            Descriptor<Type>::ExternalField::forceBeginsAt, InitialConditionForce<Type>(Param));

          initializeAtEquilibrium (
                  lattice, lattice.getBoundingBox(),
                  InitialConditionVelocityAndDensity<Type>(Param) );
           }
             break;

  }


//if(Param.FlagInitialConditionDensity){

////    setBoundaryVelocity (
////            lattice, lattice.getBoundingBox(),
////            InitialConditionVelocity<Type>(Param) );



////             plb::MultiScalarField2D<T> press(lattice.getComponent(iLevel));
////             poisson >> press;
////             std::auto_ptr<MultiTensorField2D<T,2> > vel = computeVelocity(lattice.getComponent(iLevel));
////             std::auto_ptr<MultiTensorField2D<T,3> > S = computeStrainRate(*vel);
////             recomposeFromFlowVariables(lattice.getComponent(iLevel), press, *vel, *S);

//    plb::plb_ifstream inDensityfile(Param.FilenameInitialConditionDensity.c_str());
//    plb::plb_ifstream inVelocityfile(Param.FilenameInitialConditionVelocity.c_str());

//    if (inDensityfile.is_open() ) {
//        plb::MultiScalarField2D<Type> Density(lattice);
//        inDensityfile >> Density;

//        std::auto_ptr<plb::MultiTensorField2D<Type,2> > vel = computeVelocity(lattice);
//        inVelocityfile >> *vel;

////        recomposeFromDensityFile(lattice, Density, Param);
////        recomposeFromFileFlowVariables(lattice,Density,Param);
//        recomposeFromFlowVariablesTest(lattice,Density,*vel,Param, lattice.getBoundingBox());


////        setBoundaryDensity (
////                     lattice, lattice.getBoundingBox(),
////                     InitialConditionDensityScalarField<Type>(Param, &Density) );
////        initializeAtEquilibrium (
////             lattice, lattice.getBoundingBox(),
////             InitialConditionVelocityAndDensityScalarField<Type>(Param, &Density) );
//      }


//}
//else {


//  if(Param.FlagExternalForce){

//      setBoundaryVelocity (
//              lattice, lattice.getBoundingBox(),
//              InitialConditionVelocity<Type>(Param) );
//      setBoundaryDensity (
//              lattice, lattice.getBoundingBox(),
//              InitialConditionDensity<Type>(Param) );

//      setExternalVector(lattice, lattice.getBoundingBox(),
//                        Descriptor<Type>::ExternalField::forceBeginsAt, InitialConditionForce<Type>(Param));

//      initializeAtEquilibrium (
//              lattice, lattice.getBoundingBox(),
//              InitialConditionVelocityAndDensity<Type>(Param) );
//  }
//  else
//  {
//      setBoundaryVelocity (
//              lattice, lattice.getBoundingBox(),
//              InitialConditionVelocity<Type>(Param) );
//      setBoundaryDensity (
//              lattice, lattice.getBoundingBox(),
//              InitialConditionDensity<Type>(Param) );

//      initializeAtEquilibrium (
//              lattice, lattice.getBoundingBox(),
//              InitialConditionVelocityAndDensity<Type>(Param) );
//    }
//}

}


#endif 
