////////////////////////////////////////////////////////////////////////
// MARATHON Good-Electrons Countor
//  -- Zhihong Ye
//
//Note: If you change anything, please update bellow on what you did;
//      Highly recommend to ask Zhihong to add or change features.
//
//  * Updated on 02/05/2018 03:58PM by Zhihong
//     Add few more lines for ultra-long run-list what can go out of screen
//
//  * Updated on 02/07/2018 11:46PM by Zhihong
//     Change the E/P Cut on HRS-R from 0.6 to 0.95, before it is calibrated. 
//
////////////////////////////////////////////////////////////////////////

#include "TRI_Main.h"
#include "TRI_Tools.h"
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
const double EP_Cut_R = 0.80;
const double EC2_Cut = 300; //L.prl2.e, R.sh.e
const int Main_Trigger_Left = 2;
const int Main_Trigger_Right = 5;

//void Electron_Count(){
void Electron_Count_1(int run =0, TString aInputFileName ="" ){
	const double xB[16] = {0.218, 0.258, 0.298, 0.338, 0.378, 0.418, 0.458, 0.498, 0.538, 0.578, 0.618, 0.658, 0.698,0.738, 0.778, 0.818 };
	//TString aInputFileName="";
	if(aInputFileName==""){
		cout<<" --- Input File = ";cin >> aInputFileName;
	}
	aInputFileName = "./Runlist/"+aInputFileName;

	/*Read InputFile{{{*/
	//TString aInputFileName = ""; cout<<"--- Input File Name : "; cin >> aInputFileName;
	// int kin = 0; cout<<"--- Kin (1 or 2 or 3 ...) : "; cin >> kin;
	cout   << endl << Form("@@@@@@ Initialize Input values from %s ...",aInputFileName.Data())<<endl;
	Int_t aline=0;
	const TString Target=gGet_InputFile_Var(aInputFileName,aline);aline++; cout << "   Target Name = " << Target<<endl; 
	const int kin=gGet_InputFile_Var(aInputFileName,aline).Atoi();aline++;                cout << Form("   Kin = Kin%d ", kin)<<endl; 
	TString Kin = Form("Kin%d", kin);

	

	TString Run_String = gGet_InputFile_Var(aInputFileName,aline);
	const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);aline++;
	const int NumberOfRun = 1;//RunNoChain.size();
	cout << "   Run-List = " ;//<< Run_String<<endl;
	for(int i=0;i<NumberOfRun;i++)
		cout<<RunNoChain[i]<<" ";
	cout<<endl;
	/*}}}*/
cout << "debug 1" <<endl;
	TString Arm="";// = gGet_InputFile_Var(aInputFileName,aline);aline++; 
	if(run>50000)
		Arm = "R";
	else
		Arm = "L";
	
	TString runstring = TString::Format("%d",run);


	cout << "debug 1.5" <<endl;
	TString rootfilePath="/chafs1/work1/tritium/Rootfiles/bqual/";

	if(kin<=7){
		rootfilePath = "/volatile/halla/triton/Tritium_Rootfiles/";}


		TFile *file = new TFile(Form("%sbeam_qual_%d.root",rootfilePath.Data(),run),"read");
		TTree  *T_Tree = (TTree*)file->Get("T"); 

cout << "debug 2" <<endl;

	TString cut_L = Form("DL.evtypebits>>%d&1",Main_Trigger_Left);
	TString cut_e_L = Form("DL.evtypebits>>%d&1 && L.tr.n==1 && L.cer.asum_c>%4.1f && (L.prl1.e+L.prl2.e)/(L.gold.p*1000.)>%3.2f",Main_Trigger_Left, GC_Cut, EP_Cut);
	TString cut_good_e_L = Form("DL.evtypebits>>%d&1 && L.tr.n==1 && L.cer.asum_c>%4.1f && (L.prl1.e+L.prl2.e)/(L.gold.p*1000.)>%3.2f && (L.prl2.e)>%3.2f && (rpl.z)>%4.3f && (rpl.z)<%4.3f && (L.tr.tg_ph)>%4.3f && (L.tr.tg_ph)<%4.3f && (L.tr.tg_th)>%4.3f && (L.tr.tg_th)<%4.3f && (L.tr.tg_dp)>%4.3f && (L.tr.tg_dp)<%4.3f && evLeftdnew_r>1e4",Main_Trigger_Left, GC_Cut, EP_Cut, EC2_Cut, TG_VZ_Min, TG_VZ_Max,TG_Phi_Min, TG_Phi_Max, TG_Theta_Min, TG_Theta_Max, TG_Dp_Min, TG_Dp_Max);

	TString cut_R = Form("DR.evtypebits>>%d&1",Main_Trigger_Right);
	TString cut_e_R = Form("DR.evtypebits>>%d&1 && R.tr.n==1 && R.cer.asum_c>%4.1f && (R.ps.e+R.sh.e)/(R.gold.p*1000.)>%3.2f",Main_Trigger_Right,GC_Cut,EP_Cut_R);
	
        TString cut_good_e_R = Form("DR.evtypebits>>%d&1 && R.tr.n==1 && R.cer.asum_c>%4.1f && (R.ps.e+R.sh.e)/(R.gold.p*1000.)>%3.2f && (R.sh.e)>%3.2f && (rpr.z)>%4.3f && (rpr.z)<%4.3f && (R.gold.ph)>%4.3f && (R.gold.ph)<%4.3f && (R.gold.th)>%4.3f && (R.gold.th)<%4.3f && (R.gold.dp)>%4.3f && (R.gold.dp)<%4.3f && evRightdnew_r>1e4",Main_Trigger_Right, GC_Cut, EP_Cut_R, EC2_Cut, TG_VZ_Min, TG_VZ_Max,TG_Phi_Min, TG_Phi_Max, TG_Theta_Min, TG_Theta_Max, TG_Dp_Min, TG_Dp_Max);
        
	double B_up =-1.0; 
	TString beamQual = Form("BeamQual.beamOn>=%4.3f",B_up);

	
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

	cout<<"--- Good Electron Cut: "<< cut_good_e<<endl;
	cut=cut+"&&"+beamQual;
		cut_e=cut_e+"&&"+beamQual;
			cut_good_e=cut_good_e+"&&"+beamQual;
	int Total_Main= T_Tree->GetEntries(TCut(cut));
	int Total_Ele= T_Tree->GetEntries(TCut(cut_e));
	int Good_Ele= T_Tree->GetEntries(TCut(cut_good_e));
	cout<<endl; 
	cout<<Form("--- For %s at %s on HRS-%s:  ", Target.Data(), Kin.Data(), Arm.Data())<<endl;
	cout<<"--- Total Good Electrons from these runs are = "<<Good_Ele<<endl;

	gStyle->SetOptStat(0);
	TCanvas *c1 = new TCanvas("c1","c1",800,600);
	TH1F *h1 = new TH1F("h1","",200, -0.2,0.2);
	h1->SetXTitle("ReactZ (m)");

	T_Tree->Draw((z_var+">>h1").Data(),TCut(cut));
	T_Tree->SetLineColor(3);
	T_Tree->Draw(z_var.Data(),TCut(cut_e),"same");
	T_Tree->SetLineColor(2);
	T_Tree->Draw(z_var.Data(),TCut(cut_good_e),"same");

        Int_t max = h1->GetMaximum();
        h1->GetYaxis()->SetRangeUser(0, max*1.1);

	TLatex *t1 = new TLatex();
	t1->SetNDC();
	t1->SetTextFont(32);
	t1->SetTextSize(0.06);
	t1->SetTextColor(1); 
	t1->DrawLatex(0.15, 0.91, Form("Target = %s at %s (x=%4.3f) on HRS-%s ",Target.Data(), Kin.Data(),xB[kin-1], Arm.Data() ));

	t1->SetTextColor(4); 
	t1->SetTextSize(0.04);
	t1->DrawLatex(0.15, 0.85, Form("Total Good Electrons = %4.3fK (Total %4.1fK T%d Events) ", (Good_Ele/1000.), (Total_Main/1000.), Main_Trigger));

	t1->SetTextColor(6); 
	t1->SetTextSize(0.03);
        int NumberPerLine=10;
        int NumberOfLines=20;
	if(NumberOfRun>120){
		t1->SetTextSize(0.02);
		NumberPerLine=15;
		NumberOfLines=40;
	}
	if(NumberOfRun>200){
		t1->SetTextSize(0.02);
		NumberPerLine=20;
		NumberOfLines=40;
	}
	//t1->DrawLatex(0.20, 0.52, Form("Run-List = %s",Run_String.Data() ));
	TString RUNLIST="Run-List = ";
	for(int j=0;j<std::min(NumberOfRun,NumberPerLine);j++)
		//RUNLIST +=Form("%d ",RunNoChain[j]);
	t1->DrawLatex(0.20, 0.80, Form("%s",RUNLIST.Data() ));
	for(int k=1; k<NumberOfLines;k++){ //20 lines, add more lines if needed, but when there are too many, reduce the font-size and line-distance
		if(NumberOfRun>NumberPerLine*k){
			TString RUNLIST0="                   ";
			for(int j=NumberPerLine*k;j<std::min(NumberOfRun,NumberPerLine*(k+1));j++)
			//	RUNLIST0+=Form("%d ",RunNoChain[j]);
			t1->DrawLatex(0.20, 0.80-0.05*k, Form("%s",RUNLIST0.Data() ));
		}
	}

//	c1->Print(Form("good_ele/%s_%s_%s_%d_N%d_T%d_TripCut.png", Target.Data(),Kin.Data(),Arm.Data(), RunNoChain[0],NumberOfRun,Main_Trigger));
//	c1->Print(Form("good_ele/%s_%s_%s_%d_N%d_T%d_TripCut.pdf", Target.Data(),Kin.Data(),Arm.Data(), RunNoChain[0],NumberOfRun,Main_Trigger));
}
