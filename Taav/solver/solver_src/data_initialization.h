#ifndef DATA_INITIALIZATION_H
#define DATA_INITIALIZATION_H

#include "data_initialization.hh"



template<typename T, template<typename U> class Descriptor>
void recomposeFromFile ( plb::MultiBlockLattice2D<T,Descriptor>& lattice,
                                  plb::MultiScalarField2D<T>& density)

{
    std::vector<plb::AtomicBlock2D*> atomicBlocks(2);
    atomicBlocks[0] = &lattice;
    atomicBlocks[1] = &density;
//    atomicBlocks[2] = &velocity;
//    atomicBlocks[3] = &strainRate;
    applyProcessingFunctional( new plb::RecomposeFromFlowVariablesFunctional2D<T,Descriptor>,  lattice.getBoundingBox(),
                               atomicBlocks );


}




template<typename T, template<typename U> class Descriptor>
void recomposeFromDensityFile ( plb::MultiBlockLattice2D<T,Descriptor>& lattice,
                                plb::MultiScalarField2D<T>& density, Parameters& Param)

{
    std::vector<plb::MultiBlock2D*> multiBlocks(2);
    multiBlocks[0] = &lattice;
    multiBlocks[1] = &density;
//    atomicBlocks[2] = &velocity;
//    atomicBlocks[3] = &strainRate;

    std::vector<std::string> Velocity_str;
    Velocity_str.push_back(Param.velocityX.c_str());
    Velocity_str.push_back(Param.velocityY.c_str());

    applyProcessingFunctional( new plb::RecomposeFromFlowVariablesFunctional2D<T,Descriptor>,  lattice.getBoundingBox(),
                               multiBlocks );
//    applyProcessingFunctional( new RecomposeFromDensityFlowVariablesFunctional2D<T,Descriptor>(Velocity_str),  lattice.getBoundingBox(),
//                               multiBlocks );


}



template<typename T, template<typename U> class Descriptor>
void recomposeFromFileFlowVariables ( plb::BlockLattice2D<T,Descriptor>& lattice,
                                  plb::ScalarField2D<T>& density, plb::TensorField2D<T,2>& velocity,
                                  plb::TensorField2D<T,3>& strainRate, plb::Box2D domain )
{
    std::vector<plb::AtomicBlock2D*> atomicBlocks(4);
    atomicBlocks[0] = &lattice;
    atomicBlocks[1] = &density;
    atomicBlocks[2] = &velocity;
    atomicBlocks[3] = &strainRate;
    applyProcessingFunctional( new RecomposeFromFileFlowVariablesFunctional2D<T,Descriptor>, domain,
                               atomicBlocks );
}

template<typename T, template<typename U> class Descriptor>
void recomposeFromFileFlowVariables ( plb::MultiBlockLattice2D<T,Descriptor>& lattice,
                                  plb::MultiScalarField2D<T>& density, plb::MultiTensorField2D<T,2>& velocity,
                                  plb::MultiTensorField2D<T,3>& strainRate, plb::Box2D domain, Parameters& Param )
{
    std::vector<plb::MultiBlock2D*> multiBlocks(2);
    multiBlocks[0] = &lattice;
    multiBlocks[1] = &density;
//    multiBlocks[2] = &velocity;
//    multiBlocks[3] = &strainRate;
    std::vector<std::string> Velocity_str;
    Velocity_str.push_back(Param.velocityX.c_str());
    Velocity_str.push_back(Param.velocityY.c_str());
//    applyProcessingFunctional( new RecomposeFromFileFlowVariablesFunctional2D<T,Descriptor>(Velocity_str), domain,
//                               multiBlocks );
    applyProcessingFunctional( new RecomposeFromFileDensityFunctional2D<T,Descriptor>(Velocity_str), domain,
                               multiBlocks);

}

template<typename T, template<typename U> class Descriptor>
void recomposeFromFileFlowVariables ( plb::MultiBlockLattice2D<T,Descriptor>& lattice,
                                  plb::MultiScalarField2D<T>& density, plb::Box2D domain, Parameters& Param )
{
    std::vector<plb::MultiBlock2D*> multiBlocks(2);
    multiBlocks[0] = &lattice;
    multiBlocks[1] = &density;
//    multiBlocks[2] = &velocity;
//    multiBlocks[3] = &strainRate;
    std::vector<std::string> Velocity_str;
    Velocity_str.push_back(Param.velocityX.c_str());
    Velocity_str.push_back(Param.velocityY.c_str());
//    applyProcessingFunctional( new RecomposeFromFileFlowVariablesFunctional2D<T,Descriptor>(Velocity_str), domain,
//                               multiBlocks );
    applyProcessingFunctional( new RecomposeFromFileDensityFunctional2D<T,Descriptor>(Velocity_str), domain,
                               multiBlocks);

}


template<typename T, template<typename U> class Descriptor>
void recomposeFromFileFlowVariables ( plb::MultiBlockLattice2D<T,Descriptor>& lattice,
                                  plb::MultiScalarField2D<T>& density, plb::MultiTensorField2D<T,2>& velocity,
                                  plb::MultiTensorField2D<T,3>& strainRate, Parameters& Param )
{
    recomposeFromFileFlowVariables( lattice, density, velocity, strainRate, lattice.getBoundingBox(), Param);
}


template<typename T, template<typename U> class Descriptor>
void recomposeFromFileFlowVariables ( plb::MultiBlockLattice2D<T,Descriptor>& lattice,
                                  plb::MultiScalarField2D<T>& density, Parameters& Param )
{
    recomposeFromFileFlowVariables( lattice, density, lattice.getBoundingBox(), Param);
}




#endif // DATA_INITIALIZATION_H
