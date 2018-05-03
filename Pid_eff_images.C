/*This script will calculate the the efficency of the given PID cut:
Need to Supply TString for which arm Left or Right, an int to turn on beam trip cut.
Need to supply: a calo energy minimum and a cher sum adc channel minimum.

Limitations/ issues

*/

vector<Double_t> Pid_eff_3(TChain *T,TString ARM, int flag=0, Int_t cur_sel=0, Double_t calo_e = 0, Double_t cer_min = 0, Double_t beamon_min=1 , int debug=0){
	//TVector Pid_eff_2(TChain *T ,TString ARM,Int_t cur_sel=0, Double_t calo_e = 0, Double_t cer_min = 0, Double_t beamon_min=1 ){
	
	Double_t PID_eff	=1; 	//This will be the over all correction value for this run.
	Double_t PID_e_eff	=1;		//This will be the eff of electron acceptance
	Double_t PID_p_eff	=1;		//This will br the eff of pion rejection.
	Double_t electron_er	=0.0;	//Stastical error in caluclation.
	Double_t pion_er	=0.0;	//Stastical error in caluclation.



//Total number of events to loop through.	
	Int_t Total_entries = T->GetEntries();
	T->ResetBranchAddresses();
	T->SetBranchStatus("*",0);
	T->SetBranchStatus("*.cer.asum_c"	,1);
	T->SetBranchStatus("*.gold.p"		,1);
	T->SetBranchStatus("*.tr.tg*"		,1);
	T->SetBranchStatus("*.evtypebits"	,1);
	T->SetBranchStatus("*.tr.n"			,1);
	T->SetBranchStatus("*BCM.BeamUp_time_v1495"	,1);
	T->SetBranchStatus("rp*.z"			,1);

	if(ARM=="Left"){
		T->SetBranchStatus("*.prl*.e"		,1);
		T->SetBranchStatus("*.prl*.asum_c"	,1);
	}
	else{
		T->SetBranchStatus("R.ps.e"		,1);
		T->SetBranchStatus("R.sh.e" 		,1);
		T->SetBranchStatus("R.sh.asum_c"	,1);
		T->SetBranchStatus("R.ps.asum_c"	,1);
	}
	
	
///Varibles defined for cuts	
	double TG_Theta_Max = 0.04;//40mrad
	double TG_Theta_Min =-0.04;//40mrad
	double TG_Phi_Max = 0.030;//25mrad
	double TG_Phi_Min =-0.030;//25mrad
	double TG_Dp_Max = 0.04;//4%mrad
	double TG_Dp_Min =-0.04;//4%mrad
	double TG_VZ_Max = 0.1200;//4%mrad
	double TG_VZ_Min =-0.1200;//4%mrad
	double P0 = 3.100; //GeV/c
	double GC_Cut = 0.0;//2000.;
		if(cer_min != 0 ){GC_Cut = cer_min;}
	double EP_Cut = 0.0;//0.75;
		if(calo_e != 0 ){EP_Cut = calo_e;}
	int Main_Trigger_Left = 2;
	int Main_Trigger_Right = 5;
	double e_sample_int = 3000.0;
	double e_sample_m = -3000.0/2300.0;
	double p_sample_int = 1500.0;
	double p_sample_m = -1.0;
	double GC_e_sample_min=4000;
	double GC_e_sample_max=6000;
	double GC_p_sample = 1000;
	
	TH2F *h1; 	TH2F *h2;
	TH2F *p1; 	TH2F *p2;
	TH1F *h_1; 	TH1F *h_2;
	TH1F *p_1; 	TH1F *p_2;
	TH1F *hh_1; TH2F *hh1;
	TString histo_string_hh1;	
	TString histo_string_h1;
	TString histo_string_h2;
	TString histo_string_p1;
	TString histo_string_p2;
	TString draw_ops[3] = {"goff","colz",""};
	
/////////////	
cout <<"PID check 1"<<endl;
	TString Arm,arm;
	Int_t Main_Trigger;
	TString cal_det;
	TString string_p_sample, string_e_sample,plot;
	if(ARM=="Left"){
		Arm ="L";arm="l";
		Main_Trigger = Main_Trigger_Left;
		cal_det = "(L.prl1.e+L.prl2.e)/(L.gold.p*1000.)";
		
		if(flag==0){
			string_p_sample = Form("((L.prl2.asum_c-%f)/L.prl1.asum_c) < %f ",p_sample_int,p_sample_m);
			string_e_sample = Form("((L.prl2.asum_c-%f)/L.prl1.asum_c) > %f ",e_sample_int,e_sample_m);  
			plot = "L.prl2.asum_c:L.prl1.asum_c";

			}
	
		if(flag==1){
			string_p_sample = Form("%s.cer.asum_c<%4.1f",Arm.Data(),GC_p_sample);
string_e_sample = Form("%4.1f<%s.cer.asum_c&&%s.cer.asum_c<%4.1f",GC_e_sample_min,Arm.Data(),Arm.Data() ,GC_e_sample_max);
			plot = Form("%s.cer.asum_c",Arm.Data());
			}
		}
	else{
		Arm="R";arm="r";
		Main_Trigger = Main_Trigger_Right;
		cal_det = "(R.ps.e+R.sh.e)/(R.gold.p*1000.)";
		if(flag==0){
			string_p_sample = Form("((R.sh.asum_c-%f)/R.ps.asum_c) < %f ",p_sample_int,p_sample_m);
			string_e_sample = Form("((R.sh.asum_c-%f)/R.ps.asum_c) > %f ",e_sample_int,e_sample_m);
			plot = "R.sh.asum_c:R.ps.asum_c";
			}
		if(flag==1){
			string_p_sample = Form("%s.cer.asum_c<%4.1f",Arm.Data() ,GC_p_sample);
			string_e_sample = Form("%4.1f<%s.cer.asum_c&&%s.cer.asum_c<%4.1f",GC_e_sample_min,Arm.Data(),Arm.Data() ,GC_e_sample_max);
			plot = Form("%s.cer.asum_c",Arm.Data());
			}
		}

		if(flag==0){
			h1 	=new TH2F("h1","h1",100,0,4000,100,0,4000);
			hh1 =new TH2F("hh1","hh1",100,0,4000,100,0,4000);
			h2	=new TH2F("h2","h2",100,0,4000,100,0,4000);
			p1	=new TH2F("p1","p1",100,0,4000,100,0,4000);
			p2 	=new TH2F("p2","p2",100,0,4000,100,0,4000);
			histo_string_hh1 = "hh1";
			histo_string_h1 = "h1";
			histo_string_h2 = "h2";
			histo_string_p1 = "p1";
			histo_string_p2 = "p2";
			h2->SetMarkerColor(2);h2->SetLineColor(2);
			p2->SetMarkerColor(2);p2->SetLineColor(2);
			}
		if(flag==1){
			h_1 	=new TH1F("h_1","h1",5000,0,15000);
			hh_1 	=new TH1F("hh_1","hh1",5000,0,15000);
			h_2		=new TH1F("h_2","h2",5000,0,15000);
			p_1		=new TH1F("p_1","p1",5000,0,15000);
			p_2 	=new TH1F("p_2","p2",5000,0,15000);
			histo_string_hh1 = "hh_1";
			histo_string_h1 = "h_1";
			histo_string_h2 = "h_2";
			histo_string_p1 = "p_1";
			histo_string_p2 = "p_2";
			h_2->SetMarkerColor(2);h_2->SetLineColor(2);
			p_2->SetMarkerColor(2);p_2->SetLineColor(2);
			h_1->SetFillStyle(3009);h_1->SetFillColor(1);
			p_1->SetFillStyle(3009);p_1->SetFillColor(1);
		}

cout <<"PID check 2"<<endl;

/* Cuts for electrons*/
	TString Trigger =Form("D%s.evtypebits>>%d&1",Arm.Data(),Main_Trigger);
	TString one_track =Form("%s.tr.n==1",Arm.Data());
	TString pid_cer = Form("%s.cer.asum_c>%4.1f",Arm.Data(),GC_Cut);
	TString pid_cal = Form("%s>%3.2f",cal_det.Data(),EP_Cut);
	TString target_z = Form("rp%s.z>%4.3f && rp%s.z<%4.3f ",arm.Data(),TG_VZ_Min,arm.Data(),TG_VZ_Max);
	TString target_ph = Form("(%s.tr.tg_ph)>%4.3f && (%s.tr.tg_ph)<%4.3f",Arm.Data(),TG_Phi_Min,Arm.Data(), TG_Phi_Max);
	TString target_th = Form("(%s.tr.tg_th)>%4.3f &&(%s.tr.tg_th)<%4.3f",Arm.Data(),TG_Theta_Min,Arm.Data(),TG_Theta_Max);
	TString track_dp = Form("(%s.tr.tg_dp)>%4.3f && (%s.tr.tg_dp)<%4.3f",Arm.Data(),TG_Dp_Min,Arm.Data(),TG_Dp_Max);
	TString beam_trip = Form("%sBCM.BeamUp_time_v1495[%d] >= %f",ARM.Data(),cur_sel,beamon_min);
	
	//Form("%sBCM.current_dnew >= 2.5*%d",ARM.Data(),cuts_bool);
	
	//		if(Arm =="R"){beam_trip="1";} //Need to replay right arm after bcm class fix

	
	TString pid =pid_cal +"&&"+ pid_cer;
	//if(flag==1){pid = pid_cal;}
	//if(flag==0){pid = pid_cer;}
	
	TString target = target_z+"&&"+target_ph+"&&"+target_th;
	TString track = one_track+"&&"+track_dp;
	TString Gcut = Trigger+"&&"+target+"&&"+track+"&&"+beam_trip;
	TString pion_sample_cut = Gcut+"&&"+string_p_sample;
	TString e_sample_cut = Gcut+"&&"+string_e_sample;
	TString tot_e_cut =  pid+"&&"+e_sample_cut;
	TString tot_p_cut =  pid+"&&"+pion_sample_cut;
			
//Values used in calculation.
//Number of electrons after cut
	Double_t cut_electrons 		= T->GetEntries(TCut(tot_e_cut)); 
//Numer of electrons before cut
	Double_t e_sample 		= T->GetEntries(TCut(e_sample_cut));		
//Number of pions after cut
	Double_t cut_pions		= T->GetEntries(TCut(tot_p_cut));		
//Number of pions before cut
	Double_t p_sample		= T->GetEntries(TCut(pion_sample_cut));		

cout <<"PID check 3"<<endl;
/*
	TCanvas *c1 = new TCanvas("c1","c1");
	T->Draw(Form("%s.cer.asum_c>>dd2(5000,0,15000)",Arm.Data()),TCut(Gcut));
	TCanvas *c2 = new TCanvas("c2","c2");
	T->Draw(Form("%s.cer.asum_c>>dd22(5000,0,15000)",Arm.Data()),TCut(e_sample_cut));
	TCanvas *c3 = new TCanvas("c3","c3");
	T->Draw(Form("%s.cer.asum_c>>ddd22(5000,0,15000)",Arm.Data()),TCut(pid));
*/	
	TCanvas *c;
	
// Drawing to test the script. Can remove the / * and * /  to view histograms 
	if(debug==1){
	 c= new TCanvas("c","c");
	gStyle->SetOptStat(111100);
	c->Divide(1,3);
	c->cd(1);
	T->Draw(Form("%s>>%s",plot.Data(),histo_string_hh1.Data()),Gcut);
	TLine *Le = new TLine(0,e_sample_int,(0-e_sample_int)/e_sample_m  ,0);
	TLine *Lp = new TLine(0,p_sample_int,(0-p_sample_int)/p_sample_m  ,0);
//	Le->SetLineColor(2);Le->Draw("same");
//	Lp->SetLineColor(2);Lp->Draw("same");

	c->cd(2);
	T->Draw(Form("%s>>%s",plot.Data(),histo_string_h1.Data()),TCut(e_sample_cut));
	T->Draw(Form("%s>>%s",plot.Data(),histo_string_h2.Data()),TCut(pion_sample_cut),"same");

	c->cd(3);
	T->Draw(Form("%s>>%s",plot.Data(),histo_string_p1.Data()),TCut(tot_e_cut));
	T->Draw(Form("%s>>%s",plot.Data(),histo_string_p2.Data()),TCut(tot_p_cut),"same");
	}


	PID_e_eff = cut_electrons/e_sample;
	PID_p_eff = 1.0 - (cut_pions/p_sample);
	PID_eff = (1.0/PID_e_eff)*PID_p_eff;
	
	//electron_er = sqrt( pow((1/sqrt(cut_electrons*1.0)*1.0),2) + pow((1/sqrt(e_sample*1.0)*1.0),2) );
	//pion_er = sqrt( pow((1/sqrt(cut_pions*1.0)*1.0),2) + pow((1/sqrt(p_sample*1.0)*1.0),2) );
	electron_er = 1/sqrt(e_sample*1.0);
	pion_er 	= 1/sqrt(p_sample*1.0);
	if(debug==1){
		cout << "PID" <<endl;
		cout << "E " << cut_electrons << ": "<<e_sample << " | P " << cut_pions << " : " <<p_sample <<endl;
		cout << "E " << PID_e_eff <<"\t P " << PID_p_eff <<endl; 
		//cout << "E " << electron_er <<"\t P " << pion_er <<endl; 
	}
	
	vector<Double_t> PID_effs;
	PID_effs.push_back(PID_e_eff);
	PID_effs.push_back(electron_er);
	
	PID_effs.push_back(PID_p_eff);
	PID_effs.push_back(pion_er);
	//TVector PID_effs[2];
	//PID_effs[0]=PID_e_eff;
	//PID_effs[1]=PID_p_eff;
	
	

	if(debug==0){
		delete c;
		if(flag==0){
			delete h1; 		delete h2;
			delete p1; 		delete p2;
			delete hh1;
		}
		else{
			delete h_1; 	delete h_2;
			delete p_1; 	delete p_2;
			delete hh_1; 	
		}
	}
	return PID_effs;
}
	



