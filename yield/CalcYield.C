/////////////////////////////////////////////////////////////////////////////////
//	This script will calculate the Yield of a kinemtaic 
//	and deposit that value along withe bin value and error 
//	on the yield into a txt file: 
//
//	Will look at doing this for theta and Xbj, 
/////////////////////////////////////////////////////////////////
#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"

const string runl_dir = "/work/halla/triton/Runlist/";

void CalcYield(string tgt ="", string kin="",  int debug=1)
{
	if(kin=="" || tgt ==""){
		cout << "Please enter the  kin and tgt you would like to use" <<"\n";
		cin >> kin;
		cin >> tgt;
		}
	
	vector<int>missing_runs;
	
	vector<int>runlist;
	vector<int>runlist1;
	
	//open up the run list file
	string runl_name = runl_dir+tgt+"_kin"+kin+".dat";
	kin_file K_File;
	K_File.set_file(runl_name,debug);
	if(K_File.run_file_status==0) { cout << "run list file does not exsit" <<"\n"; exit(1);}
	runlist=K_File.run_vect;
	//////////////////////////////////
	
	/// SQL to get run list
	vector<RunList> RList;
	RList = SQL_Kin_Target_RL(kin,tgt);
	runlist1 = RLtoint(RList);
	if(debug){for(unsigned int z=0;z<runlist.size();z++){cout<<runlist[z]<<" ";}}
	if(debug)cout<<"\n";

	//A functionk to get radcor for this kin...
	//this we mostly likly be a function of eprime and theta
	double rad_cor=1;
	////
	
	//Number of bins and step per kin
	int bins = 25;
	double th_step = 0.75;
	double x_step  = 0.02;
	/// vectors of info
	vector<double> tmpv(2,0.0);
	vector< double> Xbj_yield(bins,0.0);  //yield 
	vector< double> theta_yield(bins,0.0);
	vector< double> Xbj_yield_err(bins,0.0);  //yield error 
	vector< double> theta_yield_err(bins,0.0);
	vector<double> avg_xb(bins,0.0);//Average value for printing to file
	vector<double> avg_theta(bins,0.0);
	vector<double> xbj_cen(bins,0.0); //Centeral value or cut value for bin selecetion
	vector<double> theta_cen(bins,0.0);
	vector<double> xbj_raw_e(bins,0.0); //raw number of electrons;
	vector<double> theta_raw_e(bins,0.0);
	vector<double> xbj_tot(bins,0.0); //used to calculate the average;
	vector<double> theta_tot(bins,0.0);
	
	//debuging output file for run by run info
	ofstream out; out.open("out.txt");

	for(int z=0;z<bins;z++){
		xbj_cen[z]  =0.16+z*x_step;
		theta_cen[z]=13.0+z*th_step;
		out<< z <<" ";
	}
		out<<"\n";
	//Total luminosity for the setting		
	double total_lumin=0.0;
	double tot_lumin_err=0.0;


	//Loop through the runs over i
	for(unsigned int i=0; i<runlist.size();i++)
	{
		int run = runlist[i]; //Get run number
	 	if(!SQLRunlistStatus(run)){
			cout <<"Skipping run " << run <<"\n";
			missing_runs.push_back(run);
			continue;}
		if(debug>1) cout <<"Starting Run " <<run<<"\n";
		//Use SQL to retrieve run info;
		CODASetting coda = GetCODASetting(run);
		AnalysisInfo ana_info = GetAnalysisInfo(run);
	
		//Calculate the correction on a run bases. LT, det. eff.  ....
		//I will store this in SQL, I will use the DB to store these run by run values
		double correction_error=0;
		double run_correction= SQLRunCorrection(run, correction_error, 1, debug);
		int debug_flag=0; if(debug){debug_flag=1;}	
		vector<double>luminosity_run={2,0.0};
		luminosity_run=Calc_lum(run,debug_flag);
		total_lumin+=luminosity_run[0];
		tot_lumin_err += pow(luminosity_run[0],2);
		//Open the tree from (run)
		TChain *T = LoadRun(run);
	 	int totn = T->GetEntries();	
		//Shorten up the tree to only look at good electron events;
		TCut total_cut;
		if(coda.arm=="L")total_cut = electron_cut_L&&acc_cut_L;
		else{total_cut = electron_cut_R&&acc_cut_R;}
		T->Draw(">>GoodEs",total_cut);
		TEventList *GoodEs;
		gDirectory->GetObject("GoodEs",GoodEs);
		T->SetEventList(GoodEs);
		////////////////
			
		//Defining the vars address to the tree
		double eprime =0;   double theta  =0;
		double x_bj   =0;   double Qsqrd  =0;				
		
		//Reset branch address from any previous issues
		T->ResetBranchAddresses();
		T->SetBranchStatus("*",0);//Turn off all branches
		T->SetBranchStatus(Form("%s.gold.p",coda.arm.Data())	,1);//Turn on eprime
		T->SetBranchStatus(Form("EK%sx.angle",coda.arm.Data())	,1);//scattered angle
		T->SetBranchStatus(Form("EK%sx.x_bj",coda.arm.Data())	,1);//x_bj
		T->SetBranchStatus(Form("EK%sx.Q2",coda.arm.Data())	,1);//Q2
		///Address those branches
		T->SetBranchAddress(Form("%s.gold.p",coda.arm.Data()),  &eprime);
	        T->SetBranchAddress(Form("EK%sx.angle",coda.arm.Data()),&theta);
		T->SetBranchAddress(Form("EK%sx.x_bj",coda.arm.Data()), &x_bj);
		T->SetBranchAddress(Form("EK%sx.Q2",coda.arm.Data()),	&Qsqrd);
		///////////////////////

		//vector for just this runs electrons;
		vector<int> N_ele_x(bins,0);
		vector<int> N_ele_th(bins,0);
		vector<double> xbj_run(bins,0.0); //used to calculate the average;
		vector<double> theta_run(bins,0.0);
		vector< double> Xbj_yield_run(bins,0.0);  //yield per run
		vector< double> theta_yield_run(bins,0.0);

		//Loop through all the events in the Good electron list over event
		int nentries =  GoodEs->GetN();
		if(debug)cout << nentries << " " <<totn <<"\n";
		
	
		for(int event=0; event<nentries;event++)
		{
			T->GetEntry(GoodEs->GetEntry(event));
			theta=theta/rad;
			//Get the rad correction for this event;
			//double rac_cor = RadCor();
			//Look at the bins
			for(int i=0;i<bins;i++)
			{

				double xbj_diff   = x_bj-xbj_cen[i];
				double theta_diff= theta-theta_cen[i];
				if(abs(xbj_diff) <x_step && xbj_diff >0)
				{//This event falls into the ith bin for x
					Xbj_yield_run[i] += 1.0/(run_correction*luminosity_run[0]);
					Xbj_yield[i]+=1.0/(run_correction);
					xbj_raw_e[i]++;
					xbj_tot[i]+=x_bj;
					xbj_run[i]+=x_bj;
					N_ele_x[i]++;		//gets reset			
				}//end of xbin		
				if(abs(theta_diff)<th_step && theta_diff>0)
				{//this event is in this theta bin
					theta_yield_run[i] += 1.0/(run_correction*luminosity_run[0]);
					theta_yield[i] += 1.0/run_correction;
					theta_raw_e[i]++;
					theta_tot[i]+=theta;					
					theta_run[i]+=theta;					
					N_ele_th[i]++;		//gets reset				
				}//end of th_bin
				
			if(debug>=4){if(event/10000 == event/10000.0){
			
		cout<<event/(nentries*1.0)<<" "<<i<<" : "<< xbj_diff <<" " << theta_diff <<" "<< N_ele_x[i] <<" "<< N_ele_th[i] <<endl;
				}}
			}//end of bins
		}//End of event loop!!!

		//Counter for num of bins;
		int xbins =0;
		int thbins=0;

		//calculate the errr for this run and sum it together
		double tmp_err_x=0.0; //Used to sum up error
		double tmp_err_th=0.0; //Used to sum up error
		for(int i=0;i<bins;i++)
		{
			if(N_ele_x[i]>0){
				xbins++;
				tmp_err_x = sqrt( 1/N_ele_x[i] + pow((correction_error/run_correction),2) + pow((luminosity_run[1]/luminosity_run[0]),2) ) * N_ele_x[i]/(run_correction*luminosity_run[0]); 
			}
			Xbj_yield_err[i]+=pow(tmp_err_x,2);			
			if(N_ele_th[i]>0){
				thbins++;
				tmp_err_th = sqrt( 1/N_ele_th[i] + pow((correction_error/run_correction),2) + pow((luminosity_run[1]/luminosity_run[0]),2) ) * N_ele_th[i]/(run_correction*luminosity_run[0]); 					}
			theta_yield_err[i]+=pow(tmp_err_th,2);			
		}//end of error bins

		//Clean up
		delete T;
		if(debug>=3){
			cout << "theta   :  yield     :error " <<"\n";
			for(int z=0;z<bins;z++){
				if(N_ele_th[z]!=0)
cout <<theta_run[z]/N_ele_th[z]<<"\t"<< theta_yield_run[z]<<"  "<< theta_yield_err[z] << "\n";

	out<< theta_yield_run[z]<<" ";

				}//end of z loop
			}//end of debug print statment	
	}//End of run loop	
	//////////////////////////////////////////////////////////////////////
	
	out<<"\n";

	tot_lumin_err =sqrt(tot_lumin_err);

	//Print all this info to file for plotting
	//output file txt file
	
	ofstream xout; xout.open(Form("./yield_output/xbj/%s_kin%s.dat",tgt.c_str(),kin.c_str()));
	xout << "Xbj\t"<<"Yield\t"<<"Error\n";
	ofstream thout; thout.open(Form("./yield_output/theta/%s_kin%s.dat",tgt.c_str(),kin.c_str()));
	thout << "theta\t"<<"Yield\t"<<"Error\n";

	for(int i=0;i<bins;i++)
	{
		theta_yield[i] /= total_lumin;
		Xbj_yield[i] /= total_lumin;
		thout << theta_tot[i]/theta_raw_e[i]<<"\t"<<theta_yield[i]<<"\t"<<theta_yield_err[i]<<"\n";
		xout << xbj_tot[i]/xbj_raw_e[i]<<"\t"<<Xbj_yield[i]<<"\t"<<Xbj_yield_err[i]<<"\n";	
	}	
	
	xout.close();
	thout.close();
	out.close();


	if(debug){cout<<"\n\n";
		for(unsigned int z=0;z<missing_runs.size();z++){
			cout<< missing_runs[z]<<"  ";}
		cout<<"\n\n";}




	exit(1);	
}//end of program	
