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

const string runl_dir = "/home/jbane/tritium/replay/HallA-Online-Tritium/replay/scripts/Runlist/"; 
//"/home/jbane/tritium/Runlist/";

void RunYield(int runnum, string tgt ="", string kin="",int whichRL=2,  int debug=3)
{
	if(kin=="" || tgt ==""){
		cout << "Please enter the  kin and tgt you would like to use" <<"\n";
		cin >> kin;
		cin >> tgt;
		}
	
	vector<int>missing_runs;
	
	vector<int>runlist2;
	vector<int>runlist1;
	
	//open up the run list file
	string runl_name = runl_dir+tgt+"_kin"+kin+".dat";
	kin_file K_File;
	K_File.set_file(runl_name,debug);
	if(K_File.run_file_status==0) {}// exit(1);}
	runlist1=K_File.run_vect;
//	if(debug){for(unsigned int z=0;z<runlist1.size();z++){cout<<runlist1[z]<<" ";}}
	//////////////////////////////////
	
	/// SQL to get run list
	vector<RunList> RList;
	string suf="1";
//	RList = SQL_Kin_Target_RL(kin,tgt,suf);
//	runlist2 = RLtoint(RList);
	runlist2= SQL_Kin_Target(kin,tgt,suf);
//	if(debug){for(unsigned int z=0;z<runlist2.size();z++){cout<<runlist2[z]<<" ";}}
//	if(debug)cout<<"\n";

	
	vector<int>runlist;
	if(whichRL==1){runlist=runlist1;}
	else		{runlist=runlist2;}
	
	if(runlist.size()<1){
		cout << runlist.size() <<endl;
		cout << "There are no runs found in this kinemtaic, switching list and checking\n";
		if(whichRL==1)runlist=runlist2;
		else runlist=runlist1;
		cout << runlist.size() <<endl;
		if(runlist.size()<1){cout <<"No runs in list, exiting\n"; exit(1);}
	}

	if(debug){for(unsigned int z=0;z<runlist.size();z++){cout<<runlist[z]<<" ";}}


	//A functionk to get radcor for this kin...
	//this we mostly likly be a function of eprime and theta
	double rad_cor=1;
	////
	
	//Number of bins and step per kin
	int bins = 100;
	double th_step = (37.7-15)/(bins*1.0);
	double x_step  = (0.85-0.15)/(bins*1.0);
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
		xbj_cen[z]  =0.15+z*x_step;
		theta_cen[z]=15.0+z*th_step;
		out<< z <<" ";
	}
		out<<"\n";
	//Total luminosity for the setting		
	double total_lumin=0.0;
	double tot_lumin_err=0.0;
	double pos_err_kin=0.0;
	double pos_cor_kin=0.0;

	runlist.clear();
	runlist.push_back(runnum);

	//Loop through the runs over i
	for(unsigned int i=0; i<runlist.size();i++)
	{
		int run = runlist[i]; //Get run number

		if(run >= 2248 && run <2566)continue;


	 	if(!SQLRunlistStatus(run)){
			cout <<"Skipping run " << run <<"\n";
			missing_runs.push_back(run);
			continue;}
		if(debug>1) cout <<"Starting Run " <<run<<"\n";
		//Use SQL to retrieve run info;
		CODASetting coda = GetCODASetting(run);
		RunInfo runinfo  = GetRunInfo(run);
		if(debug>1){
			cout << "Good Run ? " << runinfo.good_run <<"\n";

			if(runinfo.good_run==0){PrintRunInfo(run);}//continue;}
			}
//		AnalysisInfo ana_info = GetAnalysisInfo(run);
	
		//Calculate the correction on a run bases. LT, det. eff.  ....
		//I will store this in SQL, I will use the DB to store these run by run values
		double correction_error=0;
		double run_correction= SQLRunCorrection(run, correction_error, 1, debug);


		//Deat time is applied through SQL
		if(debug) cout << "Corrections :: "<<run_correction<<" error "<<correction_error<<"\n";
		//////Calculate the luminosty and the error on that, this err includes
		// tgt thickness and a hard coded 1% for beam charge
		int debug_flag=0; if(debug){debug_flag=1;}	
		vector<double>luminosity_run={2,0.0};
		luminosity_run=Calc_lum(run,debug_flag);
		if(luminosity_run[1]!=luminosity_run[1]){
			luminosity_run[1]=0.0;
			}
		total_lumin+=luminosity_run[0];
		tot_lumin_err += pow(luminosity_run[1],2);
		
		///////////////////////////////////////////
		

		vector<double> density_cor = DensityCor(run);
		if(debug)cout << "Density cor "<< density_cor[0] <<" " << density_cor[1] <<endl;	
//		density_cor[0]=1.0;		


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


		double avg_pos=0.0;
		PositronCor PC= GetPosInfo(run);
		double pos_cor_err_run=0;
	

//Loop through all the events in the Good electron list over event
		int nentries =  GoodEs->GetN();
		if(debug)cout << nentries << " " <<totn <<"\n";
			


	
	
		for(int event=0; event<nentries;event++)
		{
			T->GetEntry(GoodEs->GetEntry(event));
			theta=theta/rad;
			double event_cors=1.0;
			//positron correction, 
			vector<double> pos_cor = GetPosCorFactor(x_bj,PC);
			pos_cor[0]=1.0-pos_cor[0];
			if(pos_cor[0]<0.0)pos_cor[0]=1.0;
			
			if(pos_cor[0] != pos_cor[0] || 1.0/pos_cor[0] ==0) pos_cor[0] = 1.0;
			event_cors*=pos_cor[0];
			avg_pos=avg_pos+pos_cor[0];
			pos_cor_err_run+=pos_cor[1];

//		        cout << avg_pos/(event *1.0) <<endl;	


			//Get the rad correction for this event;
			//double rac_cor = RadCor();
			//Look at the bins
			for(int i=0;i<bins;i++)
			{

				double xbj_diff   = x_bj-xbj_cen[i];
				double theta_diff= theta-theta_cen[i];
				if(abs(xbj_diff) <x_step && xbj_diff >0)
				{//This event falls into the ith bin for x
					Xbj_yield_run[i] += 1.0/(run_correction
						*luminosity_run[0]*density_cor[0]);
/*yield for the kin*/			Xbj_yield[i]+=1.0/(event_cors);
					xbj_raw_e[i]++;
					xbj_tot[i]+=x_bj;
					xbj_run[i]+=x_bj;
					N_ele_x[i]++;		//gets reset			
				}//end of xbin		
				if(abs(theta_diff)<th_step && theta_diff>0)
				{//this event is in this theta bin
					theta_yield_run[i] += 1.0/(run_correction
						*luminosity_run[0]*density_cor[0]);
/*yield for the kin*/			theta_yield[i] += 1.0/(event_cors);
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
		avg_pos = avg_pos/nentries;
		
		if(debug) cout << "Average positron correction = " << avg_pos<<" "<<pos_cor_err_run/nentries <<endl;

		//reset the pos average
		avg_pos=0.0;

		//Counter for num of bins;
		int xbins =0;
		int thbins=0;

		//calculate the errr for this run and sum it together
		vector<double> tmp_err_x(bins,0.0); //Used to sum up error
		vector<double> tmp_err_th(bins,0.0); //Used to sum up error
		for(int i=0;i<bins;i++)
		{
			if(N_ele_x[i]>0){
				xbins++;
				//just for this run!
				tmp_err_x[i] = sqrt( 1/sqrt(N_ele_x[i]) + pow((correction_error/run_correction),2) + pow((luminosity_run[1]/luminosity_run[0]),2) *pow(Xbj_yield_run[i],2) ); 
			}
			Xbj_yield_err[i]+=pow(tmp_err_x[i],2);			
			
			//Apply any run corrections to the yield from all the events
			Xbj_yield[i]*=1.0/(density_cor[0]*run_correction);
			theta_yield[i]*=1.0/(density_cor[0]*run_correction);
			/////////////////////////////////////////////////////////////

			if(N_ele_th[i]>0){
				thbins++;
				//just for this run!
				tmp_err_th[i] = sqrt( (1/sqrt(N_ele_th[i]*1.0)) + pow((correction_error/run_correction),2) + pow((luminosity_run[1]/luminosity_run[0]),2) *pow(theta_yield_run[i],2) );
			}
			theta_yield_err[i]+=pow(tmp_err_th[i],2);			
		}//end of error bins

		//Clean up
		delete T;

		//theta file by run


		if(debug>=3){
			ofstream thbyrun; thbyrun.open(Form("./yield_output/byrun/%d.dat",run));
			cout << "theta   :Ne         yield     :error " <<"\n";
			thbyrun<< "theta   :Ne         yield     :error " <<"\n";
			for(int z=0;z<bins;z++){
				if(N_ele_th[z]!=0)
cout <<theta_run[z]/N_ele_th[z]<<"\t"<<N_ele_th[z]<<" "<< theta_yield_run[z]<<"  "<< tmp_err_th[z]<<"  "<< sqrt(theta_yield_err[z]) << "\n";
thbyrun <<theta_run[z]/N_ele_th[z]<<"\t"<<N_ele_th[z]<<" "<< theta_yield_run[z]<<"  "<< tmp_err_th[z] << "\n";

	out<< theta_yield_run[z]<<" ";

				}//end oif z loop
			out<<"\n";
			thbyrun.close();
			}//end of debug print statment	
	}//End of run loop	
	//////////////////////////////////////////////////////////////////////
	
	out<<"\n";

	tot_lumin_err =sqrt(tot_lumin_err);
	if(debug) cout <<"Total Lumin: " <<total_lumin << " err " << tot_lumin_err <<endl;

	//Print all this info to file for plotting
	//output file txt file
	
//	ofstream xout; xout.open(Form("./yield_output/xbj/%s_kin%s.dat",tgt.c_str(),kin.c_str()));
//	xout << "Xbj\t"<<"Ne\t"<<"Yield\t"<<"Error\n";
//	ofstream thout; thout.open(Form("./yield_output/theta/%s_kin%s.dat",tgt.c_str(),kin.c_str()));
//	thout << "theta\t"<<"Ne\t"<<"Yield\t"<<"Error\n";

	/*for(int i=0;i<bins;i++)
	{
		theta_yield[i] /= total_lumin;
		Xbj_yield[i] /= total_lumin;
		
		double total_error_th = theta_yield[i]*sqrt( pow(theta_yield_err[i]/(theta_yield[i]*1.0),2) + pow(tot_lumin_err/total_lumin  ,2) );	
		double total_error_x= Xbj_yield[i]*sqrt( pow(Xbj_yield_err[i]/(Xbj_yield[i]*1.0),2) + pow(tot_lumin_err/total_lumin  ,2) );	
			
		thout << theta_tot[i]/theta_raw_e[i]<<"\t"<<theta_raw_e[i]<<"\t"<<theta_yield[i]<<"\t"<<total_error_th<<"\n";
		xout << xbj_tot[i]/xbj_raw_e[i]<<"\t"<<xbj_raw_e[i]<<"\t"<<Xbj_yield[i]<<"\t"<<total_error_x<<"\n";	
	}	
	
	xout.close();
	thout.close();
	out.close();
*/

	if(debug){cout<<"\n\n";
		for(unsigned int z=0;z<missing_runs.size();z++){
			cout<< missing_runs[z]<<"  ";}
		cout<<"\n\n";}




	exit(1);	
}//end of program	
