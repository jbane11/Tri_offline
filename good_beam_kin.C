#include "inc.h"
#include "TRI_Main.h" //Used for th RunNoChain to Tchain all of the TTrees together
#include "TRI_Tools.h" //Used for th RunNoChain to Tchain all of the TTrees together


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


void good_beam_kin(TString filename=""){
	clock_t t1,t2;

/////input the name of the kin file and parse it
	if(filename==""){ cout<<"Input file name:   "; cin>>filename;}
	filename = "/adaqfs/home/a-onl/tritium_work/Runlist/" + filename;
    ifstream file(filename.Data());
    	t1 =clock();
    if(!file.good()){cout << filename.Data() << " does not exist! "<<endl; exit(1);}
    TString content;
    TString Target,Kin,Run_String;
    
    TString kin;
    for (int ii=0; content.ReadLine(file) && ii<3;ii++ ) {  
    	cout<<"!!!:  "<<content<<endl;
        if(ii==0)Target = content;
        if(ii==1){
           kin = content.Data();
           Kin = Form("Kin%s", kin.Data());
         }
        if(ii==2)Run_String = content;         
      }
      file.close();
////////////////////////////////////////////////////////

    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);

	TString rootfilePath="/chafs1/work1/tritium/Rootfiles/";
	Int_t kin_num = kin.Atoi();
	if(kin_num<=7){
		rootfilePath = "/volatile/halla/triton/Tritium_Rootfiles/";}
//////////////////////////////////////////////////////////

	Int_t num_of_runs=0; // counter for runs..
	int first_run = RunNoChain[0];
	int last_run = RunNoChain[RunNoChain.size()-1];

	TVector Total_Main(1);	TVector Total_Ele(1);		
	TVector Good_Ele(1);	TVector Good_Ele_Q(1);
	
	Int_t Main =0;
	Int_t Good_Ele_total=0;
	Int_t Total_Ele_total=0;
	Int_t Total_Good_Ele_Q=0;
		
	TCanvas *C[RunNoChain.size()-1];
	TChain* T;
	clock_t CClock[RunNoChain.size()-1];
	CClock[0]=clock();
////////////////////////////////////////////////////////////
//Start of each run
	for(int i = 0; i<RunNoChain.size()-1;i++){
		int run=RunNoChain[i]; 
		cout << "This is the start of run " << run <<endl;		
		TFile *newfile;	TTree *newtree ; 		
			
					//Which arm
		TString ARM; TString Arm;
		if(run<90000){ARM="Left"; Arm ="L";	}
			 else{ARM="Right";Arm ="R"; }
 	
//==========this to take care the split rootfiles========================
   		TString file_name1 = TString::Format("%stritium_%d.root",rootfilePath.Data(),run);
		TString basename1 = TString::Format("%stritium_%d",rootfilePath.Data(),run);
		TString rootfile1 = basename1 + ".root";
	    T = new TChain("T");
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
		    continue;
  		}
  		file->Close();
  		delete file;
///////////////////////////////////////////////////////////////////////////////////
//Resize vectors
		Total_Main.ResizeTo(num_of_runs+1);	
		Total_Ele.ResizeTo(num_of_runs+1);		
		Good_Ele.ResizeTo(num_of_runs+1);
		Good_Ele_Q.ResizeTo(num_of_runs+1);
////Varibles and branches of orginal root file
		Double_t Vclkcnt; Double_t clkcnt; 
		Double_t dnew_r;
		T->ResetBranchAddresses();
		T->SetBranchAddress("V1495ClockCount",&Vclkcnt);
		T->SetBranchAddress("LeftLclock",&clkcnt);
		T->SetBranchAddress("evLeftdnew_r",&dnew_r);
		T->GetEntry(0);
		Double_t Vclkcnt0 = Vclkcnt; //First event for the clock

///////////////////////////////////////////////////////////////////
//Find the average dnew_r with beam on.
		TH1F *Hdnew_r = new TH1F("Hdnew_r","dnew_r",100000,0,100000);
		Double_t avg_dnew =0;
		for(int ijk=0;ijk<10;ijk++){
			T->Draw(Form("ev%sdnew_r>>Hdnew_r",ARM.Data()),Form("ev%sdnew_r>=%f",ARM.Data(),avg_dnew-avg_dnew*0.75),"goff");
			Double_t avg_dnew_n=Hdnew_r->GetMean();
			//cout << ijk << " "<<avg_dnew_n <<endl;
			if(abs(avg_dnew - avg_dnew_n)/avg_dnew <=0.10){break;}
			avg_dnew=avg_dnew_n;
		}
		delete Hdnew_r;
///////////////////////////////////////////////////////////////////		
		//Total number of events to loop through.	
		Int_t Total_entries = T->GetEntries();
// Get the last time entry to help determin historam limits
		T->GetEntry(Total_entries-1);
		Double_t clkcnt_l = Vclkcnt; 
		int bins = (Vclkcnt/103700/60+10); 
		int maxb = Vclkcnt/103700/60+10;
// Line to show cut!
		TLine *L_avg = new TLine(0,avg_dnew,maxb,avg_dnew);
// One canvas per rootfile		
		C[num_of_runs]= new TCanvas(Form("C%d",num_of_runs),"Beam quality");
		C[num_of_runs]->cd();
//The uncut histo!
		TH2F *Hdr_Vclk = new TH2F("Hdr_Vclk","dnew rate:V clock count",bins*10,0,maxb,100000,0,100000); 
		T->Draw(Form("ev%sdnew_r:(V1495ClockCount-%1.1f)/103700/60>>Hdr_Vclk",ARM.Data(),Vclkcnt0),"","");
		Hdr_Vclk->GetXaxis()->SetTitle("Time from start of run(minutes)");
		Hdr_Vclk->GetYaxis()->SetTitle("dnew_r (scaler rate)");
		L_avg->Draw("same");
		L_avg->SetLineColor(2);
//The cut histo!
		TH2F *Hdr_Vclk_cut = new TH2F("Hdr_Vclk_cut","dnew rate:V clock count with cut",bins,0,maxb,100000,0,100000); 
	
		int new_event;
		int quality=0;
		Double_t time_up=0;
	
		TVector dnew_rate_v(1);
	

	
	TFile *fqual = new TFile(Form("%sbeam_qual_%d.root",rootfilePath.Data(),run),"read");
	  if(fqual->IsZombie()){
	
	 	//Make anew file and clone the tree.
	newfile = new TFile(Form("%sbeam_qual_%d.root",rootfilePath.Data(),run),"recreate");
	newtree = T->CloneTree();
	TBranch *Qaul_beamup = newtree->Branch("BeamQual.beamOn",&time_up,"BeamQual.beamOn/D");
	TBranch  *Qaul_qual = newtree->Branch("BeamQual.quality",&quality,"BeamQual.quality/I"); 
	
	

		
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
	if(time_up>=0.45){
		Hdr_Vclk_cut->Fill((Vclkcnt-Vclkcnt0)/103700.0/60.0,dnew_r*1.2);
	}
	//counter
	if(i/5000==i/5000.0){cout <<"Entry "<< i << " ! " <<Total_entries-i<< " to GO!"<<endl;}
	
	//reset
	time_up=0;
	quality=0;
	}//end of entry loop;
		
		
		
		newfile->Write();
		delete Qaul_beamup;
		delete Qaul_qual;

	}// end of qual loop
	else{
	//If the gaulity file already has been made
		newfile = new TFile(Form("%sbeam_qual_%d.root",rootfilePath.Data(),run),"read");
		newtree = (TTree*)newfile->Get("T"); 
	
	
	}
		//TCanvas *C2 = new TCanvas("C2","With cut");
		Hdr_Vclk_cut->SetMarkerColor(2);
		Hdr_Vclk_cut->SetLineColor(2);
		Hdr_Vclk_cut->Draw("same");


		C[num_of_runs]->SetName("Dnew_time");
		C[num_of_runs]->Write();
		
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
		}	
	else if(Arm=="R"){
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
	Total_Main[num_of_runs]= newtree->GetEntries(TCut(cut));
	Total_Ele[num_of_runs]= newtree->GetEntries(TCut(cut_e));
	Good_Ele[num_of_runs]= newtree->GetEntries(TCut(cut_good_e));
	Good_Ele_Q[num_of_runs]= newtree->GetEntries(TCut(cut_good_Q));
	cout<<endl; 
	cout<<Form("--- For %s at %s on HRS-%s:  ", Target.Data(), Kin.Data(), Arm.Data())<<endl;
	cout<<"--- Total Good Electrons from these runs are = "<<Good_Ele[num_of_runs]<<endl;
	cout<<"--- Total Good Electrons with new  trip  cut = "<<Good_Ele_Q[num_of_runs]<<endl;
	 Main +=Total_Main[num_of_runs];
	 Good_Ele_total+=Good_Ele[num_of_runs];
	 Total_Ele_total+=Total_Ele[num_of_runs];
	 Total_Good_Ele_Q+=Good_Ele_Q[num_of_runs];

	num_of_runs++;//iterate the number of runs!!! 

	CClock[num_of_runs]=clock();
	double time = (CClock[num_of_runs]-CClock[num_of_runs-1])/CLOCKS_PER_SEC;
	cout << "This run took " << time/60.0 << " minutes to run!"<<endl;	
//////////////////////////////////////////////////////////

	fqual->Close();
	newfile->Close();
cout << "before delete "<<endl;
	//delete newtree;
	
	delete newfile;
cout << "after delete tree and newfile "<<endl;

	
	delete fqual;
cout << "before delete T "<<endl;

		
	delete Hdr_Vclk_cut;
	delete Hdr_Vclk;
	delete L_avg;
	delete T;
	delete C[num_of_runs];
////////////////////////////////////////////////////////////	
	}//end of one run
	t2=clock();
	double t_time = (t2-t1)/CLOCKS_PER_SEC;
	cout << "This kin took " << t_time/60.0 << " minutes to run!"<<endl;	
		
		
	cout << " Electrons before : " << Good_Ele_total <<endl;
	cout << " Electrons after  : " << Total_Good_Ele_Q<<endl;


}



