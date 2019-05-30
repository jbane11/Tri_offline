#include "../headers/inc1.h"
#include "../headers/rootalias1.h"
#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"


void DT_kins_all(){

  TString tgt_A[5] = {"DM","EM","He3","D2","H3"};
  TGraphErrors *GE_DT[5];
  TLegend *leg = new TLegend(0.75,0.6,0.9,0.9);
  TFile *f1 = new TFile("DT_kins_all.root","NEW");
  if( !f1->IsOpen() ){cout <<" Error with root file"<<endl; return;}

  int color[5] = {1 , 2, 4, 6, 7};
  int style[5] = {33,34,39,22,43};

  for(int A=0;A<5;A++){
	if(A>=5){break;}
	cout<<A <<" Starting " <<tgt_A[A] <<" !" <<endl;
	TString tgt = tgt_A[A];

	TString R_list_loc = "/work/halla/triton/Runlist/";

	vector<Double_t> kin_found; //vector to keep track of the kins found;
	kin_found.reserve(20);
	int counter;		//Counter to easly keep count of correct kins;
	
	vector< vector<double> > DT_main; 
	vector<Double_t> DT_avg_kin;	 DT_avg_kin.reserve(20);

	vector<Double_t> run_error;	//Error for TGraph 
	run_error.reserve(20);
	vector<Double_t> DT_error;	//Error in avg calc.	
	DT_error.reserve(20);
//Loop through all kins, max of 16; will run i =0 -> i=15;
	for(int i=0;i<17;i++){	
		cout << "Looking for kin " << i<<endl;	
		////////////////////////////////////////////////////////////////////////
		//Use kin_file class in inc1.h to setup the kin file
		//This needs to be set with set_file
		//It will gives us the runlist, tgt, kin in TStrings
		TString KIN = Form("%s_kin%d.dat",tgt.Data(),i);
		kin_file *K_F = new kin_file; //K_F short for kinemtaic file(sp)
		K_F->set_file(Form("%s%s",R_list_loc.Data(),KIN.Data()));
		//Make sure the kin_file exist!!!
		if(K_F->run_file_status==0){cerr <<"skipping kin " << i <<endl;continue;}
		//store the runlist into a vector;
		vector<int> Run_String = gGet_RunNoChain(K_F->run_string);	
		/////////////////////////////////////////////////////////////////////////
		
                vector<double> scaler_count;    //Number of events to fire the scaler;
                vector<double> daq_count;       //Number of events to pass to the DAQ;
		vector<double> DT_tmp;		//DT for each run, reset at start of new kin;
		scaler_count.clear();
		daq_count.clear();
		DT_tmp.clear();
	
		double DT_calc_avg=0;
		int run_count=0;
		//Look at one run at at time!
		for(int j=0;j<Run_String.size();j++){
			cout <<"Looking at Run " <<Run_String[j]<< "\t\t" ;
			int run = Run_String[j];
			TChain *T = new TChain;
			T= LoadRun(run,"T");
			if(T==nullptr){cerr << "\tSkipping run " <<run<<"!!"<<endl; continue;}
        		TString ARM,arm;
			int main_trigger=0;
	                if(run<90000){ARM="Left"; arm="L";main_trigger=2;}
                        	else{ ARM="Right";arm="R";main_trigger=5;}
			/////Get prescale values!
			int PS_main=GetPS(T,main_trigger);
			//////////////
			//Define which arm	
			/////////////////////	
			//Get number of scaler events from the maximum recoded scaler event. 
			scaler_count.push_back(T->GetMaximum(Form("%sT%i",ARM.Data(),main_trigger)));
		
			//Cut for main trigger only
			TCut trigger=Form("D%s.evtypebits&(1<<%i)",arm.Data(),main_trigger);		
			TH1F *his = new TH1F("his","Trigger bites",1000,-1,999);
			T->Draw(Form("D%s.evtypebits>>his",arm.Data()),trigger,"goff");
			daq_count.push_back(his->GetEntries());
			double DT= (1.-PS_main*daq_count[run_count]/scaler_count[run_count])*100;
			DT_calc_avg+=DT;
			DT_tmp.push_back(DT);
			cout << DT<< "\n";			
			
			//Clean up!!!		
			delete his;
			delete T;

			run_count++;
		}//end of one run at at time
		
//		DT_main.push_back(DT_tmp);
		DT_calc_avg=DT_calc_avg/(run_count*1.0);
		DT_avg_kin.push_back(DT_calc_avg);
		kin_found.push_back(i);

		double sum = std::accumulate(DT_tmp.begin(), DT_tmp.end(), 0.0);
		double mean = sum / DT_tmp.size();

		std::vector<double> diff(DT_tmp.size());
		std::transform(DT_tmp.begin(), DT_tmp.end(), diff.begin(),
               	std::bind2nd(std::minus<double>(), mean));
		double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
		double stdev = std::sqrt(sq_sum / DT_tmp.size());
		DT_error.push_back(stdev);
		run_error.push_back(0.0);
//cout<<endl<< sum << " "<< mean<<" "<<sq_sum<<" "<<" "<< DT_tmp.size()<<endl;
cout<< " kin " <<i<<"   tgt"<<tgt<<"   :   " << DT_calc_avg<<"    "  << stdev <<endl;	
		counter++;
		DT_tmp.clear();
	}//end of kin loop
	cout <<f1 << "  "<< gFile <<endl;

	TVectorD Tkin_found(kin_found.size(),	&kin_found[0]);
	TVectorD Trun_error(run_error.size(),	&run_error[0]);
	TVectorD TDT_avg_kin(DT_avg_kin.size(),	&DT_avg_kin[0]);
	TVectorD TDT_error(DT_error.size(),	&DT_error[0]);


	GE_DT[A] = new TGraphErrors(Tkin_found,TDT_avg_kin,Trun_error,TDT_error);
	GE_DT[A]->Draw("ap");
	GE_DT[A]->SetTitle(Form("Dead time of Main Trigger for %s.",tgt.Data()));
	GE_DT[A]->GetXaxis()->SetTitle("Kin #");
	GE_DT[A]->GetYaxis()->SetTitle("DT");
	GE_DT[A]->SetMarkerColor(color[A]);
	GE_DT[A]->SetMarkerStyle(style[A]);
	leg->AddEntry(GE_DT[A],Form("%s",tgt.Data()),"lep");

	GE_DT[A]->Write( Form("DT_%s",tgt.Data() ) );

  }//end of tgt

  cout << " Done Collect data and making T Graphs "<<endl; 
  int check;

  TMultiGraph *MG_DT = new TMultiGraph();
  for(int A=0;A<5;A++){
	cout << GE_DT[A]->IsA() <<"\t";
	MG_DT->Add(GE_DT[A],"lp");}
  cout << " Made the Multi Graph" <<endl;
  
  TCanvas *C = new TCanvas;
  MG_DT->Draw("a");
  leg->Draw("same");
  C->Print("DT_all_kins.jpg");
  f1->Close();
  cout << "Made the canvas!!"<<endl;



}//End of DT_kins
