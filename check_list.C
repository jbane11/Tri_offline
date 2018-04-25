#include "inc1.h"
#include "TRI_Main.h"
#include "TRI_Tools.h"
#include "THaRun.h"
#include "THaRunParameters.h"
#include "rootalias1.h"


void check_list(TString filename=""){
	
	/////input the name of the kin file and parse it
        if(filename==""){ cout<<"Input file name:   "; cin>>filename;}
        filename = "Runlist/" + filename;
////////////////////////////////////////////////////
        kin_file KinFile;
        KinFile.set_file(filename.Data(),0);
        TString tgt     =       TString::Format("%s",KinFile.target.c_str());
        TString kin     =       TString::Format("%s",KinFile.kin_num.c_str());
        TString Kin     =       TString::Format("kin%s",kin.Data());
        TString Run_String =    TString::Format("%s",KinFile.run_string.c_str());
        if(KinFile.run_file_status==0){cerr << "error @ Runlist"<<endl;return ;}
///Call tri_tools function to get the runs
  	const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
	FILE *file = fopen("missing_runs.txt","a");	
        for(int r = 0; r<RunNoChain.size();r++){
                int run=RunNoChain[r];
		TTree *T =LoadRun(run);
		if(T == nullptr) fprintf(file,"%d,",run);
		delete T;	
	}



}

