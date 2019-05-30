/*C/C++ Includes{{{*/
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <sys/stat.h>
/*}}}*/
/*ROOT Includes{{{*/
#include <TArray.h>
#include <TEventList.h>
#include <TString.h>
#include <TCanvas.h>
#include <TVector.h>
#include <TLine.h>
#include <TSystem.h>
#include <TStyle.h>
#include <Riostream.h>
#include "TObjString.h"
#include <TNamed.h>
#include <TPRegexp.h>
#include <TObjArray.h>
#include <TChain.h>
#include <TMath.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TROOT.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TMultiGraph.h>
#include <TDatime.h>
#include <TError.h>
#include <TVirtualFitter.h>
#include <TSQLServer.h>
#include <TSQLResult.h>
#include <TSQLRow.h>
#include <TCut.h>
#include <TMultiGraph.h>
#include <TCutG.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TRandom3.h>
#include <TSpectrum.h>
#include <TRatioPlot.h>
#include "TPaveStats.h"





//#include "/home/jbane/headers/rootalias.h"
#include "./rootalias.h"
#include "./SQLanalysis.h"
//#include "/home/jbane/headers/SQLanalysis.h"
using namespace std;
//	string kin_cor_loc = "/adaqfs/home/a-onl/tritium_work/Bane/Tri_offline/kin_txt/";
	vector<string> alltgt_names  = { "H3" ,"D2", "H", "He3","C12"};
	vector<string> alltgt_vect  = { "H3" ,"D2", "H", "He3"};
	vector<int>goodcolors ={1,2,3,6,7,9,15};

	std::string kin_cor_loc = "/home/jbane/tritium/Tri_offline/kin_txt/";
	std::string T2_MC_Dir= "/home/jbane/halla_xem/weight_T2/";
			///Varibles defined for cuts
	double TG_Theta_Max_inc = 0.04;//40mrad
	double TG_Theta_Min_inc =-0.04;//40mrad
	double TG_Phi_Max_inc = 0.030;//25mrad
	double TG_Phi_Min_inc =-0.030;//25mrad
	double TG_Dp_Max_inc = 0.04;//4%mrad
	double TG_Dp_Min_inc =-0.04;//4%mrad
	double TG_VZ_Max_inc = 0.1200;//4%mrad
	double TG_VZ_Min_inc =-0.1200;//4%mrad
	double P0_inc = 3.100; //GeV/c
	double GC_Cut_inc = 1000.;
	double EP_Cut_inc= 0.75;
	int Main_Trigger_Left_inc = 2;
	int Main_Trigger_Right_inc = 5;
	double e_sample_int_inc = 3000.0;
	double e_sample_m_inc= -3000.0/2300.0;
	double p_sample_int_inc = 1500.0;
	double p_sample_m_inc = -1.0;
	double GC_e_sample_min_inc=4000;
	double GC_e_sample_max_inc=6000;
	double GC_p_sample_inc = 1000;
	int cur_sel_inc=0;
	double beamon_min_inc=1;
	double PI=3.14159265359;

  //struct tm last_mc_edit = {0};
	// varible size bins for x
	vector< vector<double>> varible_xbase = {{},
		{0.17,0.19,0.21,0.23,0.25,0.27,0.29,
		 0.31,0.34,0.37,0.40,0.43,
		 0.46,0.5,0.54,0.58,0.62,0.68,
		 0.72,0.77,0.82,0.87},
		 {0.18,0.20,0.22,0.24,0.26,0.28,0.30,0.32,0.35,0.38,0.41,0.44,0.47,0.51,0.55,0.59,0.63,0.67,0.71,0.76,0.81,0.86},
		 {0.18,0.21,0.24,0.27,0.30,0.33,0.36,0.39,0.42,0.45,0.48,0.52,0.56,0.60,0.64,0.68,0.72,0.77,0.82,0.87},
		 {0.17,0.20,0.23,0.26,0.29,0.32,0.35,0.38,0.41,0.44,0.47,0.51,0.55,0.59,0.63,0.68,0.73,0.79,0.86},
		 {0.14,0.20,0.23,0.26,0.29,0.32,0.35,0.38,0.41,0.44,0.47,0.51,0.55,0.61,0.66,0.71,0.77,0.84}

	};
	vector< vector<double>> varible_xstep = {{},
		{0.02,0.02,0.02,0.02,0.02,0.02,0.02,
		 0.03,0.03,0.03,0.03,0.03,
		 0.04,0.04,0.04,0.04,0.04,0.04,
		 0.05,0.05,0.05,0.05},
		 {0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.03,0.03,0.03,0.03,0.03,0.04,0.04,0.04,0.04,0.04,0.04,0.05,0.05,0.05,0.05},
		 {0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.04,0.04,0.04,0.04,0.04,0.04,0.05,0.05,0.05,0.05},
		 {0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.04,0.04,0.04,0.04,0.05,0.05,0.06,0.07,0.08},
		 {0.06,0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.03,0.04,0.04,0.06,0.05,0.05,0.06,0.07,0.08}
	 };



	vector<vector<double>> ECC_table;

	vector<vector<double>> RC_table;
	string RC_tgt="";
	int RC_kin;


void SetStyles(){
TStyle* mcStyle = new TStyle("mcStyle","Manuel's Root Styles");
 mcStyle->SetPalette(kViridis); // avoid horrible default color scheme
 mcStyle->SetOptStat(0);
 mcStyle->SetOptTitle(1);
 //mcStyle->SetOptDate(0);
 mcStyle->SetPadGridX(kTRUE);
 mcStyle->SetPadGridY(kTRUE);
 mcStyle->SetLabelSize(0.05,"xyz"); // size of axis value font
 mcStyle->SetTitleSize(0.05,"xyz"); // size of axis title font
 mcStyle->SetTitleFont(22,"xyz"); // font option
 mcStyle->SetLabelFont(22,"xyz");
 mcStyle->SetTitleOffset(1.05,"y");
 mcStyle->SetLegendBorderSize(0);
 mcStyle->SetGridStyle(0);
 mcStyle->SetGridColor(22);
 mcStyle->SetLegendFillColor(0);
 mcStyle->SetOptTitle(0);
 //mcStyle->SetLegendFillStyle(0);
 //mcStyle->SetLegendTextSize(22);

 gROOT->SetStyle("mcStyle");
}

void HallA_style() {
  gROOT->SetStyle("Plain");
  gStyle->SetPaperSize(TStyle::kUSLetter);
  gStyle->SetPaperSize(18,22);
  gStyle->SetOptFit(1111);
  gStyle->SetPalette(1);
  gStyle->SetNdivisions(505);

  gStyle->SetCanvasColor(10);
  gStyle->SetPadTopMargin(.05);
  gStyle->SetPadLeftMargin(.15);
  gStyle->SetPadRightMargin(.1);
  gStyle->SetPadBottomMargin(.15);
  gStyle->SetTitleYOffset(1.3);
  gStyle->SetLabelFont(42,"X");
  gStyle->SetLabelFont(42,"Y");

  // prepare gStyle to be useful
  //   1 = solid
  //   2 = long dash (30 10)
  //   3 = dotted (4 8)
  //   4 = dash-dot (15 12 4 12)
  //   5 = short dash ( 15 15 )
  //   6 = dash-dot-dot
  gStyle->SetLineStyleString(1,"[]");
  gStyle->SetLineStyleString(2,"[30 10]");
  gStyle->SetLineStyleString(3,"[4 8]");
  gStyle->SetLineStyleString(4,"[15 12 4 12]");
  gStyle->SetLineStyleString(5,"[15 15]");
  gStyle->SetLineStyleString(6,"[15 12 4 12 4 12]");
  gStyle->SetLabelSize(0.045,"X");
  gStyle->SetLabelSize(0.045,"Y");
  gStyle->SetNdivisions(505,"Y");
  gStyle->SetOptDate(0);
  gStyle->SetDateY(.98);
  gStyle->SetStripDecimals(kFALSE);
}


void HistSettings(TH1F* H){
	H->GetXaxis()->CenterTitle();
	H->GetYaxis()->CenterTitle();
}

void HistSettings(TH2F* H){
	H->GetXaxis()->CenterTitle();
	H->GetYaxis()->CenterTitle();
}

//classes/

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



class Corrections_Value {
	public:
	double cor;
	double err;
	string name;
	void set_values(double a, double b, string c);
};
	void Corrections_Value::set_values(double a, double b, string c=""){
		cor=a; err =b; name=c;}

bool isnumber_1(string line)
{
    char* p;
    strtod(line.c_str(), &p);
    return *p == 0;
}

/////Function to determine if a input is a number!
bool is_number(const std::string& s)
{
return( strspn( s.c_str(), "-.0123456789" ) == s.size() );

}
/////////////////take csv string of ints and return vector
vector<int> parse_csv_int(string s)
{
	std::stringstream ss(s);
	vector<int> vec;
	while( ss.good() )
	{
		string substr;
		getline( ss, substr, ',' );
		int sub;
		if(is_number(substr))
		{
 			sub = stoi(substr);
			vec.push_back(sub);
		}
	}

	return vec;
}
vector<string> parse_csv_str(string s)
{
	std::stringstream ss(s);
	vector<string> vec;
	while( ss.good() )
	{
		string substr;
		getline( ss, substr, ',' );
		string sub;
		sub = substr;
		vec.push_back(sub);
	}
	return vec;
}
/*
void setmcedit(int day=10, int mon=01, int year=2019)
{
	last_mc_edit.tm_year = year;
	last_mc_edit.tm_mon  = mon;
	last_mc_edit.tm_mday = day;
}*/


vector<double> parse_csv_d(string s, char delim=',')
{
	std::stringstream ss(s);
	vector<double> vec;
	while( ss.good() )
	{
		string substr;
		getline( ss, substr, delim );
		double sub;
		if(isnumber_1(substr))
		{
			if(substr.size()==0){substr="0";}
	 			sub = stod(substr);
				vec.push_back(sub);
		}
	}

	return vec;
}
double milRtoDeg(double mR){return mR*0.001 *(180/PI);}



//Class to open up a kin file and create strings for the information inside.
class kin_file {
	public:
		std::string target;std::vector<int> Oldrun;
		std::string kin_num;
		std::string run_string;
		vector<int> run_vect;
		int run_file_status=0;

	void set_file(std::string name, int debug = 0)
	{
		std::ifstream file(name);
  		if(file.fail()){cout << "Run list file does not exist !"<<endl; return;}
		run_file_status=1;
		TString content;

  		for (int ii=0; content.ReadLine(file) && ii<3;ii++ )
		{
    			if(debug==1){cout<<"!!!:  "<<content<<endl;}
    			if(ii==0)target = content;
       			if(ii==1){
           			kin_num = content.Data();
           		}
    			if(ii==2)run_string = content;
        	}
  		file.close();
		run_vect = parse_csv_int(run_string);


  	}
};
//////////////////////////////////////////////////////////////////////////////
class  corrections{
	public:
	std::vector<vector<double>> CT;
	std::vector<string> Labels;
	std::vector<int> ORs;
	int CS;
	//void Read_Table(std::string kin_target, std::string kin_num);

//};

void Read_Table(std::string kin_target, std::string kin_num, int debug=0) {
	//vectors to be used
	vector< vector <string > > table;
	vector< string > labels ;
	vector<data_type> d_type;
	vector<string> vec; vector<int> Oldrun;
  	vector< vector< double > > input_vec;
	vector <double> col;
	//input file
	std::fstream list;
  	std::string line_file;
  	list.open (Form("%s/kin%s/%s_kin%s.txt",kin_cor_loc.c_str(),kin_num.c_str(),kin_target.c_str(),kin_num.c_str()), std::fstream::in);
	if(!list.is_open()) {CS=0; cout << " Corrections file does not exist " <<endl; return; }
	CS=1; //does the files open
 	//////////////

	std::string stringrun;std::string::size_type sz;
  	int inrun=0; int status=0;  int innum=0;
  	int numint=0;	int num_of_inputs=0;

  	string delimiter = "\t";

	while (std::getline(list, line_file)){
    		vec.push_back(line_file);
		size_t pos = 0;	size_t pos1 = 0;
		int columns=1;
		std::string token;

		if(debug==1)cout <<numint<<" :  "<< innum << " "<< line_file <<endl;
    		while ((pos = vec[numint].find(delimiter)) != std::string::npos) {
			token = vec[numint].substr(0, pos);
			vec[numint].erase(0, pos + delimiter.length());
  	  		if(numint==1){labels.push_back(token); cout << token <<" :: \t";}
  	  		double in_num;
			if(is_number(token)) {
  	  			num_of_inputs++;
  	  	 		in_num=stod(token);
				col.push_back (in_num);
				d_type.resize(num_of_inputs);
				d_type[num_of_inputs-1].set_values(in_num,columns,labels[columns-1]);

				if(debug==1)cout <<labels[columns-1]<<":\t"<< token << ":\t";
     	  		}
			columns++;
		//	if(debug==1)cout<<endl;
		}
		if(debug)cout<<endl;
		input_vec.push_back(col);
     		col.clear();
    		numint++;
		stringrun="";
    		if(numint==1||numint==2)continue;
    		for(int jj =0; jj<10;jj++){
      			if(line_file[jj]== '\t'){break;}
      			stringrun+=line_file[jj];
    		}

    		inrun=stoi(stringrun,&sz);
    		size_t leng = stringrun.length();
       		Oldrun.push_back(inrun);
//		if(debug==1)cout<< "old run num : " <<inrun <<endl;
    		innum++;
     	}
  	list.close();
	if(input_vec.size()>2)input_vec.erase(input_vec.begin());
	if(input_vec.size()>1)input_vec.erase(input_vec.begin());

	ORs=Oldrun;
	Labels=labels;
	CT=input_vec;
}
};
/*
inline vector<std::string> corrections_label(std::string kin_target, std::string kin_num) {
        //vectors to be used
        vector< string > labels ;
	vector< string > vec;
        vector<data_type> d_type;
        vector< vector< double > > input_vec;
        vector <double> col;
  	//input file
        std::fstream list;
        std::string line_file;
        list.open (Form("%s/kin%s/%s_kin%s.txt",kin_cor_loc.c_str(),kin_num.c_str(),kin_target.c_str(),kin_num.c_str()), std::fstream::in);
        if(!list.is_open()) { cout << " Corrections file does not exist " <<endl; return labels;}
      	std::string stringrun;std::string::size_type sz;
        int inrun=0; int status=0;  int innum=0;
        int numint=0;   int num_of_inputs=0;

        string delimiter = "\t";

        while (numint<=1&&std::getline(list, line_file)){
                vec.push_back(line_file);
                size_t pos = 0; size_t pos1 = 0;
                int columns=1;
                std::string token;
                while ((pos = vec[numint].find(delimiter)) != std::string::npos) {
                        token = vec[numint].substr(0, pos);
                        vec[numint].erase(0, pos + delimiter.length());
                        if(numint==1){labels.push_back(token);}
		}
		numint++;
	}
	list.close();
	return labels;
}
*/

inline TString Good_Electron_Cuts(TString ARM_inc="Left", int PID =1,int trigger=1, int tg_acc=1, int track=1, int beam=1)
{
//Need to use set_limits to set before hand.
/*		///Varibles defined for cuts
	TG_Theta_Max_inc = 0.04;//40mrad
	TG_Theta_Min_inc =-0.04;//40mrad
	TG_Phi_Max_inc = 0.030;//25mrad
	TG_Phi_Min_inc =-0.030;//25mrad
	TG_Dp_Max_inc = 0.04;//4%mrad
	TG_Dp_Min_inc =-0.04;//4%mrad
	TG_VZ_Max_inc = 0.1200;//4%mrad
	TG_VZ_Min_inc =-0.1200;//4%mrad
	P0_inc = 3.100; //GeV/c
	GC_Cut_inc = 2000.;
	EP_Cut_inc= 0.75;
	Main_Trigger_Left_inc = 2;
	Main_Trigger_Right_inc = 5;
	cur_sel_inc=0;
	beamon_min_inc=1;

*/
	TString Arm_inc,arm_inc;
	Int_t Main_Trigger_inc;
	TString cal_det_inc;
	TString cal_det_inc1;
	if(ARM_inc=="Left"){
		Arm_inc ="L";arm_inc="l";
		Main_Trigger_inc = Main_Trigger_Left_inc;
		cal_det_inc = "(L.prl1.e+L.prl2.e)/(L.gold.p*1000.)";
		cal_det_inc1 = "(L.prl1.e+L.prl2.e)/(HacL_D1_P0rb*1000.)";
		//cal_det_inc2 = "L.prl1.asum_c
		}
	else{
		Arm_inc="R";arm_inc="r";
		Main_Trigger_inc = Main_Trigger_Right_inc;
		cal_det_inc = "(R.ps.e+R.sh.e)/(R.gold.p*1000.)";
		cal_det_inc1 = "(R.ps.e+R.sh.e)/(HacR_D1_P0rb*1000.)";
		}



	TString Trigger_inc =Form("D%s.evtypebits>>%d&1",Arm_inc.Data(),Main_Trigger_inc);
	TString one_track_inc =Form("%s.tr.n==1",Arm_inc.Data());
	TString pid_cer_inc = Form("%s.cer.asum_c>%4.1f",Arm_inc.Data(),GC_Cut_inc);
	TString pid_cal_inc = Form("%s>%3.2f",cal_det_inc.Data(),EP_Cut_inc);
	TString pid_cal_inc1 = Form("%s>%3.2f",cal_det_inc1.Data(),EP_Cut_inc);
	TString target_z_inc = Form("rp%s.z>%4.3f && rp%s.z<%4.3f ",arm_inc.Data(),TG_VZ_Min_inc,arm_inc.Data(),TG_VZ_Max_inc);
	TString target_ph_inc = Form("(%s.tr.tg_ph)>%4.3f && (%s.tr.tg_ph)<%4.3f",Arm_inc.Data(),TG_Phi_Min_inc,Arm_inc.Data(), TG_Phi_Max_inc);
	TString target_th_inc = Form("(%s.tr.tg_th)>%4.3f &&(%s.tr.tg_th)<%4.3f",Arm_inc.Data(),TG_Theta_Min_inc,Arm_inc.Data(),TG_Theta_Max_inc);
	TString track_dp_inc = Form("(%s.tr.tg_dp)>%4.3f && (%s.tr.tg_dp)<%4.3f",Arm_inc.Data(),TG_Dp_Min_inc,Arm_inc.Data(),TG_Dp_Max_inc);
	TString beam_trip_inc = Form("%sBCM.BeamUp_time_v1495[%d] >= %f",ARM_inc.Data(),cur_sel_inc,beamon_min_inc);

	TString cuts ="";
	if(trigger)cuts = Trigger_inc;
	if(PID==1)cuts+="&&"+pid_cer_inc+"&&"+pid_cal_inc;
		else if(PID==2)cuts+="&&"+pid_cer_inc+"&&"+pid_cal_inc1;
		else if(PID==3)cuts+="&&"+pid_cal_inc1;
	if(tg_acc)cuts+="&&"+target_z_inc+"&&"+target_ph_inc+"&&"+target_th_inc+"&&"+track_dp_inc;
	if(track)cuts+="&&"+one_track_inc;
	if(beam) cuts+="&&"+beam_trip_inc;

	TSubString sub = cuts.operator()(0,2);
	if(sub=="&&")cuts.Remove(0,2);


	return cuts;
}


void set_defaults()
{

			///Varibles defined for cuts
	TG_Theta_Max_inc = 0.04;//40mrad
	TG_Theta_Min_inc =-0.04;//40mrad
	TG_Phi_Max_inc = 0.030;//25mrad
	TG_Phi_Min_inc =-0.030;//25mrad
	TG_Dp_Max_inc = 0.04;//4%mrad
	TG_Dp_Min_inc =-0.04;//4%mrad
	TG_VZ_Max_inc = 0.1200;//4%mrad
	TG_VZ_Min_inc =-0.1200;//4%mrad
	P0_inc = 3.100; //GeV/c
	GC_Cut_inc = 1000.;
	EP_Cut_inc= 0.75;
	Main_Trigger_Left_inc = 2;
	Main_Trigger_Right_inc = 5;
	e_sample_int_inc = 3000.0;
	e_sample_m_inc= -3000.0/2300.0;
	p_sample_int_inc = 1500.0;
	p_sample_m_inc = -1.0;
	GC_e_sample_min_inc=4000;
	GC_e_sample_max_inc=6000;
	GC_p_sample_inc = 1000;
	cur_sel_inc=0;
	beamon_min_inc=1;

}

inline void set_limits(TString kin_tgt)
{

	//open file
	TString cor_kin = "---- "+kin_tgt;
	std::fstream limits_file;
  	std::string limits_line;
  	limits_file.open( Form("%skin_cut.dat",kin_cor_loc.c_str())	,std::fstream::in);
	if(!limits_file.is_open()){ cout <<Form("%skin_cut.dat",kin_cor_loc.c_str())<< " is not open:" <<endl; return;}
	// Define vectors to be used //
	vector<string> line_vector;
	vector<string> db_vector;
	vector<double> db_num;
	//read in file
	int j=0;
	while (std::getline(limits_file, limits_line)){
		line_vector.push_back(limits_line);							//add line to vector
		if(line_vector[j] == cor_kin){								//If correct kin
			int k=0;									//counter for cor kin
			while (std::getline(limits_file, limits_line)){					//get line of cor kin
				if(limits_line=="----") break;						//to find the end of cor kin
				db_vector.push_back(limits_line);					//add line to cor vec
				size_t pos=0;								//pos of the string
				string delim1="=";string delim2=";";					//what defines the input
				pos = db_vector[k].find(delim1);					//find the input
				if(pos> db_vector[k].length()){ k++; continue;}				//not a value line
				db_vector[k].erase(0, pos + delim1.length());				//restructure string
				pos = db_vector[k].find(delim2);					//find end of inpt
				db_vector[k].erase(pos, pos+ db_vector[k].length());			//restructure string
				if(is_number(db_vector[k])){ db_num.push_back(stod(db_vector[k]));}   	//set string as double
				k++;
			}
		}//end of correct kin
		else{
			set_defaults();
			return;
		}
	j++;} //end of input file

	if(db_num.size()<=1){return;}
	TG_Theta_Max_inc	=db_num[0];	//mrad
	TG_Theta_Min_inc 	=db_num[1];	//mrad
	TG_Phi_Max_inc 		=db_num[2];	//mrad
	TG_Phi_Min_inc 		=db_num[3];	//mrad
	TG_Dp_Max_inc 		=db_num[4];	//dp/p
	TG_Dp_Min_inc		=db_num[5];	//dp/p
	TG_VZ_Max_inc 		=db_num[6];	//meters
	TG_VZ_Min_inc 		=db_num[7];	//meters
	P0_inc 			=db_num[8]; 	//GeV/c
	GC_Cut_inc 		=db_num[9];    //adc channel
	EP_Cut_inc		=db_num[10];    // Deposit engery of p
	Main_Trigger_Left_inc 	=db_num[11];    // int 1-8
	Main_Trigger_Right_inc 	=db_num[12];    // int 1-8
	cur_sel_inc		=db_num[13];    // int 0-4
	beamon_min_inc		=db_num[14];	// uAs
 }// End of set limit function


inline bool GOOD_Event(TChain *T , int Event, int debug =0, TString ARM_inc = "Left", int PID =1, int tg_acc=1, int track=1, int beam=1)
 {

	TChain *tt= new TChain();
	tt->Add(T);
  	bool GE = 0; //Is this a good electron event
	double react_z, cal1, cal2, cer_asum, gold_p;
  	double track_n, trigger_bit ;
  	//Tree varibles
  	double Beam_up_inc[5], dp_inc,  dtheta_inc, dphi_inc;  	//Arm seperated issues
 	TString Arm_inc,arm_inc;
	Int_t Main_Trigger_inc;
	TString cal_det_inc;
	TString Cal_B_1, Cal_B_2;
	if(ARM_inc=="Left"){
		Arm_inc ="L";arm_inc="l";
		Main_Trigger_inc = Main_Trigger_Left_inc;
		cal_det_inc = "(L.prl1.e+L.prl2.e)/(L.gold.p*1000.)";
		Cal_B_1 = "L.prl1.e";
		Cal_B_2 = "L.prl2.e";

		}
	else{
		Arm_inc="R";arm_inc="r";
		Main_Trigger_inc = Main_Trigger_Right_inc;
		cal_det_inc = "(R.ps.e+R.sh.e)/(R.gold.p*1000.)";
		Cal_B_1 = "L.ps.e";
		Cal_B_2 = "L.sh.e";
		}

 	//Setting branches
 	tt->ResetBranchAddresses();
 	tt->SetBranchAddress(	Form("%sBCM.BeamUp_time_v1495"	,ARM_inc.Data())	,Beam_up_inc);
 	tt->SetBranchAddress(	Form("%s.tr.tg_dp"		,Arm_inc.Data())	,&dp_inc);
  	tt->SetBranchAddress(	Form("%s.tr.tg_th"		,Arm_inc.Data())	,&dtheta_inc);
   	tt->SetBranchAddress(	Form("%s.tr.tg_ph"		,Arm_inc.Data())	,&dphi_inc);
   	tt->SetBranchAddress(	Form("rp%s.z"			,arm_inc.Data())	,&react_z);
   	tt->SetBranchAddress(	Form("%s.tr.n"			,Arm_inc.Data())	,&track_n);
   	tt->SetBranchAddress(	Form("%s.gold.p"		,Arm_inc.Data())	,&gold_p);
   	tt->SetBranchAddress(	Form("%s.cer.asum_c"		,Arm_inc.Data())	,&cer_asum);
	tt->SetBranchAddress(	Form("%s"			,Cal_B_1.Data())	,&cal1);
	tt->SetBranchAddress(	Form("%s"			,Cal_B_2.Data())	,&cal2);
   	tt->SetBranchAddress(	Form("D%s.evtypebits"		,Arm_inc.Data())	,&trigger_bit);



	//Checking if Good
	tt->GetEntry(Event);
 	if( (int)trigger_bit>>Main_Trigger_inc&1)	{tt=nullptr;			return GE;} //Triggeri
 	if(track_n != 1)				{tt=nullptr;			return GE;} //tracking
 	if(Beam_up_inc[cur_sel_inc] <= beamon_min_inc) 	{tt=nullptr;			return GE;} //Beamtrip
 	if(react_z <= TG_VZ_Min_inc) 			{tt=nullptr;			return GE;} //End caps
 	if(react_z >= TG_VZ_Max_inc) 			{tt=nullptr;			return GE;} //End caps
	if( (cal1 +cal2)/(gold_p*1000) <= EP_Cut_inc)	{tt=nullptr;			return GE;} //PID energy
	if(cer_asum <= GC_Cut_inc )			{tt=nullptr;			return GE;} //PID cer
 	if(dp_inc 	<= TG_Dp_Min_inc || dp_inc >= TG_Dp_Max_inc) {tt=nullptr;	return GE;} //p acc
 	if(dtheta_inc <= TG_Theta_Min_inc || dtheta_inc >= TG_Theta_Max_inc) {tt=nullptr;return GE;} //theta acc
 	if(dphi_inc <= TG_Phi_Min_inc || dphi_inc >= TG_Phi_Max_inc) {tt=nullptr;	return GE;} //phi acc
	return 1;
}


double Bino_Err(double p, int n)
{
	return sqrt(p*(1-p)/(n*1.0));
}



vector<string> parse_std(string str,string delim=","){
	vector <string> vec_str;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delim)) != std::string::npos) {
		token = str.substr(0, pos);
		vec_str.push_back(token);
	    	str.erase(0, pos + delim.length());
	}

return vec_str;
}

vector<int> parse_int(string str,string delim=","){
	vector <int> vec_str;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delim)) != std::string::npos) {
		token = str.substr(0, pos);
		if(is_number(token)){
			vec_str.push_back(stoi(token));
		}
	    	str.erase(0, pos + delim.length());

	}

return vec_str;
}

vector<int> parse_int(string str,char delim=','){
	vector <int> vec_str;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delim)) != std::string::npos) {
		token = str.substr(0, pos);
		if(is_number(token)){
			vec_str.push_back(stoi(token));
		}
	    	str.erase(0, pos + 1);

	}

return vec_str;
}

double CalcCharge(int runnum, double cur_thres=4.0 , int debug=0){

	double charge=0.0;
	TChain *T = LoadRun(runnum);
	if(T==nullptr){cout << "somthing wrong with loadrun\n" ;
			return charge;}
	int entries = T->GetEntries();
	double Qdnew=0.0; 	Double_t Idnew=0.0;
	double dnew2=0.0;	Double_t renew=0;
	TString arm="Left";
	if(runnum>=8000)arm="Right";
	T->ResetBranchAddresses();
	T->SetBranchAddress(Form("%sBCMev.current_dnew",arm.Data()),&Idnew);
	T->SetBranchAddress(Form("%sBCMev.charge_dnew",arm.Data()),&Qdnew);
	T->SetBranchAddress(Form("%sBCMev.isrenewed",arm.Data()),&renew);
	if(debug)	cout << "num " << entries <<"\n";
	for(int i=0; i<entries; i++){
		T->GetEntry(i);
		if((int)renew  ==1  && Idnew>=cur_thres){
			charge+=(Qdnew-dnew2);
			}
		dnew2=Qdnew;
		Qdnew=0.0;Idnew=0.0;renew=0;
	}
	delete T;

	return charge;
}

double CalcCharge1(int runnum, double &current,double cur_thres=4.0 ){
	int debug=0;

	double charge=0.0;
	TChain *T = LoadRun(runnum);
	if(T==nullptr){cout << "somthing wrong with loadrun\n" ;
			return charge;}
	int entries = T->GetEntries();
	Double_t Qdnew=0.0; 	Double_t Idnew=0.0;
	double dnew2=0.0;	Double_t renew=0;
	TString arm="Left";
	if(runnum>=8000)arm="Right";
	T->ResetBranchAddresses();
	T->SetBranchAddress(Form("%sBCMev.current_dnew",arm.Data()),&Idnew);
	T->SetBranchAddress(Form("%sBCMev.charge_dnew",arm.Data()),&Qdnew);
	T->SetBranchAddress(Form("%sBCMev.isrenewed",arm.Data()),&renew);
	if(debug)	cout << "num " << entries <<"\n";
	double accum_current=0.0;
	int G_events=0;
	int prev_p=-1;
	for(int i=0; i<entries; i++){
		T->GetEntry(i);
		if((int)renew  ==1  && Idnew>=cur_thres && Idnew <= 50.0 && Qdnew==Qdnew){
		if(debug>1){
			double per = i/(entries*1.0)*1000;
			int peri = per/10;
			int modu = peri % 10;
			if( modu  == 0 && peri != prev_p)
			{
				prev_p=peri;
				cout << " current " << Idnew ;
				cout << " charge  " << charge ;
				cout << " avg cur " << accum_current/G_events;
				cout << " %\t"<< i/(entries*1.0) <<endl;
			}
		}

			accum_current+=Idnew;
			charge+=(Qdnew-dnew2);
			G_events++;
			}
		dnew2=Qdnew;
		Qdnew=0.0;Idnew=0.0;renew=0;
	}

	current = accum_current/(G_events*1.0);
	delete T;

	return charge;
}

double CalcChargeold(int runnum, double &current,double cur_thres=4.0 ){
	int debug=0;
	double gain = 0.000333;
	double charge=0.0;
	TChain *T = LoadRun(runnum);
	if(T==nullptr){cout << "somthing wrong with loadrun\n" ;
			return charge;}
	int entries = T->GetEntries();
	Double_t Qdnew=0.0; 	Double_t Idnew=0.0;
	double dnew2=0.0;	Double_t renew=0;
	double clock=0.0; 	double oclock=0.0;
	TString arm="Left";
	if(runnum>=8000)arm="Right";
	T->ResetBranchAddresses();
	T->SetBranchAddress(Form("ev%sdnew_r",arm.Data()),&Idnew);
	T->SetBranchAddress(Form("ev%sdnew",arm.Data()),&Qdnew);
	T->SetBranchAddress(Form("ev%sLclock",arm.Data()),&clock);
	if(debug)	cout << "num " << entries <<"\n";
	double accum_current=0.0;
	int G_events=0;
	int prev_p=-1;
	for(int i=0; i<entries; i++){
		T->GetEntry(i);
		Idnew*=gain;
		if(oclock  != clock  && Idnew>=cur_thres && Idnew <= 50.0 && Qdnew==Qdnew)
		{
			if(debug>1)
			{
				double per = i/(entries*1.0)*1000;
				int peri = per/10;
				int modu = peri % 10;
				if( modu  == 0 && peri != prev_p)
				{
					prev_p=peri;
					cout << " current " << Idnew ;
					cout << " charge  " << charge ;
					cout << " avg cur " << accum_current/G_events;
					cout << " %\t"<< i/(entries*1.0) <<endl;
				}
			}

			accum_current+=Idnew;
			charge+=(Qdnew-dnew2)*gain ;
			G_events++;
		}
		dnew2=Qdnew;
		oclock=clock;
		Qdnew=0.0;Idnew=0.0;
	}

	current = accum_current/(G_events*1.0);
	delete T;

	return charge;
}

double GetChargeSQL(int run, int thres=0){
	double charge=0.0;
	double current=0.0;

	string name  = Form("charge%d",thres);
	string cur_n = Form("current%d",thres);

        CODASetting    coda     = GetCODASetting(run);
       // AnalysisInfo  Ainfo     = GetAnalysisInfo(run);
       // if(Ainfo.status == -1){cout <<"Not in analysis DB \n"; exit(1);}
        TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
        TString        query    = Form("select run_number, %s, %s from %schargelist where run_number=%d;",name.c_str(),cur_n.c_str(), coda.experiment.Data(),run);

	TSQLResult* result=Server->Query(query.Data());
	if( result->GetRowCount()>0){
	  	TSQLRow *row= result->Next();
		charge = atof(row->GetField(1));
		current= atof(row->GetField(2));

	}
	else{
		cout << "run not in charge run list"<<endl;
	}

	return charge;
}


double GetChargeSQL(int run, double &current, int thres=0){
	double charge=0.0;

	string name  = Form("charge%d",thres);
	string cur_n = Form("current%d",thres);

        CODASetting    coda     = GetCODASetting(run);
       // AnalysisInfo  Ainfo     = GetAnalysisInfo(run);
       // if(Ainfo.status == -1){cout <<"Not in analysis DB \n"; exit(1);}
        TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
        TString        query    = Form("select run_number, %s, %s from %schargelist where run_number=%d;",name.c_str(),cur_n.c_str(), coda.experiment.Data(),run);

	TSQLResult* result=Server->Query(query.Data());
	if( result->GetRowCount()>0){
	  	TSQLRow *row= result->Next();
		charge = atof(row->GetField(1));
		current= atof(row->GetField(2));

	}
	else{
		cout << "run not in charge run list"<<endl;
	}

	return charge;
}


const Double_t Qe=TMath::Qe();
const Double_t Na=TMath::Na();
const Double_t CMtoNB=1.0e33;



vector<double> Calc_lum(int run,int debug=1){
     vector<double> lum = {1.0 , 0.0 };
     double lumin=0;      double lum_err=0;      Double_t charge=0;
     double den_cor=1.0;    double atomicMass=1;   double current=0;
     double cor_parameters[2]={0.0,0.0}; double den_cor_err=0;
     double cor_errs[2]= {0.0,0.0}; 	double charge_err=0;
     string tgt="";
     double tgt_thick=1.0;
     double thick_err=0.0;
     double charge1=0.0;


	charge = GetChargeSQL(run,current,0);

	TargetInfo  TI = GetTargetInfo("",-999,run);
	tgt = TI.name;
	tgt_thick =TI.Thickness;
	thick_err =TI.Thickness_err;

     double charge_E = charge / ( Qe*1e6);
     charge_err = 0.005; //Using 1% for now... Need to fix
     double Ierr = 0.01*current; //Using 1% for now... Need to fix

     if(debug)	cout << tgt<<": " << charge <<"  " <<current<<" \n";

     if(tgt == "Tritium"|| tgt == "Tritium ") atomicMass = 3.016;
     else if(tgt == "Helium-3") atomicMass = 3.016;
     else if(tgt == "Deuterium")atomicMass = 2.014102;
     else if(tgt == "Hydrogen") atomicMass = 1.007947;
     else if(tgt=="Carbon")	atomicMass = 12.01;

	den_cor = DensityCor(den_cor_err, run, current,debug);

    if(debug) cout <<"\n density cor " << den_cor <<"  " << den_cor_err<<"\n\n";
     lumin = (charge_E*tgt_thick*den_cor*Na/atomicMass)/CMtoNB;

	/// Error on charge , error on thick , error on den_cor
     lum_err = sqrt( pow(charge_err,2) +
		pow((thick_err/(tgt_thick*1.0)),2)  +
		pow(den_cor_err,2) );

    if(debug) cout << " luminosity " << lumin << " err% " <<lum_err<<"  thick_err% " << thick_err/tgt_thick << "\n";//  den_error% "<< den_cor_err/den_cor<<"\n";
     lum[0]=lumin;
     lum[1]=lum_err;
     return lum;
}


struct RunDBInfo{
	double angle=0.0;
	double p0=0.0;
	double ebeam=0.0;
	vector<double> offset ={0.0,0.0,0.0};
};

RunDBInfo GetRunDBInfo(int run){
	RunDBInfo RunDB;
	string ser = Form("run %d",run);

	string run_db_name= "/home/jbane/tritium/replay/HallA-Online-Tritium/replay/DB/db_run.dat";
	string line;
	ifstream Inf;
	Inf.open(run_db_name);
	if(!Inf)return RunDB;
	size_t pos;
	while(Inf.good())
	{
		getline(Inf,line);
		if(line[0] == '#')
		{
			pos=line.find(ser);
			if(pos!=string::npos)
			{
			//Found the entry!
				while(line[0]!='-'){
					getline(Inf,line);
					if(line.find("ebeam")!=string::npos)
					{
						pos=line.find("=");
						string sub = line.substr(pos+1);
						RunDB.ebeam=atof(sub.c_str());
					}
					if(line.find("theta")!=string::npos)
					{
						pos=line.find("=");
						string sub = line.substr(pos+1);
						RunDB.angle=atof(sub.c_str());
					}
					if(line.find("pcentral")!=string::npos)
					{
						pos=line.find("=");
						string sub = line.substr(pos+1);
						RunDB.p0=atof(sub.c_str());
					}
					if(line.find("off_x")!=string::npos)
					{
						pos=line.find("=");
						string sub = line.substr(pos+1);
						RunDB.offset[0]=atof(sub.c_str());
					}
					if(line.find("off_y")!=string::npos)
					{
						pos=line.find("=");
						string sub = line.substr(pos+1);
						RunDB.offset[1]=atof(sub.c_str());
					}
					if(line.find("off_z")!=string::npos)
					{
						pos=line.find("=");
						string sub = line.substr(pos+1);
						RunDB.offset[2]=atof(sub.c_str());
					}

				}//end of while loop
			break;
			}//end of run
		}//end of #

	}//end of first read

	return RunDB;
}


double ECC_Cor(int kin, string tgt){
	double ECC=0.0;
	int col=0;
	int row=-1;
	if(tgt=="H3"){col=1;}
	else if(tgt=="He3"){col=2;}
	else if(tgt=="D2"){col=3;}
	else if(tgt=="H"){col=4;}
	else if(tgt=="C12"){return 0.0;}

	if(ECC_table.size()==0)
	{//read in table;
		ifstream in;
		in.open("/home/jbane/Documents/ECC_table.csv");
		string line;
		getline(in,line);//titles

		while(in.good())
		{
			getline(in,line);
			vector<double> tmp = parse_csv_d(line);
			ECC_table.push_back(tmp);

		}
	}
	for(unsigned int i=0; i<ECC_table.size();i++)
	{
		if((int)ECC_table[i][0]==kin){row=i;break;}
	}
	if(row==-1){return ECC;}


	ECC=ECC_table[row][col];

	return ECC;
}



double RC_factor(int kin, string tgt, double E, double Ep, double theta){
	double RCF=1.0;
	if(RC_table.size()==0 || RC_tgt!=tgt || RC_kin!=kin){
		//read in table;
		cout << "read in new RC table" <<endl;
		int tarid=0;
		if(tgt=="D2")tarid=2;
		else if(tgt=="H3")tarid=3;
		else if(tgt=="He3")tarid=4;
		else if(tgt=="C12")tarid=7;
		else if(tgt=="H")tarid=1;
		string file=Form("/home/jbane/tritium/T2_externals/OUT/kin%d_%s_%d.out",kin,tgt.c_str(),tarid);
		ifstream in;
		in.open(file);
		string line="";
		if(!in.good())
		{ cout <<"Bad table" <<endl; return 1.0;}

		double ind;
		vector<double> tmp;
		while(in.good()){
			for(int i=0;i<5;i++){
				in>>ind;
				tmp.push_back(ind);
				}
			RC_table.push_back(tmp);
			tmp.clear();
		}
		RC_tgt=tgt;
		RC_kin=kin;
	}

	double Ep_sel[4]={0.0};
	double theta_sel[4]={0.0};
	double RC_sel[4]={0.0};

	double ediff=1000;
	double thdiff=100;
	double thstep=0.1;
	double thstep_1=0.1;
	double Epstep=0.01;

	int thF=0;
	double thT=0;
	//find the theta boundries
	for(unsigned int i=0;i<RC_table.size();i++)
	{

		thT=RC_table[i][2];
		thdiff=abs(theta-thT);
		if(thT < theta){
			if(thdiff<=thstep){
				theta_sel[0]=thT;
					}

		}
		else{
			if(thdiff<=thstep_1){
				thstep_1=thdiff;
				theta_sel[1]=thT;
					}
		}
	}

	for(unsigned int i=0;i<RC_table.size();i++)
	{
		thT=RC_table[i][2];
		if(abs(thT-theta_sel[0]) < thstep){
			if(RC_table[i][1] < Ep){
				if(abs(RC_table[i][1] - Ep) < Epstep){
					RC_sel[0]=RC_table[i][4];
					Ep_sel[0]=RC_table[i][1];
				}
			}
			else{
				if(abs(RC_table[i][1] -Ep) <Epstep){
					RC_sel[1]=RC_table[i][4];
					Ep_sel[1]=RC_table[i][1];
				}
			}
		}
		if(abs(thT-theta_sel[1]) < thstep){
			if(RC_table[i][1] < Ep){
				if(abs(RC_table[i][1] - Ep) < Epstep){
					RC_sel[2]=RC_table[i][4];
					Ep_sel[2]=RC_table[i][1];
				}
			}
			else{
				if(abs(RC_table[i][1] -Ep) <Epstep){
					RC_sel[3]=RC_table[i][4];
					Ep_sel[3]=RC_table[i][1];
				}
			}
		}
	}

	//iterperlate between the 4 points
//	cout << RC_sel[0] << " "<< RC_sel[1] << " "<< RC_sel[2] << " "<< RC_sel[3] << " \n";

//	cout << theta_sel[0] <<" "<< theta_sel[1]<<endl;

	double RC1 = (RC_sel[1]*(Ep-Ep_sel[0]) + RC_sel[0]*(Ep_sel[1]-Ep))/(Ep_sel[1]-Ep_sel[0]);
	double RC2 = (RC_sel[3]*(Ep-Ep_sel[2]) + RC_sel[2]*(Ep_sel[3]-Ep))/(Ep_sel[3]-Ep_sel[2]);
//	cout <<RC1<<" "<<RC2 <<endl;

	RCF= (RC2*(theta-theta_sel[0]) + RC1*(theta_sel[1]-theta) )/(theta_sel[1]-theta_sel[0]) ;


//	cout << theta-theta_sel[0] << " " << theta_sel[1]-theta << " "<< theta_sel[1]-theta_sel[0] <<endl;



	return RCF;
}


vector<double> beam_info(TTree *T, int debug=1)
{
  TH1F *h_x = new TH1F("h_x","rbx",2500,-5,5);
  TH1F *h_y = new TH1F("h_y","rby",2500,-5,5);
  THaRun *RI = GetRunData(T);
  int run =RI->GetNumber();
  CODASetting coda = GetCODASetting(run);
  string arm = coda.arm.Data();
  TCut cut="1";
  if(arm == "L"){  cut = electron_cut_L;}
  else { cut = electron_cut_R;}

	TString g_op="goff";
	string can_name=Form("c_%d",run);


	if( gROOT->FindObject(Form("%s",can_name.c_str())) !=nullptr)
	{		can_name+="_1";	}

  TCanvas *C= new TCanvas(Form("%s",can_name.c_str()),Form("run %d",run));
	if(debug>=2)
	{
		g_op="";

		C->Divide(1,2);
		C->cd(1);
	}
	double res=1.8;
	double x_mean, y_mean, x_min, x_max, y_min, y_max;

  T->Draw("Lrb.x*1000>>h_x",cut,g_op);
	x_mean = h_x->GetMean();

	if(T->GetEntries(cut) <3000)
	{
		cut = "1";
		T->Draw("Lrb.x*1000>>h_x",cut,g_op);
	}
	if(T->GetEntries(cut) <6000)
	{
		if(debug)cout << "using first and last bin for x" <<endl;
		x_min  = h_x->GetBinCenter( h_x->FindFirstBinAbove(10));
		x_max  = h_x->GetBinCenter( h_x->FindLastBinAbove(10));
		TLine *lx1 = new TLine(x_min,0,x_min,h_x->GetMaximum());
		TLine *lx2 = new TLine(x_max,0,x_max,h_x->GetMaximum());
		lx1->SetLineColor(2);lx2->SetLineColor(2);
		lx1->Draw(Form("same %s",g_op.Data()));
		lx2->Draw(Form("same %s",g_op.Data()));
	}
	else{
		 TSpectrum *s = new TSpectrum(5,1);
	  s->Search(h_x,res,g_op,0.8);
	  double *peaks = s->GetPositionX();
		if(debug) {for(int i=0; i<s->GetNPeaks();i++){cout <<peaks[i]<<"  ";}cout<<"\n";}
		int num_x_s=1;
		while(s->GetNPeaks() <2){
			double thres=(0.7-(0.05*num_x_s));
			s->Search(h_x,res,g_op,thres);
			peaks= s->GetPositionX();
			if(debug) {
				cout << "Search again with threshold of " <<thres<<endl;
				for(int i=0; i<s->GetNPeaks();i++){cout <<peaks[i]<<"  ";}cout<<"\n";
			}
			if(num_x_s>=14)break;
			num_x_s++;
		}
		if(s->GetNPeaks() <2)cout << "Not enough peaks for x"<<endl;

	  x_min = peaks[0];
	  x_max = peaks[1];
	  if(x_min>x_max){double tmp = x_min; x_min=x_max;x_min=tmp;}
	}
	if(x_max - x_min < 1.0){
		if(debug) cout << "bad spectrum"<<endl;
		x_min  = h_x->GetBinCenter( h_x->FindFirstBinAbove(10));
		x_max  = h_x->GetBinCenter( h_x->FindLastBinAbove(10));
		TLine *lx1 = new TLine(x_min,0,x_min,h_x->GetMaximum());
		TLine *lx2 = new TLine(x_max,0,x_max,h_x->GetMaximum());
		lx1->SetLineColor(2);lx2->SetLineColor(2);

			lx1->Draw(Form("same %s",g_op.Data()));
			lx2->Draw(Form("same %s",g_op.Data()));

	}


	if(debug) cout <<" Y time"<<endl;

	if(debug>=2)C->cd(2);

	if(arm == "L"){  cut = electron_cut_L;}
	else { cut = electron_cut_R;}

  T->Draw("Lrb.y*1000>>h_y",cut,g_op);
	y_mean = h_y->GetMean();
	if(T->GetEntries(cut) <1000)
	{
		cut = "1";
		T->Draw("Lrb.y*1000>>h_y",cut,g_op);
	}
	if(T->GetEntries(cut) <3000)
	{
		if(debug)cout << "using first and last bin for y" <<endl;
		y_min  = h_y->GetBinCenter( h_y->FindFirstBinAbove(10));
		y_max = h_y->GetBinCenter( h_y->FindLastBinAbove(10));
		TLine *ly1 = new TLine(y_min,0,y_min,h_y->GetMaximum());
		TLine *ly2 = new TLine(y_max,0,y_max,h_y->GetMaximum());
		ly1->SetLineColor(2);ly2->SetLineColor(2);
		ly1->Draw(Form("same %s",g_op.Data()));
		ly2->Draw(Form("same %s",g_op.Data()));
	}
	else{
	  TSpectrum *sy = new TSpectrum(5,1);
	  sy->Search(h_y,res,g_op,0.8);
		int num_y_s=1;
		double *peaksy = sy->GetPositionX();
		if(debug) {for(int i=0; i<sy->GetNPeaks();i++){cout <<peaksy[i]<<"  ";}cout<<"\n";}
		while(sy->GetNPeaks() <2){
			double thres=(0.7-(0.05*num_y_s));
			sy->Search(h_y,res,g_op,thres);
			peaksy= sy->GetPositionX();
			if(debug) {
				cout << "Search again with threshold of " <<thres<<endl;
				for(int i=0; i<sy->GetNPeaks();i++){cout <<peaksy[i]<<"  ";}cout<<"\n";}
			if(num_y_s>=14)break;
			num_y_s++;
		}
		if(sy->GetNPeaks() <2)cout << "Not enough peaks for y"<<endl;
  	y_min = peaksy[0];
  	y_max = peaksy[1];
		if(y_min>y_max){double tmp = y_min; y_min=y_max;y_min=tmp;}
	}
	if(x_max - x_min < 1.0){
		if(debug) cout << "bad spectrum"<<endl;
		y_min  = h_y->GetBinCenter( h_y->FindFirstBinAbove(10));
		y_max = h_y->GetBinCenter( h_y->FindLastBinAbove(10));
		TLine *ly1 = new TLine(y_min,0,y_min,h_y->GetMaximum());
		TLine *ly2 = new TLine(y_max,0,y_max,h_y->GetMaximum());
		ly1->SetLineColor(2);ly2->SetLineColor(2);
		if(debug>1){
			ly1->Draw(Form("same %s",g_op.Data()));
			ly2->Draw(Form("same %s",g_op.Data()));
		}
	}


  double size_x = abs(x_max-x_min);
  double size_y = abs(y_max-y_min);

  vector<double> beamI;
  beamI.push_back(size_x/10.0);
  beamI.push_back(size_y/10.0);
  beamI.push_back(x_mean/10.0);
  beamI.push_back(y_mean/10.0);
	//In cm!!!!

	if(debug>=3)
	{
		C->SaveAs(Form("./images/beampos_%d.png",run));
	}

	if(debug<=2){
		C=new TCanvas;
	 	delete C;}

  return beamI;
}

vector<double> beam_info(int run, int debug=1)
{
  TTree *T = LoadRun(run);
  vector<double>beamI = beam_info(T,debug);
  return beamI;
}

/**
 * Get the size of a file.
 * @param filename The name of the file to check size for
 * @return The filesize, or 0 if the file does not exist.
 */
size_t getFilesize(const std::string& filename) {
    struct stat st;
    if(stat(filename.c_str(), &st) != 0) {
        return 0;
    }
    return st.st_size;
}

long getlastmod(string filename)
{
	struct stat st;
  if(stat(filename.c_str(), &st) != 0) {
        return 0;
    }
 return st.st_mtime;
}

bool isnewer(string filename, string check_date =Form( "%slast_edit" ,T2_MC_Dir.c_str()) )
{

	auto filetime =getlastmod(filename);
	auto checktime = getlastmod(check_date);
	return (checktime<filetime);
}


double GetClkCount(TChain *T)
{
	double total_clkcnt=0;
	T->SetBranchAddress("V1495ClockCount",&total_clkcnt);
	int all_ent = T->GetEntries();
  T->GetEntry(all_ent-1);
	double clkcnt = total_clkcnt;
	T->ResetBranchAddresses();
	return clkcnt;

}


TBox * NewEBox(double x1, double y1, double x2, double y2 )
{
	TBox *newbox =  new TBox(x1,y1,x2,y2);
	newbox->SetFillColor(7);
	newbox->SetFillStyle(3013);
	return newbox;
}

TBox * NewNEBox(double x1, double y1, double x2, double y2 )
{
	TBox *newbox = new TBox(x1,y1,x2,y2);
	newbox->SetFillColor(2);
	newbox->SetFillStyle(3014);
	return newbox;
}

TLegend * Boxleg(TBox *A, TBox *B)
{
		TLegend *leg = new TLegend(0.3,0.9,0.7,0.975);
		leg->AddEntry(A,"Non-Es","f");
		leg->AddEntry(B,"Electrons","f");
		leg->SetNColumns(2);
		leg->SetFillStyle(0);

		return leg;
}

void Drawallbox(double x1, double y1, double x2, double y2, double xx1, double yx1, double xx2, double yx2)
{
	TBox *EB = NewEBox(x1,y1,x2,y2);
	TBox *NEB = NewNEBox(xx1,yx1,xx2,yx2);
	TLegend *BoxL = Boxleg(NEB,EB);
	EB->Draw("same");
	NEB->Draw("same");
	BoxL->Draw("same");
}

TCanvas *WSC(string var,string name)
{
	return new TCanvas(Form("%s",var.c_str()), Form("%s",name.c_str()), 1300,700);
}


/*
string unparseD(vector<double> A)
{
	string B="";
	for(unsigned int i=0; i<A.size();i++)
	{
		B+=Form("%d,"A[i]);
	}
	return B;
}
*/

/*
//   TSQLServer* Server = TSQLServer::Connect("mysql://halladb/triton-work","triton-user","3He3Hdata");
//    TString  query=Form("select run_number, current, charge from MARATHONanalysis where run_number=%d order by charge desc;",run);
//     TSQLResult* result=Server->Query(query.Data());
//    TSQLRow *row;
//     if(result->GetRowCount()>0){
//	     row = result->Next();
//	     current= atof(row->GetField(1));
 //    	     charge = atof(row->GetField(2));
	//}
	//else{

	//	cout << "Not in analysis list calculating event by event\n";
	charge1=CalcCharge(run,dnew_thres);
//	}

       if(debug) cout << "current " << current << "  charge " << charge<<"  "<<charge1 << "\n";
	charge=charge1;
     TString  query1=Form("select target from MARATHONrunlist where run_number=%d;",run);
     TSQLResult* result1=Server->Query(query1.Data());
     TSQLRow *row1;
     if(result1->GetRowCount()>0){
	row1 = result1->Next();
	tgt = row1->GetField(0);
        if(debug) cout << "Target " << tgt << "\n";

     	TString  query2=Form("select density_par_1, density_err_1, density_par_2, density_err_2, Thickness , Thickness_err, type from MARATHONTargetInfo  where name='%s';",tgt.c_str());
     	TSQLResult* result2=Server->Query(query2.Data());
     	TSQLRow *row2;
     	if(result2->GetRowCount()>0){
	      	row2 = result2->Next();
		string type=row2->GetField(6);
		if(type=="gas"){
	     		cor_parameters[0]=atof(row2->GetField(0));
		     	cor_parameters[1]=atof(row2->GetField(2));
     			cor_errs[0]=atof(row2->GetField(1));
		     	cor_errs[1]=atof(row2->GetField(3));
			}
		     	tgt_thick = atof(row2->GetField(4));
			if(row2->GetField(5)==nullptr) thick_err = 0.0;
			else{ thick_err = atof(row2->GetField(5));}

		}
	}
	else{if(debug)cout<<"\n!!!!!!This run is not in runlist!!!!!!\n\n";}

     Server->Close();
//CLose the server
*/
