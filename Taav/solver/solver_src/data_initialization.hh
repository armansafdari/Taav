#ifndef DATA_INITIALIZATION_HH
#define DATA_INITIALIZATION_HH

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
#include "initial_condition.hh"










///////////////////////////////////////////////////////////////////////////////////////////////




/* *************** Class RecomposeFromFlowVariablesFunctional2D ******************* */

template<typename T, template<typename U> class Descriptor>
class RecomposeFromFileDensityFunctional2D : public plb::BoxProcessingFunctional2D
{
public:
        RecomposeFromFileDensityFunctional2D(std::vector<std::string> Velocity_str_)
            : Velocity_str(Velocity_str_)
        { }
    virtual void processGenericBlocks(plb::Box2D domain,
                                      std::vector<plb::AtomicBlock2D*> atomicBlocks);
    virtual RecomposeFromFileDensityFunctional2D<T,Descriptor>* clone() const;
    virtual plb::BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<plb::modif::ModifT>& modified) const;
private:
    std::vector<std::string> Velocity_str;
};


template<typename T, template<typename U> class Descriptor>
void RecomposeFromFileDensityFunctional2D<T,Descriptor>::processGenericBlocks (
        plb::Box2D domain, std::vector<plb::AtomicBlock2D*> atomicBlocks )
{
    plb::BlockLattice2D<T,Descriptor>& lattice =
        dynamic_cast<plb::BlockLattice2D<T,Descriptor>&>(*atomicBlocks[0]);
   plb:: ScalarField2D<T> const& rhoField =
        dynamic_cast<plb::ScalarField2D<T> const&>(*atomicBlocks[1]);
//    plb::TensorField2D<T,2> const& uField =
//        dynamic_cast<plb::TensorField2D<T,2> const&>(*atomicBlocks[2]);
//    plb::TensorField2D<T,3> const& SField =
//        dynamic_cast<plb::TensorField2D<T,3> const&>(*atomicBlocks[3]);

   plb:: Dot2D offset1 = computeRelativeDisplacement(lattice, rhoField);
//    plb::Dot2D offset2 = computeRelativeDisplacement(lattice, uField);
//    plb::Dot2D offset3 = computeRelativeDisplacement(lattice, SField);

    std::vector<T> rawData(1+Descriptor<T>::d+plb::SymmetricTensor<T,Descriptor>::n
                           +Descriptor<T>::ExternalField::numScalars);

    for (plb::plint iX=domain.x0; iX<=domain.x1; ++iX) {
        for (plb::plint iY=domain.y0; iY<=domain.y1; ++iY) {
            plb::Cell<T,Descriptor>& cell = lattice.get(iX,iY);
            T rho               = rhoField.get(iX+offset1.x, iY+offset1.y);
            plb::Array<T,2>  u;
            u[0] = velocityFunctionX(iX+offset1.x, iY+offset1.y , Velocity_str[0]);
            u[1] = velocityFunctionY(iX+offset1.x, iY+offset1.y , Velocity_str[1]);

            // Convert rho --> rhoBar.
            rawData[0] = Descriptor<T>::rhoBar(rho);

            // Convert u --> j
            rawData[1] = rho*u[0];
            rawData[2] = rho*u[1];


            // Recompose the cell.
            plb::plint recomposeOrder = 0;
            cell.getDynamics().recompose(cell, rawData, recomposeOrder);
        }
    }
}

template<typename T, template<typename U> class Descriptor>
RecomposeFromFileDensityFunctional2D<T,Descriptor>*
    RecomposeFromFileDensityFunctional2D<T,Descriptor>::clone() const
{
    return new RecomposeFromFileDensityFunctional2D<T,Descriptor>(*this);
}

template<typename T, template<typename U> class Descriptor>
plb::BlockDomain::DomainT RecomposeFromFileDensityFunctional2D<T,Descriptor>::appliesTo() const
{
    // We could directly apply to the envelope too, but let's keep it
    //   bulk-only for future compatibility.
    return plb::BlockDomain::bulk;
}

template<typename T, template<typename U> class Descriptor>
void RecomposeFromFileDensityFunctional2D<T,Descriptor>::getTypeOfModification (
        std::vector<plb::modif::ModifT>& modified) const
{
    modified[0] = plb::modif::staticVariables;
    modified[1] = plb::modif::nothing;
    modified[2] = plb::modif::nothing;
//    modified[3] = plb::modif::nothing;
}

///////////////////////////////////////////////////////////////////////////////////////////////






/* *************** Class RecomposeFromFlowVariablesFunctional2D ******************* */

template<typename T, template<typename U> class Descriptor>
class recomposeFromFileFunctional2D : public plb::BoxProcessingFunctional2D
{
public:
    virtual void processGenericBlocks(plb::Box2D domain,
                                      std::vector<plb::AtomicBlock2D*> atomicBlocks);
    virtual recomposeFromFileFunctional2D<T,Descriptor>* clone() const;
    virtual plb::BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<plb::modif::ModifT>& modified) const;
};



/* ************* Class RecomposeFromFlowVariablesFunctional2D ******************* */

template<typename T, template<typename U> class Descriptor>
void recomposeFromFileFunctional2D<T,Descriptor>::processGenericBlocks (
        plb::Box2D domain, std::vector<plb::AtomicBlock2D*> atomicBlocks )
{
    plb::BlockLattice2D<T,Descriptor>& lattice =
        dynamic_cast<plb::BlockLattice2D<T,Descriptor>&>(*atomicBlocks[0]);
    plb::ScalarField2D<T> const& rhoField =
        dynamic_cast<plb::ScalarField2D<T> const&>(*atomicBlocks[1]);
//    TensorField2D<T,2> const& uField =
//        dynamic_cast<TensorField2D<T,2> const&>(*atomicBlocks[2]);
//    TensorField2D<T,3> const& SField =
//        dynamic_cast<TensorField2D<T,3> const&>(*atomicBlocks[3]);

    plb::Dot2D offset1 = computeRelativeDisplacement(lattice, rhoField);
//    Dot2D offset2 = computeRelativeDisplacement(lattice, uField);
//    Dot2D offset3 = computeRelativeDisplacement(lattice, SField);

    std::vector<T> rawData(1+Descriptor<T>::d+plb::SymmetricTensor<T,Descriptor>::n
                           +Descriptor<T>::ExternalField::numScalars);

    for (plb::plint iX=domain.x0; iX<=domain.x1; ++iX) {
        for (plb::plint iY=domain.y0; iY<=domain.y1; ++iY) {
            plb::Cell<T,Descriptor>& cell = lattice.get(iX,iY);
            T rho               = rhoField.get(iX+offset1.x, iY+offset1.y);
/*            Array<T,2> const& u = uField.get(iX+offset2.x, iY+offset2.y);
            Array<T,3> const& S = SField.get(iX+offset3.x, iY+offset3.y)*/;

            // Convert rho --> rhoBar.
            rawData[0] = Descriptor<T>::rhoBar(rho);

            // Convert u --> j
//            rawData[1] = rho*u[0];
//            rawData[2] = rho*u[1];

            // Convert S --> PiNeq.
//            T omega = cell.getDynamics().getOmega();
//            T prefactor = - Descriptor<T>::cs2 * rho * (T)2 / omega;
//            rawData[3] = S[0] * prefactor;
//            rawData[4] = S[1] * prefactor;
//            rawData[5] = S[2] * prefactor;

            // Recompose the cell.
            plb::plint recomposeOrder = 1;
            cell.getDynamics().recompose(cell, rawData, recomposeOrder);
        }
    }
}

template<typename T, template<typename U> class Descriptor>
recomposeFromFileFunctional2D<T,Descriptor>*
    recomposeFromFileFunctional2D<T,Descriptor>::clone() const
{
    return new recomposeFromFileFunctional2D<T,Descriptor>(*this);
}

template<typename T, template<typename U> class Descriptor>
plb::BlockDomain::DomainT recomposeFromFileFunctional2D<T,Descriptor>::appliesTo() const
{
    // We could directly apply to the envelope too, but let's keep it
    //   bulk-only for future compatibility.
    return plb::BlockDomain::bulk;
}

template<typename T, template<typename U> class Descriptor>
void recomposeFromFileFunctional2D<T,Descriptor>::getTypeOfModification (
        std::vector<plb::modif::ModifT>& modified) const
{
    modified[0] = plb::modif::staticVariables;
    modified[1] = plb::modif::nothing;
    modified[2] = plb::modif::nothing;
    modified[3] = plb::modif::nothing;
}



/* ************* Class RecomposeFromFlowVariablesFunctional2D ******************* */



///////////////////////////////////////////////////////////////////////////////////////////



/* *************** Class RecomposeFromFlowVariablesFunctional2D ******************* */

template<typename T, template<typename U> class Descriptor>
class RecomposeFromFileFlowVariablesFunctional2D : public plb::BoxProcessingFunctional2D
{
public:
        RecomposeFromFileFlowVariablesFunctional2D(std::vector<std::string> Velocity_str_)
            : Velocity_str(Velocity_str_)
        { }
    virtual void processGenericBlocks(plb::Box2D domain,
                                      std::vector<plb::AtomicBlock2D*> atomicBlocks);
    virtual RecomposeFromFileFlowVariablesFunctional2D<T,Descriptor>* clone() const;
    virtual plb::BlockDomain::DomainT appliesTo() const;
    virtual void getTypeOfModification(std::vector<plb::modif::ModifT>& modified) const;
private:
    std::vector<std::string> Velocity_str;
};


template<typename T, template<typename U> class Descriptor>
void RecomposeFromFileFlowVariablesFunctional2D<T,Descriptor>::processGenericBlocks (
        plb::Box2D domain, std::vector<plb::AtomicBlock2D*> atomicBlocks )
{
    plb::BlockLattice2D<T,Descriptor>& lattice =
        dynamic_cast<plb::BlockLattice2D<T,Descriptor>&>(*atomicBlocks[0]);
   plb:: ScalarField2D<T> const& rhoField =
        dynamic_cast<plb::ScalarField2D<T> const&>(*atomicBlocks[1]);
    plb::TensorField2D<T,2> const& uField =
        dynamic_cast<plb::TensorField2D<T,2> const&>(*atomicBlocks[2]);
    plb::TensorField2D<T,3> const& SField =
        dynamic_cast<plb::TensorField2D<T,3> const&>(*atomicBlocks[3]);

   plb:: Dot2D offset1 = computeRelativeDisplacement(lattice, rhoField);
    plb::Dot2D offset2 = computeRelativeDisplacement(lattice, uField);
    plb::Dot2D offset3 = computeRelativeDisplacement(lattice, SField);

    std::vector<T> rawData(1+Descriptor<T>::d+plb::SymmetricTensor<T,Descriptor>::n
                           +Descriptor<T>::ExternalField::numScalars);

    for (plb::plint iX=domain.x0; iX<=domain.x1; ++iX) {
        for (plb::plint iY=domain.y0; iY<=domain.y1; ++iY) {
            plb::Cell<T,Descriptor>& cell = lattice.get(iX,iY);
            T rho               = rhoField.get(iX+offset1.x, iY+offset1.y);
            plb::Array<T,2> const& u = uField.get(iX+offset2.x, iY+offset2.y);
            plb::Array<T,3> const& S = SField.get(iX+offset3.x, iY+offset3.y);

            // Convert rho --> rhoBar.
            rawData[0] = Descriptor<T>::rhoBar(rho);

            // Convert u --> j
            rawData[1] = rho*u[0];
            rawData[2] = rho*u[1];

            // Convert S --> PiNeq.
            T omega = cell.getDynamics().getOmega();
            T prefactor = - Descriptor<T>::cs2 * rho * (T)2 / omega;
            rawData[3] = S[0] * prefactor;
            rawData[4] = S[1] * prefactor;
            rawData[5] = S[2] * prefactor;

            // Recompose the cell.
            plb::plint recomposeOrder = 1;
            cell.getDynamics().recompose(cell, rawData, recomposeOrder);
        }
    }
}

template<typename T, template<typename U> class Descriptor>
RecomposeFromFileFlowVariablesFunctional2D<T,Descriptor>*
    RecomposeFromFileFlowVariablesFunctional2D<T,Descriptor>::clone() const
{
    return new RecomposeFromFileFlowVariablesFunctional2D<T,Descriptor>(*this);
}

template<typename T, template<typename U> class Descriptor>
plb::BlockDomain::DomainT RecomposeFromFileFlowVariablesFunctional2D<T,Descriptor>::appliesTo() const
{
    // We could directly apply to the envelope too, but let's keep it
    //   bulk-only for future compatibility.
    return plb::BlockDomain::bulk;
}

template<typename T, template<typename U> class Descriptor>
void RecomposeFromFileFlowVariablesFunctional2D<T,Descriptor>::getTypeOfModification (
        std::vector<plb::modif::ModifT>& modified) const
{
    modified[0] = plb::modif::staticVariables;
    modified[1] = plb::modif::nothing;
    modified[2] = plb::modif::nothing;
    modified[3] = plb::modif::nothing;
}


#endif // DATA_INITIALIZATION_HH
