/*This script will calculate the the efficency of the the main production trigger:
Need to Supply TString for which arm Left or Right, an int to turn on beam trip cut.

This will use count the number of main E triggers and the number of effiecncy triggers that should of fired the main eff. triggers.

Limitations/ issues

*/


Double_t trigger_eff(TString ARM,Int_t cuts_bool=0){
	
	Double_t Trig_eff=0; //This will be the tigger eff for the main trigger.

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
	double EP_Cut = 0.55;
	int Main_Trigger_Left = 2;
	int Eff_Trigger_Left = 1;
	int Main_Trigger_Right = 5;
	int Eff_Trigger_Right = 4;
	double e_sample_int = 3000.0;
	double e_sample_m = -1.0;
	double p_sample_int = 1500.0;
	double p_sample_m = -1.0;
/////////////	

	TString Arm,arm;
	Int_t Main_Trigger,Eff_trigger;
	TString cal_det;
	TString string_p_sample, string_e_sample;
	int nbins,binlow,binmax;
	if(ARM=="Left"){
		Arm ="L";arm="l";
		Main_Trigger = Main_Trigger_Left; Eff_trigger= Eff_Trigger_Left;
		cal_det = "(L.prl1.e+L.prl2.e)/(L.gold.p*1000.)";
		string_p_sample = Form("((L.prl2.asum_c-%f)/L.prl1.asum_c) < %f ",p_sample_int,p_sample_m);
		string_e_sample = Form("((L.prl2.asum_c-%f)/L.prl1.asum_c) > %f ",e_sample_int,e_sample_m);  
		nbins =10; binlow=0; binmax=10;
		}
	else{
		Arm="R";arm="r";
		Main_Trigger = Main_Trigger_Right;  Eff_trigger= Eff_Trigger_Right;
		cal_det = "(R.ps.e+R.sh.e)/(R.gold.p*1000.)";
		string_p_sample = Form("((R.sh.asum_c-%f)/R.ps.asum_c) < %f ",p_sample_int,p_sample_m);
		string_e_sample = Form("((R.sh.asum_c-%f)/R.ps.asum_c) > %f ",e_sample_int,e_sample_m);
		nbins =10; binlow=0; binmax=10;
		}


/* Cuts for electrons*/
	TString Trigger =Form("(D%s.evtypebits>>%d)&1",Arm.Data(),Main_Trigger);
	TString  EffTrigger= Form("(D%s.evtypebits>>%d)&1",Arm.Data(),Eff_trigger);
	TString one_track =Form("%s.tr.n==1",Arm.Data());
	TString pid_cer = Form("%s.cer.asum_c>%4.1f",Arm.Data(),GC_Cut);
	TString pid_cal = Form("%s>%3.2f",cal_det.Data(),EP_Cut);
	TString target_z = Form("rp%s.z>%4.3f && rp%s.z<%4.3f ",arm.Data(),TG_VZ_Min,arm.Data(),TG_VZ_Max);
	TString target_ph = Form("(%s.tr.tg_ph)>%4.3f && (%s.tr.tg_ph)<%4.3f",Arm.Data(),TG_Phi_Min,Arm.Data(), TG_Phi_Max);
	TString target_th = Form("(%s.tr.tg_th)>%4.3f &&(%s.tr.tg_th)<%4.3f",Arm.Data(),TG_Theta_Min,Arm.Data(),TG_Theta_Max);
	TString track_dp = Form("(%s.tr.tg_dp)>%4.3f && (%s.tr.tg_dp)<%4.3f",Arm.Data(),TG_Dp_Min,Arm.Data(),TG_Dp_Max);
	TString beam_trip = Form("%sBCM.current_dnew >= 2.5*%d",ARM.Data(),cuts_bool);
		if(Arm =="R"){beam_trip="1";} //Need to replay right arm after bcm class fix
/* Combinding those cuts together*/	
	TString pid = pid_cer+"&&"+pid_cal; 
	TString target = target_z+"&&"+target_ph+"&&"+target_th;
	TString track = one_track+"&&"+track_dp;
	TString Gcut = target+"&&"+track+"&&"+beam_trip;
	TString Ecut =  pid+"&&"+Gcut+"&&"+Trigger;
	TString Sec_cut= pid+"&&"+Gcut+"&&"+EffTrigger;
	
	TCanvas *c_Tri_eff = new TCanvas("Tri_eff","Tri_eff");
	TH1F *tri1 = new TH1F("tri1","Main Trigger",nbins,binlow,binmax);
	TH1F *tri2 = new TH1F("tri2","Main Trigger",nbins,binlow,binmax);
	T->Draw(Form("log(D%s.evtypebits)/log(2)>>tri1",Arm.Data()),TCut(Ecut));
	T->Draw(Form("log(D%s.evtypebits)/log(2)>>tri2",Arm.Data()),TCut(Sec_cut),"same");
	tri1->SetLineWidth(3); tri1->SetLineColor(2);tri2->SetLineWidth(1);
	tri2->SetFillColor(4); tri2->SetFillStyle(3003);
	gPad->SetLogy();
	//Number of electrons seen with the main trigger.
	Double_t G_eles =T->GetEntries(TCut(Ecut));
	//Number of electrons seen with the secondary trigger(eff trigger)
	Double_t eff_eles =T->GetEntries(TCut(Sec_cut));
	
	Trig_eff = G_eles/eff_eles ;
	
	cout << "Trigger eff: "<<endl;
	cout << "main : " << G_eles << " eff : " << eff_eles << "  eff : " << Trig_eff <<endl;
	
	return Trig_eff;
} 

