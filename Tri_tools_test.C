#include "TRI_Tools.h"

void Tri_tools_test(TString Run_String=""){


	if(Run_String==""){
	cout << "Please enter e run list as a csv. example ..... 1,2,3,4 "<<endl; 
	cin >> Run_String;}

    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);

	TChain* T =(TChain*) gGetTree(RunNoChain, "T");
	
   
    
    
    
    
    
    
    
    
    
    
}
