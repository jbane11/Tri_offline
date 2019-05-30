#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"

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

 	auto RI = GetRunData(T);
	int runnum =RI->GetNumber();

	Int_t Total_entries = T->GetEntries();
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
	TString tgt;
	auto TI  = GetTarget(runnum);
	tgt=TI.name;
	TString kin_tgt = "";
	kin_tgt=tgt+"_kin"+GetKin(runnum);
	cout << kin_tgt <<endl;

	//Set the limits of a good electron cut string. inc1.h
	set_limits(kin_tgt);
	//Set Ep cut include some pions
	//EP_Cut_inc = 0.1;
	// Write out cut string. only use non track PIDs
	TCut GoodE_Cuts = L_mara_trig + layers_electron_cut_L;
	//Make sure this event should have a track!
	//Look at an event that will fire the shower, s2, and s1;
	double s0_min=200;
	double s2_min=200;
	double cal_min=800;

	TString S0_Cut = Form("&&(%s.s0.la_c+%s.s0.ra_c)>=%f",Arm.Data(),Arm.Data(),s0_min);
	TString cal_string="";
	//if(Arm=="L")
		//cal_string=Form("L.prl1.asum_c>%f&&L.prl2.asum_c>%f",cal_min,cal_min);
	//else{
		//cal_string=Form("R.ps.asum_c>%f&&R.sh.asum_c>%f",cal_min,cal_min);
	//}

//	TString cal_Cut= cal_string;
	//GoodE_Cuts+="&&"+cal_string;

	//For s2, need to loop over all channels;
	TString S2_Cut ="(";
	for(int i=0;i<16;i++){
		S2_Cut+=Form("(%s.s2.la_c[%d]+%s.s2.ra_c[%d])>=%f||",Arm.Data(),i,Arm.Data(),i,s2_min);				}
	S2_Cut.Remove(S2_Cut.Length()-2,2);
	S2_Cut+=")";
	////////
	GoodE_Cuts+=S2_Cut+S0_Cut;

	//TSubString sub = GoodE_Cuts.operator()(0,2);
	//if(sub=="&&"||sub=="||")GoodE_Cuts.Remove(0,2);


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
	TCut Tracking_Cut = Form("%s.tr.n==1",Arm.Data());
	TCut T_cut = GoodE_Cuts+Tracking_Cut;
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


void Tracking_eff_csv( string tgt="H3")
{
	ofstream output;
	output.open(Form("tracking_allkin_%s.csv",tgt.c_str()));
	output<<"Tracking_eff"<<","<<"Tracking_err\n";

	for(unsigned int i=0; i<16;i++)
	{
		string kin=Form("%d",i);
		vector<int> runlist = SQL_Kin_Target(kin,tgt);
		if(runlist.size()==0)continue;
		TChain *T = new TChain("T");
		for(unsigned int k =0; k<runlist.size();k++)
		{

			T->Add(LoadRun(runlist[k]));
			int total_ent = T->GetEntries();

			if(total_ent>=1000000)break;
			if(k>=20){break;}
		}

		vector<double> t_eff = Tracking_eff(T,"L");
		output <<i<<",";
		for(unsigned int j=0;j<t_eff.size();j++)
		{	output<<t_eff[j];	if(j!=t_eff.size()-1)output<<",";	}
		output<<"\n";
		t_eff.clear();
	}

	output.close();

}


 vector<double> read_track_eff(string kin="",string tgt="H3")
 {
	 vector<double> trck_eff={1.0,0.0};
	 string line;

   ifstream input; input.open(Form("./tracking_allkin_%s.csv",tgt.c_str()));

	 if(!input.is_open()){cout << "File issues " <<endl;return trck_eff;}
   vector <double> invec;
   getline(input,line);

   while(input.good())
   {
     getline(input,line);
     invec = parse_csv_d(line,',');
     if( invec[0] == stod(kin)) break;
     invec.clear();
   }
	 trck_eff[0]=invec[1];
	 trck_eff[1]=invec[2];

	 return trck_eff;
 }

 void tracking_sql(int runnum, int debug=0)
 {
	 if(debug) cout << "Run " << runnum <<endl;
	 if(!SQLCheck(runnum)) exit(1);
   CODASetting  coda = GetCODASetting(runnum);
   AnalysisInfo Ainfo= GetAnalysisInfo(runnum);
   if(Ainfo.status == -1){cout <<"Not in analysis DB \n"; exit(1);}

   TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(), mysql_user.Data(), mysql_password.Data());

   TString query_check = Form("select Tracking_eff, Tracking_err from %sanalysis where run_number = %d;", coda.experiment.Data(), runnum);

   TSQLResult* result  = Server->Query(query_check.Data());



	 double trck_eff=1.0; double trck_err=0.0;
	 if(result->GetFieldCount()!=0)
   {
     TSQLRow *row = result->Next();
		 if(row->GetField(0)!=nullptr) trck_eff=atof(row->GetField(0));
		 else trck_eff=1.0;
		 if(row->GetField(1)!=nullptr) trck_err=atof(row->GetField(1));
		 else trck_err =0.0;
	 }

	 string kin = Form("%s",Ainfo.kin.Data());
   string target = Form("%s",Ainfo.target.Data());
   string tgt = shortTgtName(target);

	 vector<double> trck= read_track_eff(kin,tgt);



	 if(trck[0]!=trck_eff)
	 {
		 TString update = Form("UPDATE %sanalysis SET Tracking_eff = %f, Tracking_err= %f where run_number = %d;", coda.experiment.Data(),trck[0],trck[1], runnum);
     Server->Query(update.Data());
	 }
	 Server->Close();
 }
