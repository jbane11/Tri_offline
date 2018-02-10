#include "TRI_Tools.h" //Used for th RunNoChain to Tchain all of the TTrees together



/*This script will calculate the total amount of charge calulcated from the bcms.
To use this code you can either call with an argument of a csv TString run list (1,2,3,4,5) and a Int_t Boolean for cuts.
The value returned will be the total charge recoded by the BCM dnew calibrated by the BCM class.

Limitations/ issues
The code is limitted to one arm at a time. :(
*/

Double_t Total_Charge_runlist(TString Run_String="", Int_t cuts_bool=0){
	
	Double_t Total_Charge; //This will be the total amount of charge collected from the runs in Run_String;

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

//Values used in charge calculations
	Double_t dnew_ch_ev=0, unew_ch_ev=0;
	Double_t dnew_ch_total=0, unew_ch_total=0;
	Double_t dnew_cur_ev=0, unew_cur_ev=0;
	Double_t isrenewed=0; //Did the scaler record:

//Needed pointer for values in the TChain
	T->ResetBranchAddresses();
	T->SetBranchAddress(Form("%sBCM.charge_dnew",ARM.Data()),&dnew_ch_ev);
	T->SetBranchAddress(Form("%sBCM.charge_unew",ARM.Data()),&unew_ch_ev);
	T->SetBranchAddress(Form("%sBCM.current_dnew",ARM.Data()),&dnew_cur_ev);
	T->SetBranchAddress(Form("%sBCM.current_unew",ARM.Data()),&dnew_cur_ev);
	T->SetBranchAddress(Form("%sBCM.isrenewed",ARM.Data()),   &isrenewed);
	
	
	
//for loop to calculate the total charge;
	for(Int_t i=0;i<Total_entries;i++){
		T->GetEntry(i);
		//is T-event a new scaler event and if cuts are turned on makes a beam trip cut forcing at least 0.5 uAs
		if(isrenewed==1 && dnew_cur_ev >= cuts_bool*0.5){ 
			dnew_ch_total+=dnew_ch_ev;
			unew_ch_total+=unew_ch_ev;
		}//End of renewed if
		dnew_ch_ev=0, unew_ch_ev=0; //reset;
	}
// End of for loop of events in the tchain.    
    
    //cout <<"Dnew :" << dnew_ch_total << "|  Unew  :"<< unew_ch_total <<endl;
           
    Total_Charge=dnew_ch_total;
    
    
 
return Total_Charge;
}
