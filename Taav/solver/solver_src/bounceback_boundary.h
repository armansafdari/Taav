#ifndef BOUNCEBACK_BOUNDARY_H
#define BOUNCEBACK_BOUNDARY_H


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



template<typename T>
class shapeinsidedomainBouncaBack:public plb::DomainFunctional2D {
public:
    shapeinsidedomainBouncaBack(std::string Script_): Script(Script_){}

    virtual bool operator() (plb::plint iX, plb::plint iY) const {
        bool results =  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , Script.c_str()) ;
        return results;
    }

    virtual shapeinsidedomainBouncaBack<T>* clone() const {
        return new shapeinsidedomainBouncaBack<T> (*this);
    }
 private:
    std::string  Script;
};

template<typename T>
class shapeinsidedomainNoDynamics:public plb::DomainFunctional2D {
public:
    shapeinsidedomainNoDynamics(std::string Script_): Script(Script_){}

    virtual bool operator() (plb::plint iX, plb::plint iY) const {
        bool results =  Script_to_Number(static_cast<double>(iX) , static_cast<double>(iY) , Script.c_str()) ;
        return results;
    }

    virtual shapeinsidedomainNoDynamics<T>* clone() const {
        return new shapeinsidedomainNoDynamics<T> (*this);
    }
 private:
    std::string  Script;
};


template<typename T, template<typename U> class Descriptor,class BlockLatticeT>
void WallInsideDomainFunction(BlockLatticeT& lattice,
                              Parameters& Param){

    if (Param.ObjectsInsideDomainFlag){
        if (Param.LoadfilewallboundaryFlag){

            plb::MultiScalarField2D<bool> boolMask(Param.lx, Param.ly);
            plb::plb_ifstream ifile(Param.FilenameObjectsInsideDomain_xml.c_str());
            ifile >> boolMask;
            defineDynamics(lattice, boolMask, new plb::BounceBack<T,Descriptor>, true);

        }
        else{
             defineDynamics (
                     lattice, lattice.getBoundingBox(),
                     new shapeinsidedomainBouncaBack<double>(Param.WallBoundary_script.c_str()), new plb::BounceBack<T,Descriptor> );

             defineDynamics (
                     lattice, lattice.getBoundingBox(),
                     new shapeinsidedomainNoDynamics<double>(Param.NoDynamics_script.c_str()), new plb::NoDynamics<T,Descriptor> );
        }
    }
}




template<typename T, template<typename U> class Descriptor>
class MomentumExchangeObjects{

public:

    MomentumExchangeObjects(std::string NameMomentumExchangeObjects_,  std::string ScriptMomentumExchangeObjects_,plb::Array<T,Descriptor<T>::d> Force_):
        NameMomentumExchangeObjects(NameMomentumExchangeObjects_),ScriptMomentumExchangeObjects(ScriptMomentumExchangeObjects_), Force(Force_){ }

    plb::Array<T,Descriptor<T>::d> GetForces(){
        return Force;
    }
    std::string GetName(){
        return NameMomentumExchangeObjects.c_str();
    }
    std::string GetScript(){
         return NameMomentumExchangeObjects.c_str();
    }

private:
std::string NameMomentumExchangeObjects;
std::string ScriptMomentumExchangeObjects;
plb::Array<T,Descriptor<T>::d> Force;

};





template<typename T, template<typename U> class Descriptor,class BlockLatticeT,class OB>
void MomentumExchangeInsideDomain(BlockLatticeT& lattice,
                              Parameters& Param, OB& Objects){

    Param.MomentumObjectsName.erase( std::remove(Param.MomentumObjectsName.begin(), Param.MomentumObjectsName.end(), ' '), Param.MomentumObjectsName.end());

    std::vector<std::string> ObjectsNamesTmp = split (Param.MomentumObjectsName, ";;");
    std::vector<std::string> ObjectsScript = split (Param.MomentumObjectsScript, ";;");
    std::vector<std::string> ObjectsNames;
    for (uint i=0 ; i < ObjectsScript.size(); ++i) {
        ObjectsNames.push_back("unnamed"+ Number_to_string(i+1));
    }
    for (uint i=0 ; i < ObjectsNamesTmp.size(); ++i) {
        ObjectsNames[i]=ObjectsNamesTmp[i];
    }

    for (uint i=0 ; i < ObjectsScript.size(); ++i) {
        plb::Array<T,Descriptor<T>::d> ForceObj;
        for (plb::plint i=0; i<Descriptor<T>::d;++i){
            ForceObj[i] = lattice.internalStatSubscription().subscribeSum();
        }
    MomentumExchangeObjects<T, Descriptor>* Ob =  new MomentumExchangeObjects<T, Descriptor>(ObjectsNames[i].c_str(),ObjectsScript[i].c_str(),ForceObj);
    plb::pcout << "ObjectsNames " << i << " = " <<  ObjectsNames[i].c_str() << std::endl;
    plb::defineDynamics(lattice, lattice.getBoundingBox(),
                 new shapeinsidedomainBouncaBack<T>(ObjectsScript[i].c_str()),
                 new plb::MomentumExchangeBounceBack<T,Descriptor>(ForceObj));
    initializeMomentumExchange(lattice, lattice.getBoundingBox(), new shapeinsidedomainBouncaBack<T>(ObjectsScript[i].c_str()) );
    Objects.push_back(Ob);

    }
}



#endif // BOUNCEBACK_BOUNDARY_H
