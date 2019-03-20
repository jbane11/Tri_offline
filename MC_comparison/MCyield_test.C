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
// "/home/jbane/tritium/Runlist/";

void MCyield_test(string tgt ="", string kin="", int bins=50, int whichRL=2,int onerun=0, int tarid=0,int debug=3)
{
	int clean =0;
	if(kin=="" || tgt ==""){
		cout << "Please enter the  kin and tgt you would like to use" <<"\n";
		cin >> kin;
		cin >> tgt;
		}

//	double kin_angle =GetKinAngle
	double p0=3.1;
	if(kin=="16")p0=2.9;


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

	string suf="1";
	/// SQL to get run list
	vector<RunList> RList;
	RList = SQL_Kin_Target_RL(kin,tgt);
//	runlist2 = RLtoint(RList,kin);

	runlist2 =SQL_Kin_Target(kin,tgt,suf);
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
        if(debug) cout <<"\n\n"<< "Will look at " << runlist.size() << " runs!\n\n";

	int num_G_runs=0;

	//A functionk to get radcor for this kin...
	//this we mostly likly be a function of eprime and theta
	double rad_cor=1;
	////

	double avg_bin=0;
	//Number of bins and step per kin
  vector<double> xbj_base(bins,0.0);
  vector<double> theta_base(bins,0.0);
  double th_step = (37.7-15)/(bins*1.0);
  double x_step  = (1.00-0.00)/(bins*1.0);
  for(int z=0;z<bins;z++){
          xbj_base[z]  =0.00+z*x_step;
          theta_base[z]=15.0+z*th_step;
  }
////////////////
	vector<int>     theta_ele(bins,0);      //Number of electrons in theta bin
  vector<int>     xbj_ele(bins,0);        //Number of electrons in x bin
  vector<double>  theta_yield(bins,0.0);  //N of es corrected with DT, density....
  vector<double>  xbj_yield(bins,0.0);    //N of es corrected with DT, density....
  vector<double>  xbj_total(bins,0.0);    //use to find the bin center
  vector<double>  Q_total(bins,0.0);    //use to find the bin center
  vector<double>  theta_total(bins,0.0);
  vector<double>  Ep_total(bins,0.0);

  int intkin = stoi(kin);
  double RCTEST = RC_factor(intkin,tgt,1,1,1);

  double total_lumin=0.0;
  double total_lumin_err=0.0;
  double total_run_correction_err=0.0;

  TChain *T = new TChain; // TChain for root file
  TEventList *GoodEs;     // Only the good electron events.
  vector<double> Run_lumin(2,0.0);
/////Start of run loop
  for(unsigned int j = 0; j < runlist.size();j++)
  {
	  int run = runlist[j];   //Get run number
    if(debug) printf("\n\n\tStarting run %d :  %d out of %zu \n",run,j+1, runlist.size());
		if(tarid==0) T=LoadMC(run);
		else{ T= LoadMC(run,tarid);}

		string MCdir = "~/halla_xem/weight_T2/";
		string curdir = "/home/jbane/tritium/Tri_offline/MC_comparison/";

		int tarid_t;
		if(tgt=="D2")tarid_t=2;
		else if(tgt=="H3")tarid_t=3;
		else if(tgt=="He3")tarid_t=4;
		else if(tgt=="C12")tarid_t=7;
		else if(tgt=="H")tarid_t=1;
		else tarid_t=10;


		bool newfile;
		newfile = isnewer( Form("/run/media/jbane/Slim/mc%d.root",run));
		cout<<"Is the mc root file newish : " << newfile <<endl;
		if(T == nullptr || !newfile || clean){
			gSystem->Exec(Form("rm -vf ./stdout/%d.txt",run));
	    gSystem->Exec(Form("cd %s",MCdir.c_str()));
			cout<< Form("Making MC file with %s %d %d ",tgt.c_str(),intkin,run)<<endl;
	    gSystem->Exec(Form("sh %s/T2_MC %s %d %d %d >> %s/stdout/%d.txt", MCdir.c_str(), tgt.c_str(), intkin, run, tarid_t, curdir.c_str(), run));
			//gSystem->Exec(Form("%s/T2_MC %s %d %d %d", MCdir.c_str(), tgt.c_str(), intkin, run, tarid_t));
	    gSystem->Exec(Form("cd %s",curdir.c_str()));
			if(tarid==0) T=LoadMC(run);
			else{ T= LoadMC(run,tarid);}
		}

    if(T==nullptr){ continue;  }
		if(T==nullptr && num_G_runs >5)continue;

    int totn = T->GetEntries();



		CODASetting coda = GetCODASetting(run);

		double phase_space = 24.799;
		double ngen=1e6;


		//Shorten up the tree to only look at good electron events;
		TCut total_cut;
		if(coda.arm=="L"){
		total_cut = Form("(w2>%.3f&&fabs(yptar)<=%.3f&&fabs(xptar)<=%.3f&&fabs(ztar)<=%.3f&&fabs(delta)<=%.3f)", wsqr, tg_ph_L, tg_th_L, tg_vz_L*100,  tg_dp_L*100.0 ) ;
				//total_cut+=mc_ap_L;
			}
		else{
			total_cut = Form("(w2>%.3f&&fabs(yptar)<=%.3f&&fabs(xptar)<=%.3f&&fabs(ztar)<=%.3f&&fabs(delta)<=%.3f)", wsqr, tg_ph_R, tg_th_R, tg_vz_R*100, tg_dp_R*100.0 ) ;	}
		T->Draw(">>GoodEs",total_cut);
		gDirectory->GetObject("GoodEs",GoodEs);
		T->SetEventList(GoodEs);

	//	TH1F *tmp_h; T->Draw(

		float eprime =0;   float theta  =0;
		Float_t x_bj   =0;   float Qsqrd  =0;
		Float_t weight=0;  Float_t born=0;
		Float_t rc =1;
		//Reset branch address from any previous issues
		T->ResetBranchAddresses();
		T->SetBranchStatus("*",1);//Turn off all branches
		///Address those branches
		T->SetBranchAddress("delta",  &eprime);
	        T->SetBranchAddress("th_spec",&theta);
		T->SetBranchAddress("xbj", &x_bj);
		T->SetBranchAddress("q2", &Qsqrd);
		T->SetBranchAddress("yield", &weight);
		T->SetBranchAddress("born", &born);
		T->SetBranchAddress("rci", &rc);

//Loop through all the events in the Good electron list over event
		int nentries =  GoodEs->GetN();
		if(debug)cout << nentries << " " <<totn <<"\n";
//Loop throught the goof events!
                for(int event=0; event<nentries;event++)
                {
	                T->GetEntry(GoodEs->GetEntry(event)); //Use the GEs list to select good events.
                        theta=theta/rad;// convert radians to degrees for scattering anglei

			//loop through all of the bins, checking both x and theta!
			for(int i=0;i<bins;i++)
                        {

                                double xbj_diff   = x_bj-xbj_base[i];
                                double theta_diff= theta-theta_base[i];
                                if(abs(xbj_diff) <x_step && xbj_diff >0)
                                {//This event falls into the ith bin for x
                                        xbj_ele[i]++;
                                        xbj_yield[i]+=1*(born/rc)*phase_space/ngen;
                                        xbj_total[i]+=x_bj;
					Q_total[i]+=Qsqrd;
                                }//end of xbin
                                if(abs(theta_diff)<th_step && theta_diff>0)
                                {//this event is in this theta bin
                                        theta_ele[i]++;
                                        theta_yield[i]+=1*(born*rc)*phase_space/ngen;
                                        theta_total[i]+=theta;
					Ep_total[i]+=p0*(1+eprime/100.0);
                                }//end of th_bin
                        }//end of bins loop

		if(onerun) break;
		}//End of event loop!!

                GoodEs=nullptr;
                T=nullptr;
		num_G_runs++;
		int first_bin=-1;
		int last_bin=-1;
		double g_bins=0;
		for(int k=0;k<bins;k++){
			if(xbj_ele[k]>0){
				last_bin=k;
				if(first_bin==-1){first_bin=k;}
				avg_bin+=xbj_ele[k];
				g_bins++;
				}
			}
		avg_bin= avg_bin/g_bins;
		if(debug>=2){cout<< "X runs from " << xbj_total[first_bin]/xbj_ele[first_bin]<<" "<<xbj_total[last_bin]/xbj_ele[last_bin] << endl;}
		if(debug>=3){
	 		cout << "xbj \t count \t yield"<<endl;
			for(int k=0; k<bins;k++){
				if(xbj_ele[k]>0){
					cout<<  k<<"  "<<setprecision(5)<<xbj_total[k]/xbj_ele[k]<<"\t"<<xbj_ele[k]<<"\t"<<xbj_yield[k]/(num_G_runs*1.0)<<endl;
					}
				}
			}



		if(debug){if(avg_bin>=12000){cout <<" Got enough data "<<endl;break;}}
        }//End  of run loop!
	if(num_G_runs<1){
		cout << "No good runs " <<endl;}

	if(debug) cout << "Bin average = "<<avg_bin <<endl;

        vector<double>   xbj_stat_error(bins,0.0);
        vector<double> theta_stat_error(bins,0.0);

        vector<double>   xbj_error(bins,0.0);
        vector<double> theta_error(bins,0.0);

        for(int z=0;z<bins;z++)
        {
                if(xbj_ele[z]>0)
                {
                        xbj_stat_error[z]  =(1.0/(sqrt((xbj_ele[z]*1.0) )*1.0));


                }
                if(theta_ele[z]>0)
                {
                        theta_stat_error[z]=(1.0/(sqrt((theta_ele[z]*1.0))*1.0));
                }
        }


	ofstream xout; xout.open(Form("./yield_output/%s%dbins/xbj/%s_kin%s.dat",cuttype.c_str(),bins,tgt.c_str(),kin.c_str()));
       	xout << "Xbj\t"<<"Q2\t"<<"Ne\t"<<"Yield\t"<<"Error\n";
        ofstream thout; thout.open(Form("./yield_output/%s%dbins/theta/%s_kin%s.dat",cuttype.c_str(),bins,tgt.c_str(),kin.c_str()));
        thout << "theta\t"<<"E`\t"<<"Ne\t"<<"Yield\t"<<"Error\n";

if(debug)cout << "xbj \t" << " yield\t" << "   stat eror"<<endl;

        for(int i=0;i<bins;i++)
        {
		double total_error_th = theta_yield[i]*theta_stat_error[i];// + pow(tot_lumin_err/total_lumin  ,2) );
                double total_error_x= xbj_yield[i]*xbj_stat_error[i];// + pow(tot_lumin_err/total_lumin  ,2) );

                thout << theta_total[i]/theta_ele[i]<<"\t"<<Ep_total[i]/theta_ele[i]<<"\t"<<theta_ele[i]<<"\t"<<theta_yield[i]/(num_G_runs*1.0)<<"\t"<<total_error_th/(num_G_runs*1.0)<<"\n";
                xout << xbj_total[i]/xbj_ele[i]<<"\t"<<Q_total[i]/xbj_ele[i]<<"\t"<<xbj_ele[i]<<"\t"<<xbj_yield[i]/(num_G_runs*1.0)<<"\t"<<total_error_x/(num_G_runs*1.0)<<"\n";
	if(xbj_ele[i]>0) cout << xbj_total[i]/xbj_ele[i] <<"\t"<<xbj_yield[i]/(num_G_runs*1.0) <<"\t" <<(total_error_x/(num_G_runs*1.0))/(xbj_yield[i]/(num_G_runs*1.0)) <<endl;

        }

        xout.close();
        thout.close();

	exit(1);
}//end of program


/*
	double th_step = (37.7-15)/(bins*1.0);
	double x_step  = (0.95-0.15)/(bins*1.0);
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

	int num_G_runs=0;
	//Loop through the runs over i
	for(unsigned int i=0; i<runlist.size();i++)
	{
		int run = runlist[i]; //Get run number
	 	if(!SQLRunlistStatus(run)){
		//	cout <<"Skipping run " << run <<"\n";
		//	missing_runs.push_back(run);
			continue;}
		if(debug>1) cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
		if(debug>1) cout <<"Starting Run " <<run<<"\n";
		//Use SQL to retrieve run info;
		CODASetting coda = GetCODASetting(run);
		RunInfo runinfo  = GetRunInfo(run);
		if(debug>1){
			cout << "Good Run ? " << runinfo.good_run <<"\n";

			if(runinfo.good_run==0){PrintRunInfo(run);}//continue;}
			}
//		AnalysisInfo ana_info = GetAnalysisInfo(run);

		double phase_space = 24.799;
		double ngen=1e6;
		//Calculate the correction on a run bases. LT, det. eff.  ....
		//I will store this in SQL, I will use the DB to store these run by run values
		double correction_error=0;
		double run_correction= SQLRunCorrection(run, correction_error, 1, debug);
		if(debug) cout << "Corrections :: "<<run_correction<<"  error " << correction_error<<"\n";
		int debug_flag=0; if(debug){debug_flag=1;}
		vector<double>luminosity_run={2,0.0};
		luminosity_run=Calc_lum(run,debug_flag);
		total_lumin+=luminosity_run[0];
		tot_lumin_err += pow(luminosity_run[1],2);
		//Open the tree from (run)
		TChain *T = LoadMC(run);
		string MCdir = "~/halla_xem/weight_T2/";
		string curdir = "/home/jbane/tritium/Tri_offline/MC_comparison/";
		if(T==nullptr){
				gSystem->Exec(Form("cd %s",MCdir.c_str()));
				gSystem->Exec(Form("sh %s/T2_MC %s %s %d 10",MCdir.c_str(),tgt.c_str(),kin.c_str(),run));
				gSystem->Exec(Form("cd %s",curdir.c_str()));
				T= LoadMC(run);

				if(T==nullptr)exit(1);
				}
	 	int totn = T->GetEntries();
		//Shorten up the tree to only look at good electron events;
		TCut total_cut;
		total_cut = Form("(fabs(yptar)<=%.3f&&fabs(xptar)<=%.3f&&fabs(ztar)<=%.3f&&fabs(delta)<=%.3f)",tg_ph_L,tg_th_L,tg_vz_L*100, tg_dp_L*100.0 ) ;




		T->Draw(">>GoodEs",total_cut);
		TEventList *GoodEs;
		gDirectory->GetObject("GoodEs",GoodEs);
		T->SetEventList(GoodEs);
		////////////////

		//Defining the vars address to the tree
		float eprime =0;   float theta  =0;
		Float_t x_bj   =0;   float Qsqrd  =0;
		Float_t weight=0;  Float_t born=0;
		Float_t rc =1;
		//Reset branch address from any previous issues
		T->ResetBranchAddresses();
		T->SetBranchStatus("*",1);//Turn off all branches
		///Address those branches
		T->SetBranchAddress("delta",  &eprime);
	        T->SetBranchAddress("th_spec",&theta);
		T->SetBranchAddress("xbj", &x_bj);
		T->SetBranchAddress("q2", &Qsqrd);
		T->SetBranchAddress("yield", &weight);
		T->SetBranchAddress("born", &born);
		T->SetBranchAddress("rci", &rc);


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
					Xbj_yield_run[i] += 1.0*(born/rc)*phase_space/ngen;
					Xbj_yield[i]+=1.0*(born/rc)*phase_space/ngen;
					xbj_raw_e[i]++;
					xbj_tot[i]+=x_bj;
					xbj_run[i]+=x_bj;
					N_ele_x[i]++;		//gets reset
				}//end of xbin
				if(abs(theta_diff)<th_step && theta_diff>0)
				{//this event is in this theta bin
					theta_yield_run[i] += 1.0*(born/rc)*phase_space/ngen;//just this run
					theta_yield[i] += 1.0*(born/rc)*phase_space/ngen;//total
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
		vector<double> tmp_err_x(bins,0.0); //Used to sum up error
		vector<double> tmp_err_th(bins,0.0); //Used to sum up error
		for(int i=0;i<bins;i++)
		{
			if(N_ele_x[i]>0){
				xbins++;
				double run_stat_error = 1/sqrt(N_ele_x[i]*1.0);
				tmp_err_x[i]=run_stat_error*Xbj_yield_run[i];
				//tmp_err_x[i] = sqrt( 1/N_ele_x[i] + pow((correction_error/run_correction),2) + pow((luminosity_run[1]/luminosity_run[0]),2) ) * N_ele_x[i]/(run_correction*luminosity_run[0]);
			}
			Xbj_yield_err[i]+=pow(tmp_err_x[i],2);
			if(N_ele_th[i]>0){
				thbins++;
				double run_stat_error = 1/sqrt(N_ele_th[i]*1.0);
				tmp_err_th[i]=run_stat_error*theta_yield_run[i];
//				tmp_err_th[i] = sqrt( 1/N_ele_th[i] + pow((correction_error/run_correction),2) + pow((luminosity_run[1]/luminosity_run[0]),2) ) * N_ele_th[i]/(run_correction*luminosity_run[0]);
				}
			theta_yield_err[i]+=pow(tmp_err_th[i],2);
		}//end of error bins

		//Clean up
		delete T;
		if(debug>=3){
			ofstream thbyrun; thbyrun.open(Form("./yield_output/byrun/%d.dat",run));
			cout << "theta   :Ne         yield     :error " <<"\n";
			thbyrun << "theta   :Ne         yield     :error " <<"\n";
			for(int z=0;z<bins;z++){
				if(N_ele_th[z]!=0)
cout <<theta_run[z]/N_ele_th[z]<<"\t"<<N_ele_th[z]<<" "<< theta_yield_run[z]<<"  "<< tmp_err_th[z] << "\n";
thbyrun <<theta_run[z]/N_ele_th[z]<<"\t"<<N_ele_th[z]<<" "<< theta_yield_run[z]/1000.0<<"  "<< tmp_err_th[z]/1000.0 << "\n";

	out<< theta_yield_run[z]<<" ";

				}//end of z loop
			out<<"\n";
			thbyrun.close();
			}//end of debug print statment
	num_G_runs++;
	}//End of run loop
	//////////////////////////////////////////////////////////////////////

cout << "NUmber of good runs " << num_G_runs <<endl;
	tot_lumin_err =sqrt(tot_lumin_err);

	if(debug) cout <<"Total Lumin: " <<total_lumin << " err " << tot_lumin_err <<endl;

	//Print all this info to file for plotting
	//output file txt file

	ofstream xout; xout.open(Form("./yield_output/xbj/%s_kin%s.dat",tgt.c_str(),kin.c_str()));
	xout << "Xbj\t"<<"Ne\t"<<"Yield\t"<<"Error\n";
	ofstream thout; thout.open(Form("./yield_output/theta/%s_kin%s.dat",tgt.c_str(),kin.c_str()));
	thout << "theta\t"<<"Ne\t"<<"Yield\t"<<"Error\n";
	total_lumin=1.0;
	tot_lumin_err=0.0;
for(int i=0;i<bins;i++)
	{
	//	theta_yield[i] /= total_lumin;
	//	Xbj_yield[i] /= total_lumin;

		double total_error_th = theta_yield[i]*sqrt( pow(theta_yield_err[i]/(theta_yield[i]*1.0),2));// + pow(tot_lumin_err/total_lumin  ,2) );
		double total_error_x= Xbj_yield[i]*sqrt( pow(Xbj_yield_err[i]/(Xbj_yield[i]*1.0),2));// + pow(tot_lumin_err/total_lumin  ,2) );

		thout << theta_tot[i]/theta_raw_e[i]<<"\t"<<theta_raw_e[i]<<"\t"<<theta_yield[i]/(num_G_runs*1.0)<<"\t"<<total_error_th/(num_G_runs*1.0)<<"\n";
		xout << xbj_tot[i]/xbj_raw_e[i]<<"\t"<<xbj_raw_e[i]<<"\t"<<Xbj_yield[i]/(num_G_runs*1.0)<<"\t"<<total_error_x/(num_G_runs*1.0)<<"\n";
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
*/
