

/*This script will calculate the total amount of charge calulcated from the bcms.
This code is called by Kin_charge_ratio
The value returned will be the total charge recoded by the BCM dnew calibrated by the BCM class.

Limitations/ issues
The code is limitted to one arm at a time. :(
*/

Double_t Total_Charge_kin(TString ARM,Int_t cuts_bool=0){
	
	Double_t Total_Charge=1; //This will be the total amount of charge collected from the runs in Run_String;


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



