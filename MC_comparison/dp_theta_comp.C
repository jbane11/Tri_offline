#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"
////////////////////////////////////
// Sript for comparing acceptance varibles between MC and data
// Run with options -r list of runs, to look at a list of runs
// debug has levels, 0 off, 1 basic ....



void dp_theta_comp(string kin = "", string tgt="", string suf ="1",  int max_runs=-1, int max_events=-1, int debug=1)
{

	SetStyles();

	vector<int> runlist=SQL_Kin_Target(kin,tgt,suf);	
			
	//Defs with size of number of runs
	TCanvas *C[2];
	
	TChain *dataTree[runs.size()];
	TChain *mcTree[runs.size()];
	
	vector<double> run_cor;
	

	//kin info
	double p0=3.1;
	if(kin=="16") p0=2.9;
	//mc lumin info
	double phasespace = 4.0 * (0.2 * p0) * 0.1 * 0.1 *1000;  ///
	double ngen = 1000000; 


	//Information that might be used for both 1d and 2d histos
	double total_run_lumin=0.0;  //(Charge * dens / atomicmass)
	double total_mc_lumun=0.0;   // ngen/phase_space
	TCut data_cut="1";
	string mc_cut="1";
	string data_w = "1.0";
	string weight ="1.0";
	double mc_lumin=1.0;
	
	int G_runs=0;
	int total_g_events=0;
	int g_events;

	for(unsigned int i=0;i<runs.size();i++)
	{
		if(max_events==-1)
		{
			if(G_runs>=max_runs)
			{
				cout << "Hit max runs needed\n\n\n";
				break;
			}
		else
		{
			if(
		//Load of the tree, if it does not exist exit
		int run = runs[i]; 
		if(debug){cout <<"Looking at run " << run <<"\n";}
		RunInfo RI = GetRunInfo(run);
		if(RI.good_run==0){PrintRunInfo(run);}
		dataTree[i] = LoadRun(run);
		if(dataTree[i]==nullptr){ if(debug){cout<<"No data";}
			if(runs.size()==1){exit(1);}
			else{continue;}}	
		mcTree[i]=LoadMC(run,tarid);
		if(mcTree[i]==nullptr){ if(debug){cout<<"No MC";}
			if(runs.size()==1){exit(1);}
			else{continue;}}	
	
		//Get run info from the SQL db
		CODASetting coda = GetCODASetting(run);
		TargetInfo tgt_info= GetTargetInfo("",-999,run);	
		if(debug){
			cout << "Run and tgt info" <<"\n";
			cout << coda.trigger <<" "<< tgt_info.name <<"\n";
			}
		//Get analysis info
		AnalysisInfo runinfo = GetAnalysisInfo(run);
		if(debug){
			cout << "Run info" <<"\n";
			cout << runinfo.charge<<"\n";
			cout << "number of MC counts\n";
			cout << mcTree[i]->GetEntries()<<"\n";
			}
                int debug_flag=0; if(debug){debug_flag=1;}
                vector<double>luminosity_run={2,0.0};
                luminosity_run=Calc_lum(run,debug_flag);
		lumin=luminosity_run[0];

		
		data_cut = electron_cut_L+dp_cut_L+th_cut_L+ph_cut_L+z_cut_L;//acc_cut_L+track_L;
		mc_cut = Form("(fabs(yptar)<=%.3f&&fabs(xptar)<=%.3f&&fabs(ztar)<=%.3f&&fabs(delta)<=%.3f)",tg_ph_L,tg_th_L,tg_vz_L*100, tg_dp_L*100.0 ) ;
			


		G_runs++;
		total_g_events+=g_events;	
