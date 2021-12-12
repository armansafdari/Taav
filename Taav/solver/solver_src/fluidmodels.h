#ifndef FLUIDMODELS_H
#define FLUIDMODELS_H

#include "fluidmodels.hh"


void FluidModels(Parameters& Param){

if ( Param.LBMmodel.compare("BGK") == 0) {

    switch_flags_2( Param.LBMDiscriptor.compare("D2Q9") == 0 , Param.FlagExternalForce )
    {
        case flags_2(T,F):
        {
          plb::pcout << "LBM model is BGK and the scheme is D2Q9." <<std::endl;
          plb::MultiBlockLattice2D<double, plb::descriptors::D2Q9Descriptor> lattice (
                    Param.lx, Param.ly, new plb::BGKdynamics<double, plb::descriptors::D2Q9Descriptor>(1.0/Param.tau) );

          Function_D2Q9<double,plb::descriptors::D2Q9Descriptor>(lattice, Param);
          break;
        }
        case flags_2(T,T):
        {
          plb::pcout << "LBM model is BGK and the scheme is D2Q9 with external force." <<std::endl;
          plb::MultiBlockLattice2D<double, plb::descriptors::ForcedD2Q9Descriptor> lattice (
                    Param.lx, Param.ly, new plb::GuoExternalForceBGKdynamics<double, plb::descriptors::ForcedD2Q9Descriptor>(1.0/Param.tau) );
          Function_D2Q9<double,plb::descriptors::ForcedD2Q9Descriptor>(lattice, Param);
          break;
        }
    }
}

if ( Param.LBMmodel.compare("InBGK") == 0) {

    switch_flags_2( Param.LBMDiscriptor.compare("D2Q9") == 0 , Param.FlagExternalForce )
    {
        case flags_2(T,F):
        {
          plb::pcout << "LBM model is BGK and the scheme is D2Q9." <<std::endl;
          plb::MultiBlockLattice2D<double, plb::descriptors::D2Q9Descriptor> lattice (
                    Param.lx, Param.ly, new plb::IncBGKdynamics<double, plb::descriptors::D2Q9Descriptor>(1.0/Param.tau) );
          Function_D2Q9<double,plb::descriptors::D2Q9Descriptor>(lattice, Param);
          break;
        }
        case flags_2(T,T):
        {
          plb::pcout << "LBM model is BGK and the scheme is D2Q9 with external force." <<std::endl;
          plb::MultiBlockLattice2D<double, plb::descriptors::ForcedD2Q9Descriptor> lattice (
                    Param.lx, Param.ly, new plb::IncGuoExternalForceBGKdynamics<double, plb::descriptors::ForcedD2Q9Descriptor>(1.0/Param.tau) );
          Function_D2Q9<double,plb::descriptors::ForcedD2Q9Descriptor>(lattice, Param);
          break;
        }
    }
}



if ( Param.LBMmodel.compare("MRT") == 0) {

    switch_flags_2( Param.LBMDiscriptor.compare("D2Q9") == 0 , Param.FlagExternalForce )
    {
        case flags_2(T,F):
        {
          plb::pcout << "LBM model is BGK and the scheme is D2Q9." <<std::endl;
          plb::MultiBlockLattice2D<double, plb::descriptors::MRTD2Q9Descriptor> lattice (
                    Param.lx, Param.ly, new plb::MRTdynamics<double, plb::descriptors::MRTD2Q9Descriptor>(1.0/Param.tau) );
          Function_D2Q9<double,plb::descriptors::MRTD2Q9Descriptor>(lattice, Param);
          break;
        }
        case flags_2(T,T):
        {
          plb::pcout << "LBM model is BGK and the scheme is D2Q9 with external force." <<std::endl;
          plb::MultiBlockLattice2D<double, plb::descriptors::ForcedMRTD2Q9Descriptor> lattice (
                    Param.lx, Param.ly, new plb::GuoExternalForceMRTdynamics<double, plb::descriptors::ForcedMRTD2Q9Descriptor>(1.0/Param.tau) );
          Function_D2Q9<double,plb::descriptors::ForcedMRTD2Q9Descriptor>(lattice, Param);
          break;
        }
    }
}


if ( Param.LBMmodel.compare("BGKLES") == 0) {

    switch_flags_2( Param.LBMDiscriptor.compare("D2Q9") == 0 , Param.FlagExternalForce )
    {
        case flags_2(T,F):
        {
          plb::pcout << "LBM model is BGK and the scheme is D2Q9." <<std::endl;
          plb::MultiBlockLattice2D<double, plb::descriptors::D2Q9Descriptor> lattice (
                    Param.lx, Param.ly, new plb::SmagorinskyBGKdynamics<double, plb::descriptors::D2Q9Descriptor>(1.0/Param.tau, Param.cSmago) );

          Function_D2Q9<double,plb::descriptors::D2Q9Descriptor>(lattice, Param);
          break;
        }
        case flags_2(T,T):
        {
          plb::pcout << "LBM model is BGK and the scheme is D2Q9 with external force." <<std::endl;
          plb::MultiBlockLattice2D<double, plb::descriptors::ForcedD2Q9Descriptor> lattice (
                    Param.lx, Param.ly, new plb::GuoExternalForceSmagorinskyBGKdynamics<double, plb::descriptors::ForcedD2Q9Descriptor>(1.0/Param.tau, Param.cSmago) );
          Function_D2Q9<double,plb::descriptors::ForcedD2Q9Descriptor>(lattice, Param);
          break;
        }
    }
}


};



#endif // FLUIDMODELS_H
