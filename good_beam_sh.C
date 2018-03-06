#include "inc.h"


using namespace std;
const double TG_Theta_Max = 0.04;//40mrad
const double TG_Theta_Min =-0.04;//40mrad
const double TG_Phi_Max = 0.030;//25mrad
const double TG_Phi_Min =-0.030;//25mrad
const double TG_Dp_Max = 0.04;//4%mrad
const double TG_Dp_Min =-0.04;//4%mrad
const double TG_VZ_Max = 0.100;//4%mrad
const double TG_VZ_Min =-0.100;//4%mrad
const double P0 = 3.100; //GeV/c
const double GC_Cut = 2000.;
const double EP_Cut = 0.80;
const double EP_Cut_R = 0.95;
const double EC2_Cut = 300; //L.prl2.e, R.sh.e
const int Main_Trigger_Left = 2;
const int Main_Trigger_Right = 5;


void good_beam_sh(int run=0, TString kin=""){
	clock_t t1,t2;

	//Allow for for no argument
	if(run==0){cout << "please enter the run number" <<endl;cin>>run;}
	t1 =clock();
/*
	TFile *file = new TFile(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d.root",RunNoChain[0]),"read");
	TTree *T;
	T = (TTree*)file->Get("T");
//Limitation of the code: Which arm to choose!!
//Selecting the first run in the runlist and then using the 90000 cut off for run number tp select the arm.
*/
					//Which arm
		TString ARM; TString Arm;
		if(run<90000){ARM="Left"; Arm ="L";	}
			 else{ARM="Right";Arm ="R"; }

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




	Double_t Vclkcnt; Double_t clkcnt; 
	Double_t dnew_r;
	T->ResetBranchAddresses();
	T->SetBranchAddress("V1495ClockCount",&Vclkcnt);
	T->SetBranchAddress("LeftLclock",&clkcnt);
	T->SetBranchAddress("evLeftdnew_r",&dnew_r);
	T->GetEntry(0);
	Double_t Vclkcnt0 = Vclkcnt;


///////////////////////////////////////////////////////////////////
	//Find the average dnew_r with beam on.
	TH1F *Hdnew_r = new TH1F("Hdnew_r","dnew_r",100000,0,100000);
	Double_t avg_dnew =0;
	for(int ijk=0;ijk<10;ijk++){
			T->Draw(Form("ev%sdnew_r>>Hdnew_r",ARM.Data()),Form("ev%sdnew_r>=%f",ARM.Data(),avg_dnew-avg_dnew*0.75),"goff");
			Double_t avg_dnew_n=Hdnew_r->GetMean();
		//	cout << ijk << " "<<avg_dnew_n <<endl;
			if(abs(avg_dnew - avg_dnew_n)/avg_dnew <=0.20){break;}
			avg_dnew=avg_dnew_n*0.9;
		}
///////////////////////////////////////////////////////////////////		

	//Total number of events to loop through.	
	Int_t Total_entries = T->GetEntries();
	T->GetEntry(Total_entries-1);
	Double_t clkcnt_l = Vclkcnt;
	int bins = (Vclkcnt/103700/60+10); 
	int maxb = Vclkcnt/103700/60+10;
	
	TLine *L_avg = new TLine(0,avg_dnew,maxb,avg_dnew);
	TCanvas *C = new TCanvas("C","Beam quality");
	TH2F *Hdr_Vclk = new TH2F("Hdr_Vclk","dnew rate:V clock count",bins,0,maxb,100000,0,100000); 
	T->Draw(Form("ev%sdnew_r:(V1495ClockCount-%1.1f)/103700/60>>Hdr_Vclk",ARM.Data(),Vclkcnt0),"","");
	Hdr_Vclk->GetXaxis()->SetTitle("Time from start of run(minutes)");
	Hdr_Vclk->GetYaxis()->SetTitle("dnew_r (scaler rate)");
	L_avg->Draw("same");
	L_avg->SetLineColor(2);
	
	TH2F *Hdr_Vclk_cut = new TH2F("Hdr_Vclk_cut","dnew rate:V clock count with cut",bins,0,maxb,100000,0,100000); 
	

	int new_event;
	int quality=0;
	Double_t time_up=0;
	
	TVector dnew_rate_v(1);
	
	//Make anew file and clone the tree.
	TFile *newfile = new TFile(Form("%s/bqual/beam_qual_%d.root",rootfilePath.Data(),run),"recreate");
	TTree *newtree = T->CloneTree();
	TBranch *Qaul_beamup = newtree->Branch("BeamQual.beamOn",&time_up,"BeamQual.beamOn/D");
	TBranch *Qaul_qual = newtree->Branch("BeamQual.quality",&quality,"BeamQual.quality/I");
		
	for(int i =0;i<Total_entries;i++){

		T->GetEntry(i);//Get the current event
				
		if(dnew_r>=avg_dnew){ //Check if the current is ok;
			double time0 = (Vclkcnt-Vclkcnt0)/103700.0/60.0;
		
			for(int k =0; k<50;k++){ //Walk back in events/time to see if the beam was on long enough;

				quality = k;
				new_event = i-k*25; 
				if(new_event <=0){break;}		//no more events
				T->GetEntry(new_event);//Get the current event
				if(dnew_r<avg_dnew){break;} 	// Not high enough current
				
				time_up=abs((Vclkcnt-Vclkcnt0)/103700.0/60.0 - time0);
//debug counter////////////////////////
	/*			if(i/1000==i/1000.0){
	cout << i <<"\t"<< k <<"\t"<< new_event;
	cout<<"\t"<< dnew_r<<"\t"<< time_up<<std::setw(4)<<" mins" <<endl;
				}
	*/			
////////////////////////////////////////				
			}//end of walk back;

	
		}//End of > avg	
	
	T->GetEntry(i);//back to current event
	Qaul_beamup->Fill(); Qaul_qual->Fill();
	//Fill if beam is up for 20 seconds;
	if(time_up>=0.30){
		Hdr_Vclk_cut->Fill((Vclkcnt-Vclkcnt0)/103700.0/60.0,dnew_r*1.2);
	}
	//counter
	if(i/5000==i/5000.0){cout <<"Entry "<< i << " ! " <<Total_entries-i<< " to GO!"<<endl;}
	
	//reset
	time_up=0;
	quality=0;
	
		
	}//end of entry loop;
		//TCanvas *C2 = new TCanvas("C2","With cut");
		Hdr_Vclk_cut->SetMarkerColor(2);
		Hdr_Vclk_cut->SetLineColor(2);
		Hdr_Vclk_cut->Draw("same");

		
		
		C->SetName("Dnew_time");
		C->Write();
		

		
		newfile->Write();
		
			TString cut_L = Form("DL.evtypebits>>%d&1",Main_Trigger_Left);
	TString cut_e_L = Form("DL.evtypebits>>%d&1 && L.tr.n==1 && L.cer.asum_c>%4.1f && (L.prl1.e+L.prl2.e)/(L.gold.p*1000.)>%3.2f",Main_Trigger_Left, GC_Cut, EP_Cut);
	TString cut_good_e_L = Form("DL.evtypebits>>%d&1 && L.tr.n==1 && L.cer.asum_c>%4.1f && (L.prl1.e+L.prl2.e)/(L.gold.p*1000.)>%3.2f && (L.prl2.e)>%3.2f && (rpl.z)>%4.3f && (rpl.z)<%4.3f && (L.tr.tg_ph)>%4.3f && (L.tr.tg_ph)<%4.3f && (L.tr.tg_th)>%4.3f && (L.tr.tg_th)<%4.3f && (L.tr.tg_dp)>%4.3f && (L.tr.tg_dp)<%4.3f && evLeftdnew_r>1e4",Main_Trigger_Left, GC_Cut, EP_Cut, EC2_Cut, TG_VZ_Min, TG_VZ_Max,TG_Phi_Min, TG_Phi_Max, TG_Theta_Min, TG_Theta_Max, TG_Dp_Min, TG_Dp_Max);

	TString cut_R = Form("DR.evtypebits>>%d&1",Main_Trigger_Right);
	TString cut_e_R = Form("DR.evtypebits>>%d&1 && R.tr.n==1 && R.cer.asum_c>%4.1f && (R.ps.e+R.sh.e)/(R.gold.p*1000.)>%3.2f",Main_Trigger_Right,GC_Cut,EP_Cut_R);
	
        TString cut_good_e_R = Form("DR.evtypebits>>%d&1 && R.tr.n==1 && R.cer.asum_c>%4.1f && (R.ps.e+R.sh.e)/(R.gold.p*1000.)>%3.2f && (R.sh.e)>%3.2f && (rpr.z)>%4.3f && (rpr.z)<%4.3f && (R.gold.ph)>%4.3f && (R.gold.ph)<%4.3f && (R.gold.th)>%4.3f && (R.gold.th)<%4.3f && (R.gold.dp)>%4.3f && (R.gold.dp)<%4.3f && evRightdnew_r>1e4",Main_Trigger_Right, GC_Cut, EP_Cut_R, EC2_Cut, TG_VZ_Min, TG_VZ_Max,TG_Phi_Min, TG_Phi_Max, TG_Theta_Min, TG_Theta_Max, TG_Dp_Min, TG_Dp_Max);

	TString cut = "", cut_e="", cut_good_e="", z_var="";
	int Main_Trigger = 0;
	if(Arm=="L"){
		z_var = "rpl.z";
		cut = cut_L;
		cut_e = cut_e_L;
		cut_good_e = cut_good_e_L;
		Main_Trigger = Main_Trigger_Left;
	}else if(Arm=="R"){
		z_var = "rpr.z";
		cut = cut_R;
		cut_e = cut_e_R;
		cut_good_e = cut_good_e_R;
		Main_Trigger = Main_Trigger_Right;
	}
	double B_up =0.35; 
	TString beamQual = Form("BeamQual.beamOn>=%4.3f",B_up);
	TString cut_good_Q = cut_good_e+"&&"+beamQual;


	cout<<"--- Good Electron Cut: "<< cut_good_e<<endl;
	int Total_Main= newtree->GetEntries(TCut(cut));
	int Total_Ele= newtree->GetEntries(TCut(cut_e));
	int Good_Ele= newtree->GetEntries(TCut(cut_good_e));
	int Good_Ele_Q= newtree->GetEntries(TCut(cut_good_Q));
	cout<<endl; 

	cout<<"--- Total Good Electrons from these runs are = "<<Good_Ele<<endl;
	cout << " Electrons after  : " << Good_Ele_Q<<endl;		
	
  	FILE* results =fopen (Form("./results_%s.txt",kin.Data()),"a"); 
	fprintf(results,"%d\t%d\t%d\n",run,Good_Ele,Good_Ele_Q);
  	fclose(results);

	t2=clock();
  	double time = (t2-t1)/CLOCKS_PER_SEC;
	cout << "This took " << time/60.0 << " minutes to run!"<<endl;	
	
	exit(1);	
}




