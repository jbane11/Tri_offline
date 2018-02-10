#include "TRI_Tools.h" //Used for th RunNoChain to Tchain all of the TTrees together

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
const int Main_Trigger_Left = 2;
const int Main_Trigger_Right = 5;


void Yield_kin( TString filename=""){

	//input the name opf the kin file and parse it
	if(filename==""){ cout<<"Input file name:   "; cin>>filename;}
	filename = "./Runlist/" + filename;
    ifstream file(filename.Data());
    if(!file.good()){cout << filename.Data() << "does not exist! "<<endl; exit(1);}
    TString content;
    TString Target,Kin,Run_String;
    const char* kin;
    for (int ii=0; content.ReadLine(file) && ii<3;ii++ ) {  
    	cout<<"!!!:  "<<content<<endl;
        if(ii==0)Target = content;
        if(ii==1){
           kin = content.Data();
           Kin = Form("Kin%s", kin);
         }
        if(ii==2)Run_String = content;         
      }
      file.close();
   ///////////////////////////////////////////////////
   ///Call tri_tools functions and combine root files
    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
	TChain* T =(TChain*) gGetTree(RunNoChain, "T");
   //////////////////////////////////////////////////
	TString ARM,Arm,arm;
	Int_t Main_Trigger;
	TString cal_det;
	if(RunNoChain[0]<90000){ARM="Left";
							Arm ="L";arm="l";
							Main_Trigger = Main_Trigger_Left;
							cal_det = "(L.prl1.e+L.prl2.e)/(L.gold.p*1000.)";}
						else{ARM="Right";
							 Arm="R";arm="r";
							 Main_Trigger = Main_Trigger_Right;
							cal_det = "(R.ps.e+R.sh.e)/(R.gold.p*1000.)";}


	T->ResetBranchAddresses();
	TString Trigger =Form("D%s.evtypebits>>%d&1",Arm.Data(),Main_Trigger);
	TString one_track =Form("%s.tr.n==1",Arm.Data());
	TString pid_cer = Form("%s.cer.asum_c>%4.1f",Arm.Data(),GC_Cut);
	TString pid_cal = Form("%s>%3.2f",cal_det.Data(),EP_Cut);
	TString target_z = Form("rp%s.z>%4.3f && rp%s.z<%4.3f ",arm.Data(),TG_VZ_Min,arm.Data(),TG_VZ_Max);
	TString target_ph = Form("(%s.tr.tg_ph)>%4.3f && (%s.tr.tg_ph)<%4.3f",Arm.Data(),TG_Phi_Min,Arm.Data(), TG_Phi_Max);
	TString target_th = Form("(%s.tr.tg_th)>%4.3f &&(%s.tr.tg_th)<%4.3f",Arm.Data(),TG_Theta_Min,Arm.Data(),TG_Theta_Max);
	TString track_dp = Form("(%s.tr.tg_dp)>%4.3f && (%s.tr.tg_dp)<%4.3f",Arm.Data(),TG_Dp_Min,Arm.Data(),TG_Dp_Max);
	TString beam_trip = Form("%sBCM.current_dnew >= 1.0",ARM.Data());
	
	TString pid = pid_cer +"&&"+ pid_cal;
	TString target = target_z+"&&"+target_ph+"&&"+target_th;
	TString track = one_track+"&&"+track_dp;
	TString good_e = Trigger+"&&"+pid+"&&"+target+"&&"+track+"&&"+beam_trip;


	
	Double_t Total_charge = gROOT->ProcessLine(Form(".x Total_Charge_kin.C(\"%s\",%d)",ARM.Data(),1));
	
	
	T->Draw("EKLx.x_bj>>h1(100,0,1)",Form("%f*(%s)",1/Total_charge,good_e.Data()));


}



