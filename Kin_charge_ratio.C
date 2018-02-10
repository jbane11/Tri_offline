
#include "TRI_Tools.h" //Used for th RunNoChain to Tchain all of the TTrees together

void Kin_charge_ratio( TString filename=""){


	//input the name opf the kin file and parse it
	if(filename==""){ cout<<"Input file name:   "; cin>>filename;}
	filename = "./Runlist/" + filename;
    ifstream file(filename.Data());
    if(!file.good()){cout << filename.Data() << "does not exist! "<<endl; exit(1);}
    TString content;
    TString Target,Kin,Run_String;
    const char* kin;
    for (int ii=0; content.ReadLine(file) && ii<3;ii++ ) {  
    	cout<<"!!!:  "<<content<<endl;
        if(ii==0)Target = content;
        if(ii==1){
           kin = content.Data();
           Kin = Form("Kin%s", kin);
         }
        if(ii==2)Run_String = content;         
      }
      file.close();
   ///////////////////////////////////////////////////
   
    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
	TChain* T =(TChain*) gGetTree(RunNoChain, "T");
   
   
	//gROOT->ProcessLine(".L Total_Charge_runlist.C");
	//Double_t Total_charge_rec = Total_Charge_runlist(Run_String,0);
	
	TString ARM;
	if(RunNoChain[0]<90000){ARM="Left";	}
						else{ARM="Right";}
	

	Double_t Total_charge_rec = gROOT->ProcessLine(Form(".x Total_Charge_kin.C(\"%s\",%d)",ARM.Data(),1));
	double time = gROOT->ProcessLine(Form(".x get_time1.C(\"%s\",%d)",ARM.Data(),0));	

//	cout <<"The Total Charge for " <<Target << " at "<< Kin << " :"<< Total_charge_rec <<endl;
	
	TH1F *h1 = new TH1F("h1","h1",300,1.0,31);
	T->Draw(Form("%sBCM.current_dnew>>h1",ARM.Data()),Form("%sBCM.isrenewed==1",ARM.Data()),"goff");

	Double_t sent_current = h1->GetBinCenter(h1->GetMaximumBin());
	Double_t request_cur;
	if(sent_current< 3){ request_cur=3;}
		else if(sent_current< 6){ request_cur=5;}
		else if(sent_current<11){  request_cur=10;}
		else if(sent_current<16){  request_cur=15;}
		else if(sent_current<21){  request_cur=20;}
		else{request_cur=22.5;}
//	cout << "The most freqent sent current " << sent_current <<endl;
	

//	cout << "The time of all the runs in this list : " <<time <<endl;

	cout << "Charge received : " << Total_charge_rec <<"  |  Charge request "<< (time*60.0)*request_cur << " in " << time<< " minutes "<<endl;
	
	cout << Kin.Data() <<" with target " << Target << " had a beam on efficency of " << Total_charge_rec/((time*60.0)*request_cur) <<endl;
	
exit(1);


}   
