double get_time(TString ARM,Int_t cuts_bool=0){

	double time=0;
	
//Total number of events to loop through.	
	Int_t Total_entries1 = T->GetEntries();
	cout << Total_entries1 <<endl;
	Double_t isrenewed1=0; //Did the scaler record:
	Double_t time_ev=0, time_ev_old=0;	
	double total_time=0;
	double ttime[20]={0.0};
	int num=0;
	T->SetBranchAddress(Form("%sBCM.isrenewed",ARM.Data()),&isrenewed1);
	T->SetBranchAddress(Form("%sLclock",ARM.Data()),   &time_ev);
	
	
	for(Int_t j=0;j<100000;j++){
		T->GetEntry(j);	
		
		if(j/1000 == j/1000.00)cout <<j<<" "<< isrenewed1 << " " << time_ev <<"  time: "<<time<<"  to time: "<<total_time<<endl;
		
			if(isrenewed1==1){
			Double_t scaler_diff = time_ev-time_ev_old;
			if(scaler_diff < 0){
			//	ttime[num]=time;
			//	num++;
				total_time+=time; 
				cout<< j<<" " <<" end of run "<<"  current time "<<time<<"  total time so far :"<< total_time<<endl;
				}
			time+= (scaler_diff/103700.00000)/60.0000000;
			if(j/100 == j/100.00){cout << j << " "<<isrenewed1 << " "<< time_ev <<" "<<time_ev_old<< " "<<time<<endl;}		
			time_ev_old=time_ev;
		}//end of renewed	
		
	}// end of for loop
	
	total_time += time;

	cout <<" end of run " << "  current time " <<time<< "  total time so far :" << total_time<<endl; 	

	
	
	//for(int k=0;k<20;k++){if(ttime[k]==0.0){break;}
		//cout << k <<"  "<< ttime[k] <<endl;
	//}
	
	
	return total_time;	
}

