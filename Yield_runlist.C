#include "TRI_Tools.h" //Used for th RunNoChain to Tchain all of the TTrees together


void Yield_runlist(TString runlist=""){


//This block is used to gather a run list if one is not providee
	if(Run_String==""){
	cout << "Please enter the run list as a csv. example ..... 1,2,3,4 "<<endl; 
	cin >> Run_String;}

//Tchains the runlist
    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
	TChain* T =(TChain*) gGetTree(RunNoChain, "T");

//Limitation of the code: Which arm to choose!!
//Selecting the first run in the runlist and then using the 90000 cut off for run number tp select the arm.
	//Which arm
	TString ARM;
	if(RunNoChain[0]<90000){ARM="Left";	}
						else{ARM="Right";}

//Total number of events to loop through.	
	Int_t Total_entries = T->GetEntries();


	T->ResetBranchAddresses();

}



