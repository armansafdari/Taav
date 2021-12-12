

if ( Param.LBMmodel.compare("BGK") == 0) {
    
    switch_flags_2( Param.LBMDiscriptor.compare("D2Q9") == 0 , Param.FlagExternal )
    {
	case flags_2(T,F):
	  pcout << "LBM model is  BGK and the scheme is D2Q9." <<endl;
	  FunctionBGK_D2Q9(Param);
	  break;
	case flags_2(T,T):
	  pcout << "LBM model is  BGK and the scheme is D2Q9 with external force." <<endl;
	  FunctionBGK_D2Q9_External_Force(Param);
	  break;
    }  
}

if ( Param.LBMmodel.compare("MRT") == 0) {
    
    switch_flags_2( Param.LBMDiscriptor.compare("D2Q9") == 0 , Param.FlagExternal )
    {
	case flags_2(T,F):
	  pcout << "LBM model is  MRT and the scheme is D2Q9." <<endl;
	  //FunctionMRT_D2Q9(Param);
	  break;
	case flags_2(T,T):
	  pcout << "LBM model is  MRT and the scheme is D2Q9 with external force." <<endl;
	  //FunctionMRT_D2Q9_External_Force(Param);
	  break;
    }  
}

if ( Param.LBMmodel.compare("BGKLES") == 0) {
    
    switch_flags_2( Param.LBMDiscriptor.compare("D2Q9") == 0 , Param.FlagExternal )
    {
	case flags_2(T,F):
	  pcout << "LBM model is  BGK_LES and the scheme is D2Q9." <<endl;
          FunctionBGKLES_D2Q9(Param);
	  break;
	case flags_2(T,T):
	  pcout << "LBM model is  BGK_LES and the scheme is D2Q9 with external force." <<endl;
	  FunctionBGKLES_D2Q9_External_Force(Param);
	  break;
    }  
}
