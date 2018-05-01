#include "/home/jbane/HEADERS/inc1.h"
#include "/home/jbane/HEADERS/TRI_Main.h"
#include "/home/jbane/HEADERS/TRI_Tools.h"


///////////////////////////////////////////////////////////////////////////
// This Code will calulate the OneTrack efficency by finding the number of/
// events that have one Track vs the number of events that have a track !//
// It will** ptr, TClass* ptrClass, EDataType datatype, Bool_t isptrreturn a vector of double with the efficency and any errors 	///
// that have been added (mainly 1/sqrt(n))!				///
//////////

vector<Double_t> OneTrack_eff(TChain *T, TString Arm = "L",  int debug=0, int plot_flag=0)
{
	vector<Double_t> OneTrack_eff;
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
	// Write out cut string. only use non track PIDs
	TString GoodE_Cuts = Good_Electron_Cuts(ARM,2,0,0,0,1);
	int num_Es_total= T->GetEntries(TCut(GoodE_Cuts));
	//debugs
	if(debug){	
		cout <<"Cut   :"<< GoodE_Cuts <<endl;
		cout <<"One  T:"<< num_Es_total <<endl;
	}
	//Add in the oneT cut
	TString oneT = Form("&&%s.tr.n==1",Arm.Data());
	TString oneT_cut = GoodE_Cuts+oneT;
	int num_Es_1T = T->GetEntries(TCut(oneT_cut));
	//debugs
	if(debug){
		cout <<"Cut    :"<< oneT_cut <<endl;
		cout <<"Total  :"<<num_Es_1T <<endl;
	}
	// calculate the eff and error: 
	Double_t OneT_eff = num_Es_1T/(num_Es_total*1.0);
	Double_t OneT_cut_error = Bino_Err(OneT_eff,num_Es_total);
	//debugs
	if(debug){
		cout << "Eff   :" <<OneT_eff <<endl;
		cout << "Delta :" << OneT_cut_error <<endl;
	}
	OneTrack_eff.push_back(OneT_eff);
	OneTrack_eff.push_back(OneT_cut_error);

	return OneTrack_eff;
}//end of program!!
