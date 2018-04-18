#include "../inc.h"
#include "../TRI_Main.h"
#include "../TRI_Tools.h"
#include "THaRun.h"
#include "THaRunParameters.h"
#include "../Pid_eff_1.C"
#include "../Pid_eff_3.C"
#include "../rootalias1.h"



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


void kin_cor_txt_multi(TString filename ="", int ow = 0, int debug =0){
  //Overwrite section
  int overwrite[20]={ow, //Runnumber -0
		     0, //Charge -1
		     0, //DT -2
		     0, //Trigger eff -3
		     1, //PID eff -4
		     0, //tracking eff -5
  };


  
/////input the name of the kin file and parse it
  if(filename==""){ cout<<"Input file name:   "; cin>>filename;}
  filename = "../Runlist/" + filename;
  ifstream file(filename.Data());
 
  if(!file.good()){cout << filename.Data() << " does not exist! "<<endl; exit(1);}
  TString content;
  TString Target,Kin,Run_String;
  
  TString kin;
  for (int ii=0; content.ReadLine(file) && ii<3;ii++ ) {  
    cout<<"!!!:  "<<content<<endl;
    if(ii==0)Target = content;
       if(ii==1){
           kin = content.Data();
           Kin = Form("kin%s", kin.Data());
         }
    if(ii==2)Run_String = content;         
  }
  file.close();
////////////////////////////////////////////////////////

///Call tri_tools function to get the runs
  const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);

  TString rootfilePath="/chafs1/work1/tritium/Rootfiles/";
  TVector RunList(1); vector<int> Oldrun;

  //Read in the old txt file..................................
//  vector<double> Charge;
//  vector<vector<int>> dtime;

  vector<string>  labels;
  vector<data_type> d_type;
  d_type.reserve(100);
 
  Int_t num_of_runs=0;
  int first_run = RunNoChain[0];
  int last_run = RunNoChain[RunNoChain.size()-1];
  

  std::fstream list;
  std::string line_file;
  list.open (Form("./%s/%s_%s.txt",Kin.Data(),Target.Data(),Kin.Data()), std::fstream::in); 
  std::string stringrun;std::string::size_type sz;
  int inrun=0;
  int status=0;
  int innum=0;
  int numint=0;
  int num_of_inputs=0;
  vector<string> vec;
  std::vector< std::vector< float > > input_vec;
  std::vector <float> col;
  std::string delimiter = "\t";
  

  while (std::getline(list, line_file)){
    
	vec.push_back(line_file);
    size_t pos = 0;
    size_t pos1 = 0;
    int columns=1;
    std::string token;

    //cout << line_file <<"::"<<endl;
    while ((pos = vec[numint].find(delimiter)) != std::string::npos) {

      token = vec[numint].substr(0, pos);
  //    std::cout << token << " "<< columns<<" "<< numint<<std::endl;
      vec[numint].erase(0, pos + delimiter.length());
  	  

	  if(numint==1){labels.push_back(token);}
  	  double in_num;
  	  
  	 //cout <<token <<endl;
  	 
  	  if(is_number(token)) {
  	  	 num_of_inputs++;
  	  	 in_num=stod(token);  		  
  	     col.push_back (in_num);
// cout<<"vec "<<columns<<" " << col[columns-1]<<" "<< token<< " "<<endl;
  	   	 d_type.resize(num_of_inputs);

  	   	 d_type[num_of_inputs-1].set_values(in_num,columns,labels[columns-1]);
//cout << d_type[num_of_inputs-1].type<< " : "<<endl;
     	  }
	
   	  columns++;
   	   
   	   
   	      
    }
     

     input_vec.push_back(col);
     col.clear();
     //	cout<< input_vec.size()<< endl;
 
    
    //std::cout << line_file << std::endl;
    numint++;
    if(numint==1||numint==2)continue;
    for(int jj =0; jj<10;jj++){
      if(line_file[jj]== ' '){break;}
      stringrun+=line_file[jj];
    }	
    inrun=stoi(stringrun,&sz);
    size_t leng = stringrun.length();
   
    Oldrun.push_back(inrun);
    //cout<< Oldrun[innum] <<endl;
    innum++;
// for (auto i : vec) cout << i << '\n';
     }
  list.close();
  
 // cout << innum<<endl;
  
  
  //Read in the old txt file..................................
//  cout << "Number of entries " <<d_type.size()<< " " <<endl;
//  cout <<d_type.size()<< " "<<d_type[23].column<< " "<<d_type[23].value<< " "<<d_type[23].type<<" "<<endl;
 // cout << input_vec[2][0]<<endl;
  if(labels.size()<1||input_vec.size()<2){overwrite[0]=1;}
 
//  cout << "check " <<endl;
  TString ARM,arm;
  if(RunNoChain[0]<90000){ARM="Left"; arm="L";}
  else{ARM="Right";arm="R";}

  FILE* kfile =fopen(Form("./%s/%s_%s.txt",Kin.Data(),Target.Data(),Kin.Data()),"w");
  setvbuf ( kfile , NULL , _IOFBF , 1024 );
  fprintf(kfile,"%s_%s \n",Target.Data(),Kin.Data());
  
  //Use this when all inputs are defined
  //for(int i =0;i<labels.size();i++){fprintf(kfile,"%s\t",labels[i].c_str());}
  //fprintf(kfile,"\n");

  fprintf(kfile,"Run#\t");
  fprintf(kfile,"Charge_dnew\t");
  if(arm=="L"){for(int j=1;j<4;j++){ fprintf(kfile,"DT_%d\t",j+1);} }
  else{for(int j=5;j<8;j++){ fprintf(kfile,"DT_%d\t",j+1);} }
	fprintf(kfile,"PID_eff\t");
  fprintf(kfile,"\n");
  
//////////////////////////////////////////////  
//Start of each run
	for(int r = 0; r<RunNoChain.size();r++){
	   int run=RunNoChain[r]; 
				
	   cout << "This is the start of run " << run <<endl;
////////////////////////////////////////////////////////////////////////////////////////

	   TString rootfilePath="./Rootfiles/";

	   TFile *file = new TFile(Form("%stritium_%d.root",rootfilePath.Data(),run),"read");
	   if(file->IsZombie()){
	     cout<<" this rootfile doest not exist: "<<endl;
	     cout<<"In the future, might force replay or retrieval" <<endl;
	     continue;
	   }
/*
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
*/
		 TChain* T = LoadRun(run,"T");
		

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

//		cout << avg_cur << " "<< cur_sel <<endl;	

	////////////////////////////////
////////////////////////////////Charge/////////////////////////////////////
	   Double_t Charge=0;
	   bool run_in_file = 0;
	   if(innum>0){run_in_file = std::binary_search (Oldrun.begin(), Oldrun.end(), run);}
	   
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


		//	if(isrenewed)cout<<i<<" " << dnew_ch_total<<"\t"<< dnew_u_ch_total*gain<<endl;

				dnew_u_ch_ev_old=dnew_u_ch_ev;
				dnew_ch_ev=0, unew_ch_ev=0; dnew_u_ch_ev=0;//reset;
			}// End of for loop of events in the tchain.   
	   	   	
	   	Charge=dnew_ch_total;
	   	}//end of charge overwrite
		else{
		Charge=input_vec[r+2][1];
		}

  cout << "check " <<endl;
////////////////////////////End of Charge//////////////////////////////////

///////////////////////////Calculate deadtime;
	   Double_t DT[10];
	   int i =0;//Need for else of overwrite;
	   
	  // cout << "check"<<endl;
	  // cout <<input_vec[r+2][0]<<endl;
	  // cout << "check"<<endl;
	   
	   
	   if(run_in_file==0||(overwrite[0]==1||overwrite[2]==1)){
	   	if(debug==1){cout << "OW DT" <<endl;}

	   THaRun *aRun = (THaRun*)file->Get("Run_Data");
	   //THaRunParameters *para=aRun->GetParameters();
	   TArrayI ps = aRun->GetParameters()->GetPrescales();	
	
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
		//cout << DT[i]<< " " << daqcount[i] <<" "<< icount[i] <<endl;
	     }
	   }
	
	   }//overwrite
	   else{
	   		for ( i=1; i<4; i++){ 
	   		int i_col;
	   		//cout << "check "<< r <<" "<< i <<endl;
	   		if(arm=="L"){i_col=i;}
	   			else{i_col=i+3;}
	   		DT[i_col]= input_vec[r+2][i+1];
			}
	   		
	   }
///////////////////////////////////////////////////Calculate deadtime;   

//////////////////////PID eff...///////////////////////////////////////////
		Double_t Pid_eff=0.0001000000;
		printf(":%5.5f\t\n",Pid_eff);
		if(run_in_file==0||(overwrite[0]==1||overwrite[4]==1)){
			   	if(debug==1){cout << "OW PID" <<endl;}
		
		//Need Left or Right, which beamup[i], calo_thr*, cer_thr*, how long beam up) * not need can set to 0;
		//Pid_eff=gROOT->ProcessLine(Form(".x /adaqfs/home/a-onl/tritium_work/Bane/Tri_offline/Pid_eff_1.C(T,\"%s\",%d,%f,%f,%f)",ARM.Data(),cur_sel,0.0,0.0,1.0));
		Double_t calo_thres = 0.75;
		Double_t cer_thres = 2000;
		//Pid_eff = Pid_eff_1(T,ARM,cur_sel,calo_thres,cer_thres,1.0);
		
		vector<Double_t> in_put =Pid_eff_3(T,ARM,1,cur_sel,calo_thres,cer_thres,1.0,1);
		vector<Double_t> in_put1=Pid_eff_3(T,ARM.Data(),0,cur_sel,calo_thres,cer_thres,1.0,1);
		
		Pid_eff = in_put[2]/in_put[0] * in_put1[2]/in_put1[0] ;
		
		
		printf(":%5.5f\t\n",Pid_eff);
		//return;
		}
		else{
		Pid_eff = input_vec[r+2][5];
		}
///////////////////////////////////////////////////////////////////////////	

	   
	   
	   
	   

////////////////Addeding to file
	   fprintf(kfile,"%d\t",run);
	   fprintf(kfile,"%5.3f\t",Charge);
	   if(arm=="L"){for(int j=1;j<4;j++){fprintf(kfile,"%0.3f\t",DT[j]);}}
	     else{for(int j=5;j<8;j++){fprintf(kfile,"%0.3f\t",DT[j]);}}
	   fprintf(kfile,"%0.5f\t",Pid_eff);
	   fprintf(kfile,"\n");


	   if(r>=25){break;}
		
//Deleteing run pointers
	delete(T);	
	
	}
//End of run
	fclose(kfile);

	exit(1);
}
//End of script



