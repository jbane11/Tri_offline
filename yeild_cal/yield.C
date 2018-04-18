#include "../inc.h"
#include "../TRI_Main.h"
#include "../TRI_Tools.h"
#include "THaRun.h"
#include "THaRunParameters.h"
#include "../rootalias1.h"
   

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


void yield(TString filename ="", int debug =0){

    TString rootfilePath="/chafs1/work1/tritium/Rootfiles/";
    
    
/////input the name of the kin file and parse it
  	if(filename==""){ cout<<"Input file name:   "; cin>>filename;}
  	filename = "../Runlist/" + filename;
	kin_file kinfile;
	kinfile.set_file(filename.Data(),debug);
	
	
///Call tri_tools function to get the runs
  const vector<Int_t> RunNoChain=gGet_RunNoChain(kinfile.run_string);



  TVector RunList(1); vector<int> Oldrun;

///////////////////////////////////////////////////////////////////////////////////////
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
  list.open (Form("../kin_txt/kin%s/%s_kin%s.txt",kinfile.kin_num.c_str(),kinfile.target.c_str(),kinfile.kin_num.c_str()), std::fstream::in); 
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
      //std::cout << token << " "<< columns<<" "<< numint<<std::endl;
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
////////////////////////////////////////////////////////////////////////////////////  
	TChain *T = new TChain("T", "main tree");
	TH1F *xb_[RunNoChain.size()];
	TH1F *QQ_[RunNoChain.size()];
	TH1F *p_[RunNoChain.size()];
	TH1F *E_[RunNoChain.size()];
	TH1F *theta_[RunNoChain.size()];
	TH1F *nu_[RunNoChain.size()];
		
	int x_steps; int q_steps;
	int p_steps;int the_steps;
	
	TList *LList = new TList;
	TH1F *xb = new TH1F("xb","x_bj",100,0,1);
	TH1F *xb2[RunNoChain.size()];

//	TH1F *dp = new TH1F("dp","dp",
	
	TCanvas *C[RunNoChain.size()];
	
	TCanvas *C3 = new TCanvas("c3","c3");

	for(int i=0; i<RunNoChain.size();i++){
		int run = RunNoChain[i];
		


		T=LoadRun(run, "T");
		cout << T->GetEntries() <<endl;
		TString final_c;
		final_c = Good_Electron_Cuts( Form("%s_kin%s",kinfile.target.c_str(),kinfile.kin_num.c_str()) ,"Left" );	
		TArrayI  ps=GetPS(T);
	
		TChain* Epics=LoadRun(run,"E");  
		Double_t p0, angle,pos,ebeam;
		Epics->SetBranchAddress("HALLA_p",&ebeam);
		Epics->SetBranchAddress("haBDSPOS",&pos);
		Epics->SetBranchAddress("HacL_alignAGL",&angle); 
    	Epics->SetBranchAddress("HacL_D1_P0rb",&p0);


		Epics->GetEntry(1);
		double dp_steps, the_steps;
		dp_steps = (0.05+0.05)/(p0*1e-4) ;
		the_steps= (0.06+0.06)/0.005;
		
		cout << dp_steps <<endl;
		
		
		C[i] = new TCanvas(Form("C%d",i),Form("C_run%d",run));
		C[i]->Divide(1,4);
		
		xb_[i]	=new 	TH1F(Form("xb_%d",i),Form("Xb_run%d",run),100 ,0,1 );
		xb2[i]	=new 	TH1F(Form("xb2_%d",i),Form("Xb_run%d",run),100 ,0,1 );
		QQ_[i]	=new 	TH1F(Form("QQ_%d",i),Form("QQ_run%d",run),1500,0,15);
		p_[i]	=new 	TH1F(Form("p_%d",i),Form("p_run%d",run),dp_steps,-0.05,0.05);
		theta_[i]= new 	TH1F(Form("theta_%d",i),Form("theta_run%d",run),the_steps,-0.06,0.06);
		
		//corrections for this run	
		double DT_main = input_vec[i+2][3];
		double PID_cor = input_vec[i+2][5];
		double charge  = input_vec[i+2][1];
		
		double weight = PID_cor/charge/((100-DT_main)/100.0);
		
		cout <<DT_main <<" "<< PID_cor <<" "<< charge <<" "<< weight <<endl;
	
		C[i]->cd(1);
		T->Draw(Form("L.tr.tg_th>>theta_%d",i),TCut(final_c),"");
		theta_[i]->Scale(weight);
		
		C[i]->cd(2);
		T->Draw(Form("L.gold.dp>>p_%d",i),TCut(final_c),"");
		p_[i]->Scale(weight);
		
		C[i]->cd(3);
		T->Draw(Form("EKLx.Q2>>QQ_%d",i),TCut(final_c),"");
		QQ_[i]->Scale(weight);
		
		C[i]->cd(4);
		T->Draw(Form("EKLx.x_bj>>xb_%d",i),TCut(final_c),"");
		xb_[i]->Scale(weight);
		LList->Add(xb_[i]); 
		C3->cd();
		T->Draw(Form("EKLx.x_bj>>xb2_%d",i),TCut(final_c),"same");
		xb2[i]->SetLineColor(1+i);
		xb2[i]->Scale(weight);
		
			
	break;
	}
  	
  	
  	TCanvas *C2 = new TCanvas("c2",Form("%s_kin%s",kinfile.target.c_str(),kinfile.kin_num.c_str())  );	
  	xb->Merge(LList);
  	
  	xb->Draw();
  	
  	
  	TCanvas *CC22 = new TCanvas("cc22",Form("22_%s_kin%s",kinfile.target.c_str(),kinfile.kin_num.c_str())  );
  	TH1F *xb_3	=new 	TH1F("xb_3",Form("Xb%s_kin%s",kinfile.target.c_str(),kinfile.kin_num.c_str()),100 ,0,1 );
	TH1F *QQ_3	=new 	TH1F("QQ_3",Form("QQ%s_kin%s",kinfile.target.c_str(),kinfile.kin_num.c_str()),1500,0,15);
	//TH1F *p_3	=new 	TH1F("p_3",Form("p_%s_kin%s",kinfile.target.c_str(),kinfile.kin_num.c_str()),dp_steps,-0.05,0.05);
	//TH1F *theta_3= new 	TH1F("theta_3",Form("theta_%s_kin%s",kinfile.target.c_str(),kinfile.kin_num.c_str()),the_steps,-0.06,0.06);
  	
  	TTree *TT;
  	TT=LoadRun(RunNoChain[0], "T");
 
	
	TString final_cc;
	final_cc = Good_Electron_Cuts( Form("%s_kin%s",kinfile.target.c_str(),kinfile.kin_num.c_str()) ,"Left" );
   	
  	

  	int good_es =0;
  	TT->Draw(">>Good_e_list",final_cc);
  	TEventList *Good_e_list = new TEventList();
  	gDirectory->GetObject("Good_e_list",Good_e_list);
  	Long64_t *GE_list = Good_e_list->GetList();
  	good_es = Good_e_list->GetN();
  	
  	double xb_tree;
	TT->ResetBranchAddresses();
  	TT->SetBranchAddress("EKLx.x_bj" ,&xb_tree);
  	Long64_t Event;
  	
  	for(int i = 0; i<good_es;i++){
	  	Event=GE_list[i];
	  	TT->GetEntry(Event);
	  	xb_3->Fill(xb_tree);
  		}
  		
  	xb_3->Draw();
  	
  	
  
  
  
}
  
 
