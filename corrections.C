#include "TRI_Main.h" //Used for th RunNoChain to Tchain all of the TTrees together
#include "TRI_Tools.h" //Used for th RunNoChain to Tchain all of the TTrees together

//////
/*Trying to design a code to calculated the needed corrections and add them to the root file, for each event.



This needs to be run with the analyzer for the dead time calculation.
////

*/


void corrections(Int_t run = 0){

	if(run==0){cout << "please enter the run number" <<endl;cin>>run;}

	TString Target,Kin,Run_String;
	gStyle->SetOptStat(0);
//	Run_String =TString::Format("%d",run);
//    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
//	TChain* T =(TChain*) gGetTree(RunNoChain, "T");
	
	TString rootfilePath="/chafs1/work1/tritium/Bane_dat/";
 	
 	//==========this to take care the split rootfiles========================
   	TString file_name = TString::Format("%stritium_%d.root",rootfilePath.Data(),run);
	TString basename = TString::Format("%stritium_%d",rootfilePath.Data(),run);
	TString rootfile = basename + ".root";
    TChain* T = new TChain("T");
    Long_t jk=0;
    while ( !gSystem->AccessPathName(rootfile.Data()) ) {
        T->Add(rootfile.Data());
        cout << "ROOT file " << run<< "_"<< jk << " added to TChain." << endl; jk++;
        rootfile = basename + "_" + jk + ".root"; 
       // if(i>10){break;}
        }


   TTree *tree1;
   TFile *file = new TFile(Form("%stritium_%d.root",rootfilePath.Data(),run),"read");
    tree1 = (TTree*)file->Get("T");
  if(file->IsZombie()){
    cout<<" this rootfile doest not exist: "<<endl;
    cout<<"Please try again with a new run. "<<endl;
    return;
  }
////////////////////////////////////////////////////////////////////////////////////////
	
	TString ARM,arm;
	if(run<90000){ARM="Left"; arm="L";}
			 else{ARM="Right";arm="R";}
	
	//Needs a Ttree named T and Left or Right, then bool for beam trip cut
	Double_t Total_charge  = gROOT->ProcessLine(Form(".x Total_Charge_kin.C(\"%s\",%d)",ARM.Data(),0));	
		cout <<"\n\n";
	Double_t PID_eff =  gROOT->ProcessLine(Form(".x PID_eff.C(\"%s\",%d,%f,%f)",ARM.Data(),2,0.0,0.0));
		cout <<"\n\n";
	Double_t tri_eff = gROOT->ProcessLine(Form(".x trigger_eff.C(\"%s\",%f)",ARM.Data(),2.0));
		cout <<"\n\n";
	Double_t Track_eff =1;// gROOT->ProcessLine(Form(".x Track_eff.C(\"%s\",%d)",ARM.Data(),0));
	////////////////////////////

///////////////////////////Calculate deadtime;
  	THaRun *aRun = (THaRun*)file->Get("Run_Data");
  	THaRunParameters *para=aRun->GetParameters();
  	TArrayI ps = aRun->GetParameters()->GetPrescales();	
	
	char *rate = new char[500];
	char *clkrate = new char[50];
  	char hname[10][50];
  	char *hh = new char[50];
  	Double_t LT[10], DT[10];
  	int icount[10];
  	int daqcount[10];
  	TH1F *his[10];
  	double PS[8];
   
    for (int k=0; k<8; k++){PS[k]= ps[k];}

    for (int i=1; i<9; i++){
      TCut t_cut = Form("D%s.evtypebits&(1<<%i)",arm.Data(),i);
      sprintf(rate,"ev%sT%i",ARM.Data(), i);
      icount[i] = T->GetMaximum(rate);
      sprintf(hname[i],"t%i",i);
      sprintf(hh,"D%s.evtypebits>>%s", arm.Data(),hname[i]);
      his[i] =new TH1F (hname[i], hname[i], 100,0,1000000);
      T->Draw(hh,t_cut, "goff");
      daqcount[i] = his[i]->GetEntries();
      if(PS[i-1]>0){
		LT[i] = 100.*PS[i-1]*daqcount[i]/icount[i];
		DT[i] = 100. - LT[i];
      }
    }
///////////////////////////Calculate deadtime;	
	
/////////////////////////////////////////////////////////////////
//varibles need for per event calculations
	Double_t Rad_cor;	
	

   	T->SetBranchStatus("*",1);
	TFile *newfile = new TFile(Form("%stri_cor_%d.root",rootfilePath.Data(),run),"recreate");
	
	TTree *newtree = T->CloneTree();
//Total Charge per run.
	TBranch *Q = newtree->Branch("TotalCharge",&Total_charge,"Total_charge/D");
//DT for each trigger per run.
	TBranch *DT_t =newtree->Branch("DT_trigger",DT,"DT_trigger[10]/D");
//Efficency of the main electron trigger
	TBranch *Tri_eff= newtree->Branch("Trigger_eff", &tri_eff,"tri_eff/D");
//PID effeciency per run.... this includes pion rejection and electron accaptance
	TBranch *pid_eff = newtree->Branch("pid_eff",&PID_eff,"PID_eff/D");
//Tracking efficency per run	
	TBranch *trk_eff = newtree->Branch("track_eff",&Track_eff,"track_eff/D");
//Radiative correction per event, based on the legnth of travel
	TBranch *rd_cor = newtree->Branch("Rad_cor",&Rad_cor,"Rad_cor/D");
	
	
	int nentries = newtree->GetEntries();
	for(int i =0; i<nentries;i++){
		T->GetEntry(i);		Q->Fill();			DT_t->Fill();
		pid_eff->Fill();	trk_eff->Fill();	rd_cor->Fill();
		Tri_eff->Fill();
		}
	
	newfile->Write();
		
	
	
	}
	
