#ifndef INITIAL_CONDITION_HH
#define INITIAL_CONDITION_HH


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



double velocityFunctionX(plb::plint iX , plb::plint iY ,Parameters Param){
    return  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , Param.velocityX.c_str()) ;
}

double velocityFunctionY(plb::plint iX , plb::plint iY ,Parameters Param){

    return  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , Param.velocityY.c_str());;

}

double DensityFunction(plb::plint iX , plb::plint iY ,Parameters Param){

    return  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , Param.Density.c_str());;

}

double ForceFunctionX(plb::plint iX , plb::plint iY ,Parameters Param){

    return  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , Param.ForceX.c_str());;

}

double ForceFunctionY(plb::plint iX , plb::plint iY ,Parameters Param){

    return  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , Param.ForceY.c_str());;
}


double velocityFunctionX(plb::plint iX , plb::plint iY ,std::string velocityX){
    return  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , velocityX.c_str()) ;
}

double velocityFunctionY(plb::plint iX , plb::plint iY ,std::string velocityY){

    return  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , velocityY.c_str());;

}

double DensityFunction(plb::plint iX , plb::plint iY ,std::string Density){

    return  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , Density.c_str());;

}

double ForceFunctionX(plb::plint iX , plb::plint iY ,std::string ForceX){

    return  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , ForceX.c_str());;

}

double ForceFunctionY(plb::plint iX , plb::plint iY ,std::string ForceY){

    return  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , ForceY.c_str());;
}


/// A functional, used to initialize the velocity for the boundary conditions
template<typename T>
class InitialConditionVelocity {
public:
    InitialConditionVelocity(Parameters param_)
        : Param(param_)
    { }
    void operator()(plb::plint iX, plb::plint iY, plb::Array<T,2>& u) const {

        u[0] = velocityFunctionX(iX, iY, Param);
        u[1] = velocityFunctionY(iX, iY, Param);
    }
private:
    Parameters Param;

};

template<typename T, template<typename U> class Descriptor>
class InitialConditionVelocityTensorField {
public:
    InitialConditionVelocityTensorField(Parameters param_,plb::TensorField2D<T,Descriptor<T>::d>* Velocity_)
        : Param(param_),
          Velocity(Velocity_)
    { }
    void operator()(plb::plint iX, plb::plint iY, plb::Array<T,2>& u) const {
        u = Velocity->get(iX,iY);
    }
private:
    Parameters Param;
    plb::TensorField2D<T,Descriptor<T>::d>* Velocity;
};


/// A functional, used to initialize a density
template<typename T>
class InitialConditionDensity {
public:
    InitialConditionDensity(Parameters param_)
        : Param(param_)
    { }
    T operator()(plb::plint iX, plb::plint iY) const {
        return DensityFunction(iX,iY,Param);
    }
private:
    Parameters Param;
};



template<typename T>
class InitialConditionDensityScalarField {
public:
    InitialConditionDensityScalarField(Parameters param_,plb::MultiScalarField2D<T>* Density_)
        : Param(param_),
          Density(Density_)
    { }
    T operator()(plb::plint iX, plb::plint iY) const {
       return Density->get(iX,iY);

    }
private:
    Parameters Param;
    plb::MultiScalarField2D<T>* Density;
};


/// A functional, used to initialize the velocity for the boundary conditions
template<typename T>
class InitialConditionForce {
public:
    InitialConditionForce(Parameters& param_)
        : Param(param_)
    { }
    void operator()(plb::plint iX, plb::plint iY, plb::Array<T,2>& Force) const {

        Force[0] = ForceFunctionX(iX, iY, Param);
        Force[1] = ForceFunctionY(iX, iY, Param);
    }
private:
    Parameters Param;

};

//template<typename T, template<typename U> class Descriptor>
//class InitialConditionDensityScalarField:public plb::OneCellIndexedFunctional2D<T,Descriptor> {
//public:
//    InitialConditionDensityScalarField(Parameters param_,plb::MultiScalarField2D<T>* Density_)
//        : Param(param_),
//          Density(Density_)
//    { }
//    InitialConditionDensityScalarField<T,Descriptor>* clone() const {
//        return new InitialConditionDensityScalarField<T,Descriptor>(*this);
//    }
//    virtual void execute(plb::plint iX, plb::plint iY, plb::Cell<T,Descriptor>& cell) const {
//        T rho = Density->get(iX,iY);
//        plb::Array<T,2> InitialVelocity (0.,0.);
//        InitialVelocity[0] = velocityFunctionX(iX, iY, Param);
//        InitialVelocity[1] = velocityFunctionY(iX, iY, Param);

//        plb::iniCellAtEquilibrium(cell, rho, InitialVelocity);
//    }
//private:
//    Parameters Param;
//    plb::MultiScalarField2D<T>* Density;
//};


///////////////////////////////////////////////////////

template<typename T, template<typename U> class Descriptor>
class RandomInitializer : public plb::OneCellIndexedFunctional2D<T,Descriptor> {
public:
    RandomInitializer(plb::plint nx_, plb::plint ny_, T Rho_abc_ )
        : nx(nx_),
          ny(ny_),
          Rho_abc(Rho_abc_)
    { }
    RandomInitializer<T,Descriptor>* clone() const {
        return new RandomInitializer<T,Descriptor>(*this);
    }
    virtual void execute(plb::plint iX, plb::plint iY, plb::Cell<T,Descriptor>& cell) const {

        T rho = 0;
        srand(ny*iX + iY+0*(nx*ny)+1);
        T r = 0.0001 *(2*((double)rand())/(double)RAND_MAX-1);
        rho = Rho_abc *(1+r);

        plb::Array<T,2> InitialVelocity (0.,0.);

        iniCellAtEquilibrium(cell, rho, InitialVelocity);
    }
private:
    plb::plint nx;
    plb::plint ny;
    T Rho_abc;
};




/// A functional, used to create an initial condition for the density and velocity
template<typename T>
class InitialConditionVelocityAndDensity {
public:
    InitialConditionVelocityAndDensity(Parameters param_)
        : Param(param_)
    { }
    void operator()(plb::plint iX, plb::plint iY, T& rho, plb::Array<T,2>& u) const {
        rho = DensityFunction(iX,iY,Param);
        u[0] = velocityFunctionX(iX, iY , Param);
        u[1] = velocityFunctionY(iX, iY , Param);
    }
private:
    Parameters Param;
};

template<typename T>
class InitialConditionVelocityAndDensityScalarField {
public:
    InitialConditionVelocityAndDensityScalarField(Parameters param_,plb::MultiScalarField2D<T>* Density_)
        : Param(param_),
          Density(Density_)
    { }
    void operator()(plb::plint iX, plb::plint iY, T& rho, plb::Array<T,2>& u) const {
        rho  = Density->get(iX,iY);
        u[0] = velocityFunctionX(iX, iY , Param);
        u[1] = velocityFunctionY(iX, iY , Param);
    }
private:
    Parameters Param;
    plb::MultiScalarField2D<T>* Density;
};


template<typename T, template<typename U> class Descriptor>
class InitialConditionVelocityTensorFieldAndDensity {
public:
    InitialConditionVelocityTensorFieldAndDensity(Parameters param_,plb::TensorField2D<T,Descriptor<T>::d>* Velocity_)
        : Param(param_),
          Velocity(Velocity_)
    { }
    void operator()(plb::plint iX, plb::plint iY, T& rho, plb::Array<T,2>& u) const {
        rho  = DensityFunction(iX,iY,Param);
        u[0] = velocityFunctionX(iX, iY , Param);
        u[1] = velocityFunctionY(iX, iY , Param);
    }
private:
    Parameters Param;
    plb::TensorField2D<T,Descriptor<T>::d>* Velocity;
};


template<typename T>
class InitialConditionVelocityTensorFieldAndDensityScalarField {
public:
    InitialConditionVelocityTensorFieldAndDensityScalarField(Parameters param_)
        : Param(param_)
    { }
    void operator()(plb::plint iX, plb::plint iY, T& rho, plb::Array<T,2>& u) const {
        rho = DensityFunction(iX,iY,Param);
        u[0] = velocityFunctionX(iX, iY , Param);
        u[1] = velocityFunctionY(iX, iY , Param);
    }
private:
    Parameters Param;
};

#endif // INITIAL_CONDITION_HH
