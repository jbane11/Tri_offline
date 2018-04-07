/*This script will calculate the the efficency of the given PID cut:
Need to Supply TString for which arm Left or Right, an int to turn on beam trip cut.
Need to supply: a calo energy minimum and a cher sum adc channel minimum.

Limitations/ issues

*/

double Pid_eff_1(TChain *T,TString ARM="L",Int_t cur_sel=0, Double_t calo_e = 0, Double_t cer_min = 0, Double_t beamon_min=1 ){
	
	Double_t PID_eff=1; 	//This will be the over all correction value for this run.
	Double_t PID_e_eff=1;	//This will be the eff of electron acceptance
	Double_t PID_p_eff=1;	//This will br the eff of pion rejection.



//Total number of events to loop through.	
	Int_t Total_entries = T->GetEntries();
	T->ResetBranchAddresses();
	
///Varibles defined for cuts	
	double TG_Theta_Max = 0.04;//40mrad
	double TG_Theta_Min =-0.04;//40mrad
	double TG_Phi_Max = 0.030;//25mrad
	double TG_Phi_Min =-0.030;//25mrad
	double TG_Dp_Max = 0.04;//4%mrad
	double TG_Dp_Min =-0.04;//4%mrad
	double TG_VZ_Max = 0.100;//4%mrad
	double TG_VZ_Min =-0.100;//4%mrad
	double P0 = 3.100; //GeV/c
	double GC_Cut = 500.;
		if(cer_min != 0 ){GC_Cut = cer_min;}
	double EP_Cut = 0.55;
		if(calo_e != 0 ){EP_Cut = calo_e;}
	int Main_Trigger_Left = 2;
	int Main_Trigger_Right = 5;
	double e_sample_int = 3000.0;
	double e_sample_m = -1.0;
	double p_sample_int = 1500.0;
	double p_sample_m = -1.0;
/////////////	

	TString Arm,arm;
	Int_t Main_Trigger;
	TString cal_det;
	TString string_p_sample, string_e_sample,plot;
	if(ARM=="Left"){
		Arm ="L";arm="l";
		Main_Trigger = Main_Trigger_Left;
		cal_det = "(L.prl1.e+L.prl2.e)/(L.gold.p*1000.)";
		string_p_sample = Form("((L.prl2.asum_c-%f)/L.prl1.asum_c) < %f ",p_sample_int,p_sample_m);
		string_e_sample = Form("((L.prl2.asum_c-%f)/L.prl1.asum_c) > %f ",e_sample_int,e_sample_m);  
		plot = "L.prl2.asum_c:L.prl1.asum_c";
		}
	else{
		Arm="R";arm="r";
		Main_Trigger = Main_Trigger_Right;
		cal_det = "(R.ps.e+R.sh.e)/(R.gold.p*1000.)";
		string_p_sample = Form("((R.sh.asum_c-%f)/R.ps.asum_c) < %f ",p_sample_int,p_sample_m);
		string_e_sample = Form("((R.sh.asum_c-%f)/R.ps.asum_c) > %f ",e_sample_int,e_sample_m);
				plot = "R.sh.asum_c:R.ps.asum_c";
		}


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

	
	TString pid = pid_cer+"&&"+pid_cal;
	TString target = target_z+"&&"+target_ph+"&&"+target_th;
	TString track = one_track+"&&"+track_dp;
	TString Gcut = Trigger+"&&"+target+"&&"+track+"&&"+beam_trip;
	TString pion_sample_cut = Gcut+"&&"+string_p_sample;
	TString e_sample_cut = Gcut+"&&"+string_e_sample;
	TString tot_e_cut =  pid+"&&"+e_sample_cut;
	TString tot_p_cut =  pid+"&&"+pion_sample_cut;
			
//Values used in calculation.
//Number of electrons after cut
	Double_t cut_electrons 	= T->GetEntries(TCut(tot_e_cut)); 
//Numer of electrons before cut
	Double_t e_sample 		= T->GetEntries(TCut(e_sample_cut));		
//Number of pions after cut
	Double_t cut_pions		= T->GetEntries(TCut(tot_p_cut));		
//Number of pions before cut
	Double_t p_sample		= T->GetEntries(TCut(pion_sample_cut));		

// Drawing to test the script. Can remove the / * and * /  to view histograms 
	TCanvas *c = new TCanvas("c","c");
	gStyle->SetOptStat(111100);
	c->Divide(1,3);
	c->cd(1);
	T->Draw(Form("%s>>hh1(100,0,4000,100,0,4000)",plot.Data()),Gcut);
	TLine *Le = new TLine(0,e_sample_int,(0-e_sample_int)/e_sample_m  ,0);
	TLine *Lp = new TLine(0,p_sample_int,(0-p_sample_int)/p_sample_m  ,0);
	Le->SetLineColor(2);Le->Draw("same");
	Lp->SetLineColor(2);Lp->Draw("same");
	c->cd(2);
	TH2F *h2 =new TH2F("h2","h2",100,0,4000,100,0,4000);
	TH2F *p2 =new TH2F("p2","p2",100,0,4000,100,0,4000);
	T->Draw(Form("%s>>h1(100,0,4000,100,0,4000)",plot.Data()),TCut(Gcut));
	T->Draw(Form("%s>>h2",plot.Data()),TCut(tot_e_cut),"same");
	h2->SetMarkerColor(2);h2->SetLineColor(2);
	c->cd(3);
	T->Draw(Form("%s>>p1(100,0,4000,100,0,4000)",plot.Data()),TCut(Gcut));
	T->Draw(Form("%s>>p2",plot.Data()),TCut(tot_p_cut),"goff");
	p2->SetMarkerColor(2);p2->SetLineColor(2);

	PID_e_eff = cut_electrons/e_sample;
	PID_p_eff = 1.0 - (cut_pions/p_sample);
	PID_eff = (1.0/PID_e_eff)*PID_p_eff;
	
	cout << "PID" <<endl;
	cout << "E " << cut_electrons << ": "<<e_sample << " | P " << cut_pions << " : " <<p_sample <<endl;
	cout << "E " << PID_e_eff <<"\t P " << PID_p_eff <<endl; 

	printf(":%10.10f\t\n",PID_eff);

//	delete(c);	
	return PID_eff;
}
	



