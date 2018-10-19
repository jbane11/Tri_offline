#include "../headers/inc1.h"
#include "../headers/rootalias1.h"
#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"


void yield(std::string kin_name="", int debug=1)
{
	string kin_tgt=kin_name;
	//////////////////////////////////////////////////////////////////////////////
	string kin_dir = "/work/halla/triton/Runlist/";
	//Input the kin file incase was not given in argument
	if(kin_name==""){
		cout << "Please enter the kinematic file, that you want to use!"<<"\n";
		cout << "example (\"He3_kin7\")"<<"\n";
		cin >> kin_name;
	} 
	kin_name = kin_dir+kin_name+".dat";
	///Open the kin file using the function kin_file
	kin_file KFile;
	KFile.set_file(kin_name,debug);
	//This vector contains all of the runs for this target at this kin.
	vector<int> run_vector = KFile.run_vect; 
	//////////////////////////////////////////////////////////////////////////////

	//Read in the corrections table, function in inc1.h
	corrections C_T;
	C_T.Read_Table(KFile.target,KFile.kin_num,1);
	//Set cut limits from kin_cut.txt
	set_limits(kin_tgt);
	
	/////Check if all the runs are in the table////////
	vector< vector<int>> run_in_Cor;vector <int> vector_tmp;
	for(unsigned int j =0;j<run_vector.size();j++)
	{
		vector_tmp.push_back(run_vector[j]);
		auto p  =find(C_T.ORs.begin(),C_T.ORs.end(),run_vector[j]);
		if(p != C_T.ORs.end()) vector_tmp.push_back(1);
		else vector_tmp.push_back(-1);
		run_in_Cor.push_back(vector_tmp);
		if(debug) cout<< vector_tmp[0]<<" "<< vector_tmp[1]<<" :";
		vector_tmp.clear();
	}if(debug)cout<<"\n";
	/////////////////////////////////////////////////	
	
	///Stuff needed for each run; Like vectors for the quanits
	//of x, sigma.....
	vector< vector<double>> kin_varibles; 	
	// the layout of kin_varibles -run , x_bj, sigma, yield, sigma.... 
	//Histogram arrays
	int num_of_runs = run_vector.size();
	TChain *T[num_of_runs];
	TH1F *theta_hist[num_of_runs];
	TH1F *x_bj_hist[num_of_runs];
	TH1F *eprime_hist[num_of_runs];
	TH1F *dp_hist[num_of_runs];
	TH1F *e_beam_hist[num_of_runs];
	TH1F *Qsq_hist[num_of_runs];

	TFile *output_file= new TFile(Form("%s.root",kin_tgt.c_str()),"RECREATE");

	//////Start looping through the runs///////////
	for(unsigned int i=0; i<run_vector.size();i++)
	{
		int run=run_vector[i];
		if(debug)cout<<"Debug::::Start of Run " << run <<"::::\n";
		T[i] = LoadRun(run);
		

		//Need to determine central theta, e, and e' for historgams,
		double central_E_beam = GetBeamE(T[i])/1000.0;	//GeV
		double E_beam_reso = 0.0001; 		//GeV
		double E_low_bin = central_E_beam*0.95;
		double E_high_bin = central_E_beam*1.05;
		int num_of_E_bins = ceil((E_high_bin - E_low_bin)/E_beam_reso);
		e_beam_hist[i] = new TH1F(Form("e_beam_%d",run),
			Form("E beam for run %d",run), num_of_E_bins,
			E_low_bin, E_high_bin);
if(debug)cout << "E: "<<E_low_bin<<" "<<E_high_bin<<" "<<num_of_E_bins<<"\n";

		///
		double central_theta = GetSetTheta(T[i]);
		double theta_reso    = milRtoDeg(0.5);   //0.5 mrad from nim paper 
		double theta_low_bin = central_theta - milRtoDeg(30);
		double theta_high_bin= central_theta + milRtoDeg(30);
		int num_of_theta_bins= ceil((theta_high_bin-theta_low_bin)/ theta_reso);
		theta_hist[i] = new TH1F(Form("theta_%d",run),
			Form("Scattered angle for run %d",run), num_of_theta_bins,
			theta_low_bin, theta_high_bin);
if(debug)cout << "theta: "<<theta_low_bin<<" "<<theta_high_bin<<" "<<num_of_theta_bins<<"\n";
		///
		double central_Mo   = GetSetMo(T[i]);
		double Mo_reso	    = 0.0001; 		//Nim
		double Mo_low_bin   =central_Mo*0.955;
		double Mo_high_bin  =central_Mo*1.045;
		int num_of_Mo_bins  =ceil((Mo_high_bin-Mo_low_bin)/Mo_reso);
		eprime_hist[i] = new TH1F(Form("eprime_%d",run),
			Form("Scattered Mo for run %d",run), num_of_Mo_bins,
			Mo_low_bin, Mo_high_bin);
if(debug)cout << "Mo: "<<Mo_low_bin<<" "<<Mo_high_bin<<" "<<num_of_Mo_bins<<"\n";
		///
		double dp_reso	    = 0.0001; 		//Nim
		double dp_low_bin   =-0.05;
		double dp_high_bin  = 0.05;
		int num_of_dp_bins  =ceil((dp_high_bin-dp_low_bin)/dp_reso);
		dp_hist[i] = new TH1F(Form("dp_%d",run),
			Form("Scattered dp/p for run %d",run), num_of_dp_bins,
			dp_low_bin, dp_high_bin);
if(debug)cout << "dp: "<<dp_low_bin<<" "<<dp_high_bin<<" "<<num_of_dp_bins<<"\n";
		///
		double Qsq_low_bin  = 4.0*E_low_bin *Mo_low_bin *pow(sin((theta_low_bin *PI/180)/2),2);
		double Qsq_high_bin = 4.0*E_high_bin*Mo_high_bin*pow(sin((theta_high_bin*PI/180)/2),2);
		double Qsq_reso     = sqrt(pow(E_beam_reso,2)+pow(theta_reso,2)+pow(theta_reso,2) +						pow(Mo_reso,2));
		int num_of_Qsq_bins = ceil((Qsq_high_bin-Qsq_low_bin)/Qsq_reso); 
		Qsq_hist[i] = new TH1F(Form("Qsq_%d",run),
			Form("4-mo transfer for run %d",run), num_of_Qsq_bins,
			Qsq_low_bin, Qsq_high_bin);
if(debug)cout << "Qqs: "<<Qsq_low_bin<<" "<<Qsq_high_bin<<" "<<num_of_Qsq_bins<<"\n";
		///
		double x_bj_low_bin = Qsq_low_bin /(2*0.938*(E_high_bin-Mo_low_bin ));	
		double x_bj_high_bin = Qsq_high_bin/(2*0.938*(E_low_bin -Mo_high_bin));	
		double x_bj_reso= sqrt(pow(Qsq_reso,2) + pow(2*Mo_reso,2));
		int num_of_x_bj_bins = ceil((x_bj_high_bin -x_bj_low_bin)/x_bj_reso);	
		x_bj_hist[i] = new TH1F(Form("X_bj_%d",run),
			Form("xbj for run %d",run), num_of_x_bj_bins,
			x_bj_low_bin, x_bj_high_bin);
if(debug)cout << "X: "<<x_bj_low_bin<<" "<<x_bj_high_bin<<" "<<num_of_x_bj_bins<<"\n";
		
		///Which arm!/////
		string Arm = "L"; string ARM="Left";
		if(T[i]->FindBranch("L.tr.tg_dp") == nullptr ){ Arm ="R"; ARM ="Right";}

		///////////Loop through each run event by event!!
		unsigned int entries= T[i]->GetEntries();
		double ent_10 = entries/10.0;
		int ent_10_int= ceil(ent_10);
		int ent_10_cnt=1;
		double E_beam_ev=0; double E_prime_ev=0; 
		double dp_ev=0; double theta_ev=0;
		double Qsq_ev=0; double x_bj_ev=0; double nu=0;
		T[i]->ResetBranchAddresses();
		T[i]->SetBranchAddress(Form("EK%sx.angle",Arm.c_str())	,&theta_ev);
		T[i]->SetBranchAddress(Form("%s.tr.tg_dp",Arm.c_str())	,&dp_ev);
		T[i]->SetBranchAddress("HALLA_p"	   	,&E_beam_ev);
		T[i]->SetBranchAddress(Form("ex%s.p",Arm.c_str()) 	,&E_prime_ev);
		double weight =1.0;

		double charge = C_T.CT[i][1];
		double DT     = C_T.CT[i][3];
		double PID_eff =C_T.CT[i][5];

		double LT = (100-DT)/100.0;

		weight = 1/(charge*LT)* PID_eff;

		if(debug)cout<<"Looking at " <<entries <<" events\n";	
		for(unsigned int entry=0; entry<entries;entry++)
		{
			T[i]->GetEntry(entry);
			if(GOOD_Event(T[i], entry,0,ARM)){
			///Good electrons with in acc cuts.....
			cout <<"good event" <<endl;
				Qsq_ev = 4*E_beam_ev/1000.0*E_prime_ev*pow(sin(theta_ev/2.0),2);
				x_bj_ev= Qsq_ev /( 2* 0.938 * (E_beam_ev/1000.0 - E_prime_ev));
			cout<<E_beam_ev<<" "<< E_prime_ev<<" " << Qsq_ev<<" "<< x_bj_ev <<" " << theta_ev*180/PI <<" "<< E_prime_ev<<" "<<dp_ev <<endl;
				x_bj_hist[i]->Fill(x_bj_ev,weight);
				Qsq_hist[i]->Fill(Qsq_ev,weight);
				dp_hist[i]->Fill(dp_ev,weight);
				theta_hist[i]->Fill(theta_ev*180/PI,weight);
			}//end of good event	
			//reset
			Qsq_ev=0; x_bj_ev=0; theta_ev=0;
			dp_ev=0 ; E_beam_ev=0; E_prime_ev=0;
			if(debug)
			{ 
				if(entry/ent_10_cnt == entry/(ent_10_cnt*1.0))
//(entry/ent_10_cnt) == (entry/(ent_10_cnt*1.0)))
				{
					if(ent_10_cnt==1)ent_10_cnt=0;
					ent_10_cnt += ent_10;
					cout << "Finished " << entry <<" "<<(entry/(entries*1.0)) << " %\n";
				}
			}


		}//End of entries loop!!!!
		TCanvas *C = new TCanvas(Form("C%d",i),"canvas");
		C->Divide(2,2);
		C->cd(1);
		x_bj_hist[i]->Draw();
		C->cd(2);
		Qsq_hist[i] ->Draw();
		C->cd(3);
		dp_hist[i]->Draw();
		C->cd(4);
		theta_hist[i]->Draw();
		break;

		//T[num_of_runs]= nullptr;
	}
	///////End of run loop//////////////////////////


}
