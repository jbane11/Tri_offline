#include "/home/jbane/HEADERS/inc1.h"
#include "/home/jbane/HEADERS/TRI_Main.h"
#include "/home/jbane/HEADERS/TRI_Tools.h"


///////////////////////////////////////////////////////////////////////////
// This Code will calulate the OneTrack efficency by finding the number of/
// events that have one Track vs the number of events that have a track !//
// It will** ptr, TClass* ptrClass, EDataType datatype, Bool_t isptrreturn a vector of double with the efficency and any errors 	///
// that have been added (mainly 1/sqrt(n))!				///
//////////

vector<Double_t> Tracking_eff(TChain *T, TString Arm = "L",  int debug=0, int plot_flag=0)
{
	vector<Double_t> Tracking_eff;
	TString ARM="";
	if(Arm=="L")ARM="Left";
	else ARM="Right";

	Int_t Total_entries = T->GetEntries();
	cout << Total_entries <<endl;
	//Debug: statment to check correct tree:
	if(debug==1){cout << "DEBUG: Number of entries: "<<Total_entries <<endl;}

	//way to check which Arm, if the tree is already open:
	TBranch *ArmCheck = T->FindBranch(Form("%s.tr.n",Arm.Data()));
	if(ArmCheck==nullptr){
		if(Arm=="L"){Arm="R"; ARM="Right";}	
		if(Arm=="R"){Arm="L";  ARM="Left";}
		if(debug==1)cout<<"DEBUG: The correct Arm is : "<< Arm<<endl;
		}
	////////////////////////////////////////////////////////////////////

	TString kin_tgt = "";
	kin_tgt=GetTarget(T)+"_kin"+Form("%d",GetKin(T));
	cout << kin_tgt <<endl;
	
	//Set the limits of a good electron cut string. inc1.h
	set_limits(kin_tgt);
	//Set Ep cut include some pions
	//EP_Cut_inc = 0.1;
	// Write out cut string. only use non track PIDs
	TString GoodE_Cuts = Good_Electron_Cuts(ARM,0,1,0,0,1);
	//Make sure this event should have a track!
	//Look at an event that will fire the shower, s2, and s1;
	double s0_min=200;
	double s2_min=200;
	double cal_min=800;

	TString S0_Cut = Form("(%s.s0.la_c+%s.s0.ra_c)>=%f",Arm.Data(),Arm.Data(),s0_min);
	TString cal_string="";
	if(Arm=="L")
		cal_string=Form("L.prl1.asum_c>%f&&L.prl2.asum_c>%f",cal_min,cal_min);
	else{	
		cal_string=Form("R.ps.asum_c>%f&&R.sh.asum_c>%f",cal_min,cal_min);
	}
	
	TString cal_Cut= cal_string;	
	GoodE_Cuts+="&&"+cal_string;	

	//For s2, need to loop over all channels;
	TString S2_Cut ="(";
	for(int i=0;i<16;i++){
		S2_Cut+=Form("(%s.s2.la_c[%d]+%s.s2.ra_c[%d])>=%f||",Arm.Data(),i,Arm.Data(),i,s2_min);				}
	S2_Cut.Remove(S2_Cut.Length()-2,2);
	S2_Cut+=")";
	////////
	GoodE_Cuts+="&&"+S2_Cut+"&&"+S0_Cut;

	TSubString sub = GoodE_Cuts.operator()(0,2);
	if(sub=="&&"||sub=="||")GoodE_Cuts.Remove(0,2);	


        if(plot_flag){
                T->Draw(Form("%s.tr.n>>H(10,-1,9)",Arm.Data()),TCut(GoodE_Cuts));
                }


	int num_Es_total= T->GetEntries(TCut(GoodE_Cuts));
	//debugs
	if(debug){	
		cout <<"Cut GE:"<< GoodE_Cuts <<endl;
		cout <<"Total :"<< num_Es_total <<endl;
	}
	//Add in the oneT cut
	TString Tracking_Cut = Form("%s.tr.n>0",Arm.Data());
	TString T_cut = GoodE_Cuts+"&&"+Tracking_Cut;
	int num_Es_1T = T->GetEntries(TCut(T_cut));
	//debugs
	if(debug){
		cout <<"Cut Tr :"<< T_cut <<endl;
		cout <<"cut num:"<<num_Es_1T <<endl;
	}
	// calculate the eff and error: 
	Double_t T_eff = num_Es_1T/(num_Es_total*1.0);
	Double_t T_cut_error = Bino_Err(T_eff,num_Es_total);
	//debugs
	if(debug){
		cout << "Eff   :" << T_eff <<endl;
		cout << "Delta :" << T_cut_error <<endl;
	}
	Tracking_eff.push_back(T_eff);
	Tracking_eff.push_back(T_cut_error);

	return Tracking_eff;
}//end of program!!
