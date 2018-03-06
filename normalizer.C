#include "TRI_Main.h" //Used for th RunNoChain to Tchain all of the TTrees together
#include "TRI_Tools.h" //Used for th RunNoChain to Tchain all of the TTrees together


void normalizer(int run=0){

	//Allow for for no argument
	if(run==0){cout << "please enter the run number" <<endl;cin>>run;}
/*
	TFile *file = new TFile(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d.root",RunNoChain[0]),"read");
	TTree *T;
	T = (TTree*)file->Get("T");
//Limitation of the code: Which arm to choose!!
//Selecting the first run in the runlist and then using the 90000 cut off for run number tp select the arm.
*/
	//Which arm
	TString ARM;
	if(run<90000){ARM="Left";	}
			 else{ARM="Right";}


TString rootfilePath="/chafs1/work1/tritium/Rootfiles/";
 	
 	//==========this to take care the split rootfiles========================
   	TString file_name1 = TString::Format("%stritium_%d.root",rootfilePath.Data(),run);
	TString basename1 = TString::Format("%stritium_%d",rootfilePath.Data(),run);
	TString rootfile1 = basename1 + ".root";
    TChain* T = new TChain("T");
    Long_t jk=0;
    while ( !gSystem->AccessPathName(rootfile1.Data()) ) {
        T->Add(rootfile1.Data());
        cout << "ROOT file " << run<< "_"<< jk << " added to TChain." << endl; jk++;
        rootfile1 = basename1 + "_" + jk + ".root"; 
       // if(i>10){break;}
        }



   TFile *file = new TFile(Form("%stritium_%d.root",rootfilePath.Data(),run),"read");
  if(file->IsZombie()){
    cout<<" this rootfile doest not exist: "<<endl;
    cout<<"Please try again with a new run. "<<endl;
    return;
  }


	TH1F *Hd_ch_ev = new TH1F("Hd_ch_ev","Charge for dnew from fast scalers",100,-1,39);
	TH1F *Hu_ch_ev = new TH1F("Hu_ch_ev","Charge for unew from fast scalers",100,-1,39);
	TH1F *Hd_ch = new TH1F("Hd_ch","Charge for dnew from slow scalers",300,-1,299);
	TH1F *Hu_ch = new TH1F("Hu_ch","Charge for unew from slow scalers",300,-1,299);
	
	TH1F *Hd_cur_ev = new TH1F("Hd_cur_ev","Current for dnew from fast scalers",100,-1,25);
	TH1F *Hu_cur_ev = new TH1F("Hu_cur_ev","Current for unew from fast scalers",100,-1,25);
	TH1F *Hd_cur = new TH1F("Hd_cur","Current for dnew from slow scalers",100,-1,25);
	TH1F *Hu_cur = new TH1F("Hu_cur","Current for unew from slow scalers",100,-1,25);

	TCanvas *C = new TCanvas("C","Normalization");
	C->Divide(2,2);	
	C->cd(1);
	T->Draw(Form("%sBCMev.charge_dnew>>Hd_ch_ev",ARM.Data()),Form("%sBCMev.isrenewed",ARM.Data()),"");
	Hd_ch_ev->SetTitle("Charge from dnew and unew, from ev scalers.");
	Hd_ch_ev->GetXaxis()->SetTitle("Charge per scaler  event (uC) ");
	Hu_ch_ev->SetLineColor(2);
	T->Draw(Form("%sBCMev.charge_unew>>Hu_ch_ev",ARM.Data()),Form("%sBCMev.isrenewed",ARM.Data()),"same");
	
	C->cd(2);
	T->Draw(Form("%sBCM.charge_dnew>>Hd_ch",ARM.Data()),Form("%sBCM.isrenewed",ARM.Data()),"");
	Hd_ch->SetTitle("Charge from dnew and unew, from slow scalers.");
	Hd_ch->GetXaxis()->SetTitle("Charge per scaler  event (uC) ");
	Hu_ch->SetLineColor(2);
	T->Draw(Form("%sBCM.charge_unew>>Hu_ch",ARM.Data()),Form("%sBCM.isrenewed",ARM.Data()),"same");
	
	C->cd(3);
	T->Draw(Form("%sBCMev.current_dnew>>Hd_cur_ev",ARM.Data()),Form("%sBCMev.isrenewed",ARM.Data()),"");
	Hd_cur_ev->SetTitle("Current from dnew and unew, from ev scalers.");
	Hd_cur_ev->GetXaxis()->SetTitle("Current per scaler  event (uA) ");
	Hu_cur_ev->SetLineColor(2);
	T->Draw(Form("%sBCMev.current_unew>>Hu_cur_ev",ARM.Data()),Form("%sBCMev.isrenewed",ARM.Data()),"same");
	
	C->cd(4);
	T->Draw(Form("%sBCM.current_dnew>>Hd_cur",ARM.Data()),Form("%sBCM.isrenewed",ARM.Data()),"");
	Hd_cur->SetTitle("Current from dnew and unew, from slow scalers.");
	Hd_cur->GetXaxis()->SetTitle("Current per scaler  event (uA) ");
	Hu_cur->SetLineColor(2);	
	T->Draw(Form("%sBCM.current_unew>>Hu_cur",ARM.Data()),Form("%sBCM.isrenewed",ARM.Data()),"same");


	TH1F *H1d_ch_ev = new TH1F("H1d_ch_ev","Charge for dnew from fast scalers",100,-1,39);
	TH1F *H1u_ch_ev = new TH1F("H1u_ch_ev","Charge for unew from fast scalers",100,-1,39);
	TH1F *H1d_ch = new TH1F("H1d_ch","Charge for dnew from slow scalers",300,-1,299);
	TH1F *H1u_ch = new TH1F("H1u_ch","Charge for unew from slow scalers",300,-1,299);
	
	TH1F *H1d_cur_ev = new TH1F("H1d_cur_ev","Current for dnew from fast scalers",100,-1,25);
	TH1F *H1u_cur_ev = new TH1F("H1u_cur_ev","Current for unew from fast scalers",100,-1,25);
	TH1F *H1d_cur = new TH1F("H1d_cur","Current for dnew from slow scalers",100,-1,25);
	TH1F *H1u_cur = new TH1F("H1u_cur","Current for unew from slow scalers",100,-1,25);


	Double_t trip_cur = 20.0 ;//Value for the minimum accaptable current.

	TCanvas *C1 = new TCanvas("C1","Normalization with Cut");
	C1->Divide(2,2);	
	C1->cd(1);
	T->Draw(Form("%sBCMev.charge_dnew>>H1d_ch_ev",ARM.Data()),Form("%sBCMev.isrenewed&&%sBCMev.current_dnew>%0.1f",ARM.Data(),ARM.Data(),trip_cur),"");
	H1d_ch_ev->SetTitle("Charge from dnew and unew, from ev scalers.");
	H1d_ch_ev->GetXaxis()->SetTitle("Charge per scaler  event (uC) ");
	H1u_ch_ev->SetLineColor(2);
	T->Draw(Form("%sBCMev.charge_unew>>H1u_ch_ev",ARM.Data()),Form("%sBCMev.isrenewed&&%sBCMev.current_dnew>%0.1f",ARM.Data(),ARM.Data(),trip_cur),"same");
	
	C1->cd(2);
	T->Draw(Form("%sBCM.charge_dnew>>H1d_ch",ARM.Data()),Form("%sBCM.isrenewed&&%sBCM.current_dnew>%0.1f",ARM.Data(),ARM.Data(),trip_cur),"");
	H1d_ch->SetTitle("Charge from dnew and unew, from slow scalers.");
	H1d_ch->GetXaxis()->SetTitle("Charge per scaler  event (uC) ");
	H1u_ch->SetLineColor(2);
	T->Draw(Form("%sBCM.charge_unew>>H1u_ch",ARM.Data()),Form("%sBCM.isrenewed&&%sBCM.current_dnew>%0.1f",ARM.Data(),ARM.Data(),trip_cur),"same");
	
	C1->cd(3);
	T->Draw(Form("%sBCMev.current_dnew>>H1d_cur_ev",ARM.Data()),Form("%sBCMev.isrenewed&&%sBCMev.current_dnew>%0.1f",ARM.Data(),ARM.Data(),trip_cur),"");
	H1d_cur_ev->SetTitle("Current from dnew and unew, from ev scalers.");
	H1d_cur_ev->GetXaxis()->SetTitle("Current per scaler  event (uA) ");
	H1u_cur_ev->SetLineColor(2);
	T->Draw(Form("%sBCMev.current_unew>>H1u_cur_ev",ARM.Data()),Form("%sBCMev.isrenewed&&%sBCMev.current_dnew>%0.1f",ARM.Data(), ARM.Data(),trip_cur),"same");
	
	C1->cd(4);
	T->Draw(Form("%sBCM.current_dnew>>H1d_cur",ARM.Data()),Form("%sBCM.isrenewed&&%sBCM.current_dnew>%0.1f",ARM.Data(),ARM.Data(),trip_cur),"");
	H1d_cur->SetTitle("Current from dnew and unew, from slow scalers.");
	H1d_cur->GetXaxis()->SetTitle("Current per scaler event (uA) ");
	H1u_cur->SetLineColor(2);	
	T->Draw(Form("%sBCM.current_unew>>H1u_cur",ARM.Data()),Form("%sBCM.isrenewed&&%sBCM.current_dnew>%0.1f " ,ARM.Data(),ARM.Data(),trip_cur),"same");


//Total number of events to loop through.	
	Int_t Total_entries = T->GetEntries();

//Values used in for branch address
	Double_t dnew_cur_ev=0, unew_cur_ev=0;
	Double_t dnew_cur=0, unew_cur=0;
	Double_t dnew_ch_ev=0, unew_ch_ev=0;
	Double_t dnew_ch=0, unew_ch=0;	
	Double_t isrenewed=0; //Did the scaler record:
	Double_t isrenewed_ev=0; //Did the scaler record:

//Needed pointer for values in the TChain
	T->ResetBranchAddresses();
	T->SetBranchAddress(Form("%sBCMev.charge_dnew",ARM.Data()),&dnew_ch_ev);
	T->SetBranchAddress(Form("%sBCMev.charge_unew",ARM.Data()),&unew_ch_ev);
	T->SetBranchAddress(Form("%sBCMev.current_dnew",ARM.Data()),&dnew_cur_ev);
	T->SetBranchAddress(Form("%sBCMev.current_unew",ARM.Data()),&dnew_cur_ev);
	T->SetBranchAddress(Form("%sBCMev.isrenewed",ARM.Data()),   &isrenewed_ev);
	
	T->SetBranchAddress(Form("%sBCM.charge_dnew",ARM.Data()),&dnew_ch);
	T->SetBranchAddress(Form("%sBCM.charge_unew",ARM.Data()),&unew_ch);
	T->SetBranchAddress(Form("%sBCM.current_dnew",ARM.Data()),&dnew_cur);
	T->SetBranchAddress(Form("%sBCM.current_unew",ARM.Data()),&dnew_cur);
	T->SetBranchAddress(Form("%sBCM.isrenewed",ARM.Data()),   &isrenewed);

//Varibles needed for charge calculation	
	Double_t dnew_ch_total=0, unew_ch_total=0;
	Double_t dnew_ch_ev_total=0, unew_ch_ev_total=0;
	

	
	
//for loop to calculate the total charge;
	for(Int_t i=0;i<Total_entries;i++){
		T->GetEntry(i);



		if((isrenewed==1 && ((dnew_cur)/1.0) >= trip_cur)){ 
		
		//cout << dnew_cur << " "<< dnew_ch<<endl;
			dnew_ch_total+=dnew_ch;
			unew_ch_total+=unew_ch;
		}//End of renewed if
		
		if(isrenewed_ev==1 && (dnew_cur_ev >= trip_cur)){ 
			dnew_ch_ev_total+=dnew_ch_ev;
			unew_ch_ev_total+=unew_ch_ev;
		}//End of renewed if
		dnew_ch_ev=0, unew_ch_ev=0; //reset;
		dnew_ch=0, unew_ch=0; //reset;
	}
// End of for loop of events in the tchain. 

	cout << "Total charge for run :" <<run <<endl;
	cout <<"\t\t" <<"dnew \t"<< " unew"<<endl;
	cout << "Fast \t\t" << dnew_ch_ev_total << "\t"<< unew_ch_ev_total <<endl;
	cout << "Slow \t\t" << dnew_ch_total << "\t"<< unew_ch_total <<endl;


	
}
