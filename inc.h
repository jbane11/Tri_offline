/*C/C++ Includes{{{*/
#include <time.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
/*}}}*/
/*ROOT Includes{{{*/
#include <TEventList.h>
#include <TCanvas.h>
#include <TVector.h>
#include <TLine.h>
#include <TSystem.h>
#include <TString.h>
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

	string kin_cor_loc = "/adaqfs/home/a-onl/tritium_work/Bane/Tri_offline/kin_txt/";

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
	double GC_Cut_inc = 2000.;
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



//classes
/*
/////////////Class to define an input.
class data_type {
	public:
	double value;
	int column;
	std::string type;
 	
	void set_values(double a, int b, std::string str);
	};
*/

/////Function to determine if a input is a number!
bool is_number(const std::string& s)
{
return( strspn( s.c_str(), "-.0123456789" ) == s.size() );

}



//Class to open up a kin file and create strings for the information inside. 
class kin_file {
	public:
		std::string target;vector<int> Oldrun;
		std::string kin_num;
		std::string run_string;
	
	void set_file(std::string name, int debug = 0){
		ifstream file(name);
  		TString content;

  		for (int ii=0; content.ReadLine(file) && ii<3;ii++ ) {  
    		if(debug==1){cout<<"!!!:  "<<content<<endl;}
    		if(ii==0)target = content;
       		if(ii==1){
           		kin_num = content.Data();
           	}
    		if(ii==2)run_string = content; 
        }
  		file.close();
  		
  		}
};
//////////////////////////////////////////////////////////////////////////////
/*

class corrections_table {
	public:
		vector< vector <string > > table;
		vector< string > labels ;
		vector<data_type> d_type;
		
	void make_table(TString kin_name){
		std::fstream list;
  		std::string line_file;
  		list.open (Form("%s/%s",kin_cor_std::fstream list;
  std::string line_file;
  list.open (Form("../kin_txt/kin%s/%s_kin%s.txt",kinfile.kin_num.c_str(),kinfile.target.c_str(),kinfile.kin_num.c_str()), std::fstream::in);loc,kin_name.Data()), std::fstream::in); 
	  	std::string stringrun;std::string::size_type sz;
  		int inrun=0; int status=0;  int innum=0;   
  		int numint=0;	int num_of_inputs=0;
		vector<string> vec; vector<int> Oldrun;
  		vector< vector< float > > input_vec;
  		vector <float> col;
  		string delimiter = "\t";
  		 
  		 	while (std::getline(list, line_file)){
    			vec.push_back(line_file);
				size_t pos = 0;	size_t pos1 = 0; r<RunNoChain.size();r++){
				int columns=1;
				std::string token;

    			while ((pos = vec[numint].find(delimiter)) != std::string::npos) {
					token = vec[numint].substr(0, pos);
					vec[numint].erase(0, pos + delimiter.length());
  	  				if(numint==1){labels.push_back(token);}
  	  				double in_num;
					if(is_number(token)) {
  	  	 				num_of_inputs++;
  	  	 				in_num=stod(token);  		  
						col.push_back (in_num);
						d_type.resize(num_of_inputs);
						d_type[num_of_inputs-1].set_values(in_num,columns,labels[columns-1]);
     	  				}
					columns++;
				}
				input_vec.push_back(col);
     			col.clear();
    			numint++;
    			if(numint==1||numint==2)continue;
    			for(int jj =0; jj<10;jj++){
      				if(line_file[jj]== ' '){break;}
      				stringrun+=line_file[jj];
    				}	
    			inrun=stoi(stringrun,&sz);
    			size_t leng = stringrun.length();
       			Oldrun.push_back(inrun);
    			innum++;
     		}
  			list.close();
  		}
};
		*/
				
		

inline TString Good_Electron_Cuts(TString kin_tgt="", TString ARM_inc="Left")
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
	GC_Cut_inc = 2000.;
	EP_Cut_inc= 0.75;
	Main_Trigger_Left_inc = 2;
	Main_Trigger_Right_inc = 5;
	cur_sel_inc=0;
	beamon_min_inc=1;
	TString Arm_inc,arm_inc;
	Int_t Main_Trigger_inc;
	TString cal_det_inc;
	if(ARM_inc=="Left"){
		Arm_inc ="L";arm_inc="l";
		Main_Trigger_inc = Main_Trigger_Left_inc;
		cal_det_inc = "(L.prl1.e+L.prl2.e)/(L.gold.p*1000.)";
		}
	else{
		Arm_inc="R";arm_inc="r";
		Main_Trigger_inc = Main_Trigger_Right_inc;
		cal_det_inc = "(R.ps.e+R.sh.e)/(R.gold.p*1000.)";
		}



	TString Trigger_inc =Form("D%s.evtypebits>>%d&1",Arm_inc.Data(),Main_Trigger_inc);
	TString one_track_inc =Form("%s.tr.n==1",Arm_inc.Data());
	TString pid_cer_inc = Form("%s.cer.asum_c>%4.1f",Arm_inc.Data(),GC_Cut_inc);
	TString pid_cal_inc = Form("%s>%3.2f",cal_det_inc.Data(),EP_Cut_inc);
	TString target_z_inc = Form("rp%s.z>%4.3f && rp%s.z<%4.3f ",arm_inc.Data(),TG_VZ_Min_inc,arm_inc.Data(),TG_VZ_Max_inc);
	TString target_ph_inc = Form("(%s.tr.tg_ph)>%4.3f && (%s.tr.tg_ph)<%4.3f",Arm_inc.Data(),TG_Phi_Min_inc,Arm_inc.Data(), TG_Phi_Max_inc);
	TString target_th_inc = Form("(%s.tr.tg_th)>%4.3f &&(%s.tr.tg_th)<%4.3f",Arm_inc.Data(),TG_Theta_Min_inc,Arm_inc.Data(),TG_Theta_Max_inc);
	TString track_dp_inc = Form("(%s.tr.tg_dp)>%4.3f && (%s.tr.tg_dp)<%4.3f",Arm_inc.Data(),TG_Dp_Min_inc,Arm_inc.Data(),TG_Dp_Max_inc);
	TString beam_trip_inc = Form("%sBCM.BeamUp_time_v1495[%d] >= %f",ARM_inc.Data(),cur_sel_inc,beamon_min_inc);
	
	TString cuts = Trigger_inc+"&&"+one_track_inc+"&&"+pid_cer_inc+"&&"+pid_cal_inc+"&&"+target_z_inc+"&&"+target_ph_inc;
	cuts += +"&&"+target_th_inc+"&&"+track_dp_inc+"&&"+beam_trip_inc;
	
	return cuts;
}
inline void set_limits(TString kin_tgt)
{
	//open file 
	TString cor_kin = "---- "+kin_tgt;
	//cout <<cor_kin<<endl;
	std::fstream limits_file;
  	std::string limits_line;
  	limits_file.open( Form("%skin_cut.dat",kin_cor_loc.c_str())	,std::fstream::in);
	//printf("%skin_cut.dat\n",kin_cor_loc.c_str());
	//cout << limits_file.is_open() <<endl;
	
	vector<string> line_vector;
	vector<string> db_vector;
	vector<double> db_num;
	
	//read in file
	int j=0;
	while (std::getline(limits_file, limits_line)){
		//cout << limits_line <<endl;	
		line_vector.push_back(limits_line);
		
		if(line_vector[j] == cor_kin){
			int k=0;
			cout << "check: " <<endl;
			while (std::getline(limits_file, limits_line)){
				if(limits_line=="----") break;
				
				db_vector.push_back(limits_line);
				size_t pos=0;//pos of the string 
				string delim1="=";string delim2=";";
				pos = db_vector[k].find(delim1);
				
				//cout << pos << " "<<db_vector[k]<<" " <<db_vector[k].length()<<endl;
				
				if(pos> db_vector[k].length()){ k++; continue;}
				db_vector[k].erase(0, pos + delim1.length());

				//cout <<db_vector[k]<<endl;
				
				pos = db_vector[k].find(delim2);
				
				//cout << pos << " " <<db_vector[k].length()<<endl;
				
				db_vector[k].erase(pos, pos+ db_vector[k].length());					
				
				//cout <<db_vector[k]<<endl;
				if(is_number(db_vector[k])){ db_num.push_back(stod(db_vector[k]));}
				
													
				k++;
			} 
			
		}
		j++;
	//find the correct line
	}
	//set limts
	

}



inline bool GOOD_Event(TChain *T , int Event, TString ARM_inc = "Left")
{
 	bool GE = 0; //Is this a good electron event
 	
 	//Tree varibles
 	double Beam_up_inc[5], dp_inc,  dtheta_inc, dphi_inc; 
 	double react_z, cal1, cal2, cer_asum, gold_p;
 	double track_n, trigger_bit ;
 	
 	//Arm seperated issues
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
 	T->ResetBranchAddresses();
 	T->SetBranchAddress(	Form("%sBCM.BeamUp_time_v1495"	,ARM_inc.Data())	,Beam_up_inc); 	
 	T->SetBranchAddress(	Form("%s.tr.tg_dp"			  	,Arm_inc.Data())	,&dp_inc); 	
  	T->SetBranchAddress(	Form("%s.tr.tg_th"			  	,Arm_inc.Data())	,&dtheta_inc); 	
   	T->SetBranchAddress(	Form("%s.tr.tg_ph"				,Arm_inc.Data())	,&dphi_inc);
   	T->SetBranchAddress(	Form("rp%s.z"					,arm_inc.Data())	,&react_z);
   	T->SetBranchAddress(	Form("%s.tr.n"					,Arm_inc.Data())	,&track_n);
   	T->SetBranchAddress(	Form("%s.gold.p"				,Arm_inc.Data())	,&gold_p);
   	T->SetBranchAddress(	Form("%s.cer.asum_c"			,Arm_inc.Data())	,&cer_asum);
	T->SetBranchAddress(	Form("%s"						,Cal_B_1.Data())	,&cal1);
	T->SetBranchAddress(	Form("%s"						,Cal_B_2.Data())	,&cal2);
   	T->SetBranchAddress(	Form("D%s.evtypebits"			,Arm_inc.Data())	,&trigger_bit);
   
 
   	
	//Checking if Good
	T->GetEntry(Event);  	
 	if( (int)trigger_bit>>Main_Trigger_inc&1)								return GE; //Trigger
 	if(track_n != 1)														return GE; //tracking
 	if(Beam_up_inc[cur_sel_inc] <= beamon_min_inc) 							return GE; //Beamtrip
 	if(react_z <= TG_VZ_Min_inc) 							 				return GE; //End caps
 	if(react_z >= TG_VZ_Max_inc) 											return GE; //End caps
	if( (cal1 +cal2)/(gold_p*1000) <= EP_Cut_inc)							return GE; //PID energy
	if(cer_asum <= GC_Cut_inc )												return GE; //PID cer
 	if(dp_inc 	<= TG_Dp_Min_inc || dp_inc >= TG_Dp_Max_inc) 				return GE; //p acc
 	if(dtheta_inc <= TG_Theta_Min_inc || dtheta_inc >= TG_Theta_Max_inc) 	return GE; //theta acc	
 	if(dphi_inc <= TG_Phi_Min_inc || dphi_inc >= TG_Phi_Max_inc) 			return GE; //phi acc	 

 	

	  	
 	
 	
 	
	return GE;
}







