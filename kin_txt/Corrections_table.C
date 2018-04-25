#include "../inc1.h"
#include "../TRI_Main.h"
#include "../TRI_Tools.h"
#include "THaRun.h"
#include "THaRunParameters.h"
#include "../Pid_eff_3.C"
#include "../rootalias1.h"



//#include "stacktrace.cxx"
//#define BACKWARD_HAS_BFD 1

//#include "../backward.hpp"




/*
/////Function to determine if a input is a number!
bool is_number(const std::string& s)
{
return( strspn( s.c_str(), "-.0123456789" ) == s.size() );

}

/////////////Class to define an input.
class data_type {
	public:
	double value;
	int column;
	std::string type;
	void set_values(double a, int b, std::string str);
	};

void data_type::set_values(double a, int b, std::string str){
	value = a;
	column =b;
	type =str;
	}
/////////////////////////////////////////////////////////////
*/

void Corrections_table(TString filename ="", int ow = 1, int debug =0){
  //Overwrite section
  int overwrite[20]={ow, //Runnumber -0
		     0, //Charge -1
		     0, //DT -2
		     0, //Trigger eff -3
		     0, //PID eff -4
		     0, //tracking eff -5
  };


	if(debug==1){
	cout<<endl<<endl;
	for(int i=0;i<=5;i++){cout<<overwrite[i] <<" :\t";}
	cout<<endl<<endl;
}
/////input the name of the kin file and parse it
  	if(filename==""){ cout<<"Input file name:   "; cin>>filename;}
  	filename = "../Runlist/" + filename;
////////////////////////////////////////////////////
	kin_file KinFile;
	KinFile.set_file(filename.Data(),debug);
	TString tgt 	= 	TString::Format("%s",KinFile.target.c_str());
	TString kin 	= 	TString::Format("%s",KinFile.kin_num.c_str());
	TString Kin 	=	TString::Format("kin%s",kin.Data());
	TString Run_String =	TString::Format("%s",KinFile.run_string.c_str());
	if(KinFile.run_file_status==0){cerr << "error @ Runlist"<<endl;return ;}
///Call tri_tools function to get the runs
  const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
  
	corrections cor_table;
	cor_table.Read_Table(tgt.Data(),kin.Data(),debug);
	vector<std::string> labels;
	labels = cor_table.Labels;
	vector<vector<double>> input_vec;
	input_vec = cor_table.CT;
	vector<int> Oldrun;
	Oldrun = cor_table.ORs;
	int C_status = cor_table.CS;
	if(debug==1){
		cout <<"how many labels "<<labels.size() <<endl;
		cout <<"how many runs   "<<input_vec.size() <<endl;	
	}

	if(C_status==0){overwrite[0]=1;}
  	TString ARM,arm;
	if(RunNoChain[0]<90000){ARM="Left"; arm="L";}
	else{ARM="Right";arm="R";}

  	FILE* kfile =fopen(Form("./%s/%s_%s.txt",Kin.Data(),tgt.Data(),Kin.Data()),"w");
	if(kfile== nullptr){cerr << " File not open " <<endl;
		cout << "making dir " <<endl;
		gSystem->Exec(Form("mkdir %s",Kin.Data()));
		kfile =fopen(Form("./%s/%s_%s.txt",Kin.Data(),tgt.Data(),Kin.Data()),"w");
		if(kfile== nullptr){cerr << " File not open, 2nd " <<endl; return;}		
	}
  	setvbuf ( kfile , NULL , _IOFBF , 1024 );
  	fprintf(kfile,"%s_%s \n",tgt.Data(),Kin.Data());
  
  //Use this when all inputs are defined
  //for(int i =0;i<labels.size();i++){fprintf(kfile,"%s\t",labels[i].c_str());}
  //fprintf(kfile,"\n");

  fprintf(kfile,"Run#\t");
  fprintf(kfile,"Charge_dnew\t");
  if(arm=="L"){for(int j=1;j<4;j++){ fprintf(kfile,"DT_%d\t",j);} }
  else{for(int j=4;j<7;j++){ fprintf(kfile,"DT_%d\t",j);} }
	fprintf(kfile,"PID_eff\t");
  fprintf(kfile,"\n");
  
//////////////////////////////////////////////  
//Start of each run
	for(int r = 0; r<RunNoChain.size();r++){
	         int run=RunNoChain[r]; 
				
	cout<<endl << "This is the start of run " << run <<endl;
		TChain* T = LoadRun(run,"T");
		if(T==nullptr){cout << "skipping run " <<run <<endl; continue;}		

////////////////////////////////////////////////////////////////////////////////////////

	//Find average ccurrent of run.
		TH1F *cur = new TH1F("cur","cur",100,0.01,25.01);
		T->Draw(Form("%sBCM.current_dnew>>cur",ARM.Data()),Form("%sBCM.isrenewed",ARM.Data()),"goff");
		Double_t avg_cur=cur->GetMean();
		int cur_sel=4;
		if(avg_cur>=18.0){cur_sel=4;}
		else if(avg_cur>=15.0){cur_sel=3;}
		else if(avg_cur>=10.0){cur_sel=2;}
		else if(avg_cur>=5.0){cur_sel=1;}
		else{cur_sel=0;}
		delete cur;
		if(debug==1)cout << avg_cur << " "<< cur_sel <<endl;	

	////////////////////////////////
////////////////////////////////Charge/////////////////////////////////////
	   Double_t Charge=0;
	   bool run_in_file = 0;
	   int num_inputs=0;
	   if(C_status==1){
		num_inputs  = input_vec.size();
		run_in_file = std::binary_search (Oldrun.begin(), Oldrun.end(), run);
		if(debug==1)cout << run << " "<< run_in_file <<endl;	
		}
if(debug==1)cout << "debug : check charge ow "<<  (run_in_file==0) <<" "<< (overwrite[0]==1) <<" "<< (overwrite[1]==1) <<endl;	   
	   if(run_in_file==0||(overwrite[0]==1||overwrite[1]==1)){
////////////////////////////////
	   	if(debug==1){cout << "OW Charge" <<endl;}
	   	//Total number of events to loop through.	
		Int_t Total_entries = T->GetEntries();
//Values used in charge calculations
		Double_t dnew_ch_ev=0, unew_ch_ev=0;
		Double_t dnew_ch_total=0, unew_ch_total=0;
		Double_t dnew_cur_ev=0, unew_cur_ev=0;
		Double_t isrenewed=0; //Did the scaler record:
		Double_t BeamUp[5]; //beam up in seconds


//Needed pointer for values in the TChain calibrated 
		T->ResetBranchAddresses();
		T->SetBranchAddress(Form("%sBCM.charge_dnew",ARM.Data()),&dnew_ch_ev);
		T->SetBranchAddress(Form("%sBCM.charge_unew",ARM.Data()),&unew_ch_ev);
		T->SetBranchAddress(Form("%sBCM.current_dnew",ARM.Data()),&dnew_cur_ev);
		T->SetBranchAddress(Form("%sBCM.current_unew",ARM.Data()),&dnew_cur_ev);
		T->SetBranchAddress(Form("%sBCM.isrenewed",ARM.Data()),   &isrenewed);
		T->SetBranchAddress(Form("%sBCM.BeamUp_time_v1495",ARM.Data()),BeamUp);
//uncalibrated	    
		Double_t dnew_u_ch_ev=0, dnew_u_ch_ev_old=0,dnew_u_ch_total=0;
	    T->SetBranchAddress(Form("%sdnew",ARM.Data()),&dnew_u_ch_ev);

		double gain, offset; 
		if(arm=="R"){ gain = 0.0003353; offset =0;}
			else {  gain = 0.0003358; offset =0;}        
    
//for loop to calculate the total charge;
		for(Int_t i=0;i<Total_entries;i++){
			T->GetEntry(i);
//is T-event a new scaler event and if cuts are turned on makes a beam trip cut forcing at least 5.5 uAs
			if(isrenewed==1 && BeamUp[cur_sel] >= 5.0 ){ 	
				dnew_ch_total+=dnew_ch_ev;
				unew_ch_total+=unew_ch_ev;
			}//End of renewed if
			dnew_u_ch_ev_old=dnew_u_ch_ev;
			dnew_ch_ev=0, unew_ch_ev=0; dnew_u_ch_ev=0;//reset;
			}// End of for loop of events in the tchain.   
	   	   	
	   	Charge=dnew_ch_total;
	   	}//end of charge overwrite
		else{
	   	if(debug==1){cout << "No OW Charge : " << input_vec[r][1]<<endl;}
		Charge=input_vec[r][1];
		}

		if(debug==1)cout << "End of Charge " <<endl;
////////////////////////////End of Charge//////////////////////////////////

///////////////////////////Calculate deadtime;
	   Double_t DT[10];
	   int i =0;//Need for else of overwrite;
	   
	   if(run_in_file==0||(overwrite[0]==1||overwrite[2]==1)){
	   	if(debug==1){cout << "OW DT" <<endl;}

	  // THaRun *aRun = (THaRun*)file->Get("Run_Data");
	   //THaRunParameters *para=aRun->GetParameters();
	   TArrayI ps = GetPS(T);//aRun->GetParameters()->GetPrescales();	
	   	
	   char *rate = new char[500];
	   char *clkrate = new char[50];
	   char hname[10][50];
	   char *hh = new char[50];
	   Double_t LT[10]; 
	   int icount[10];  int daqcount[10];
	   TH1F *his[10];   double PS[8];
   
	   for (int k=0; k<8; k++){PS[k]= ps[k];}

	   for (i=1; i<9; i++){
	     TCut t_cut = Form("D%s.evtypebits&(1<<%i)",arm.Data(),i);
	     sprintf(rate,"%sT%i",ARM.Data(), i);
	     icount[i] = T->GetMaximum(rate);
	     sprintf(hname[i],"t%i",i);
	     sprintf(hh,"D%s.evtypebits>>%s", arm.Data(),hname[i]);
	     his[i] =new TH1F (hname[i], hname[i], 100,0,1000000);
	     T->Draw(hh,t_cut, "goff");
	     daqcount[i] = his[i]->GetEntries();
	     if(PS[i-1]>0){
		LT[i] = 100.*PS[i-1]*daqcount[i]/icount[i];
		DT[i] = 100. - LT[i];
		if(debug==1)cout << DT[i]<< " " << daqcount[i] <<" "<< icount[i] <<endl;
	     }
	     delete his[i];
	   }
	
	   }//overwrite
	   else{
	   		for ( i=1; i<4; i++){ 
	   		int i_col;
	   		if(arm=="L"){i_col=i;}
	   			else{i_col=i+3;}
	   		DT[i_col]= input_vec[r][i+1];
			}
	   		
	   }
///////////////////////////////////////////////////Calculate deadtime;   

//////////////////////PID eff...///////////////////////////////////////////
		Double_t Pid_eff=0.000000000;
		if(run_in_file==0||(overwrite[0]==1||overwrite[4]==1)){
			   	if(debug==1){cout << "OW PID" <<endl;}
		
		//Need Left or Right, which beamup[i], calo_thr*, cer_thr*, how long beam up) * not need can set to 0;
		//Pid_eff=gROOT->ProcessLine(Form(".x /adaqfs/home/a-onl/tritium_work/Bane/Tri_offline/Pid_eff_1.C(T,\"%s\",%d,%f,%f,%f)",ARM.Data(),cur_sel,0.0,0.0,1.0));
		Double_t calo_thres = 0.75;
		Double_t cer_thres = 2000;
		//Pid_eff = Pid_eff_1(T,ARM,cur_sel,calo_thres,cer_thres,1.0);
		
		vector<Double_t> in_put =Pid_eff_3(T,ARM,1,cur_sel,calo_thres,cer_thres,1.0,debug);
		if(debug==1)cout<<"PID 1/2 done" <<endl;
		vector<Double_t> in_put1=Pid_eff_3(T,ARM,0,cur_sel,calo_thres,cer_thres,1.0,debug);
		
		if(debug==1)cout<<"PID 2/2 done" <<endl;
		Pid_eff = in_put[2]/in_put[0] * in_put1[2]/in_put1[0] ;
		
		
		if(debug==1)printf("PID : %5.5f\t\n",Pid_eff);
		//return;
		}
		else{
		Pid_eff = input_vec[r][5];
		}
///////////////////////////////////////////////////////////////////////////	

	   
	   
	   
	   

////////////////Addeding to file
	   fprintf(kfile,"%d\t",run);
	   fprintf(kfile,"%5.3f\t",Charge);
	   if(arm=="L"){for(int j=1;j<4;j++){fprintf(kfile,"%0.3f\t",DT[j]);}}
	     else{for(int j=4;j<7;j++){fprintf(kfile,"%0.3f\t",DT[j]);}}
	   fprintf(kfile,"%0.5f\t",Pid_eff);
	   fprintf(kfile,"\n");


	   if(r>=25){break;}
		
//Deleteing run pointers
	delete(T);	
	
	}
//End of run
	fclose(kfile);

	exit(1);
	return ;
}
//End of script



