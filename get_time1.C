double get_time1(TString ARM,Int_t cuts_bool=0){

	double time=0;
	
//Total number of events to loop through.	
	Int_t Total_entries1 = T->GetEntries();
	Double_t isrenewed1=0; //Did the scaler record:
	double time_ev=0.0;
	double time_ev_old=0.0;	
	Double_t scaler_diff=0.0;
	Double_t time_diff=0.0;
	Double_t inc_time=0.0;
	double total_time=0.0;
	double ttime[20]={0.0};
	int num=0;
	//T->SetBranchAddress(Form("%sBCM.isrenewed",ARM.Data()),&isrenewed1);
	T->ResetBranchAddresses();
	T->SetBranchStatus(Form("%sLclock",ARM.Data()),1);
	T->SetBranchAddress(Form("%sLclock",ARM.Data()),   &time_ev);

	for(Int_t j=0;j<Total_entries1;j++){
		T->GetEntry(j,0);	
		scaler_diff = time_ev-time_ev_old;
		if(scaler_diff > 0){time_diff =(scaler_diff/103700.00000)/60.0000000;}
		else{time_diff = 0;}
		inc_time = inc_time + time_diff;
		time_ev_old=time_ev;
		
		scaler_diff=0;		
		time_diff=0;
		
	}// end of for loop
	
	total_time += inc_time;

//	cout <<" end of run " << "  current time " <<time<< "  total time so far :" << total_time<<endl; 	
	
	return total_time;	
}

