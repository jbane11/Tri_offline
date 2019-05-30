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

void CalcYield_test(string tgt ="", string kin="",int bins =50, string suf ="1", int debug=3)
{
	if(kin=="" || tgt ==""){
		cout << "Please enter the  kin and tgt you would like to use" <<"\n";
		cin >> kin;
		cin >> tgt;
		}
		int num_G_runs=0;
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
//	RList = SQL_Kin_Target_RL(kin,tgt,suf);
//	runlist2 = RLtoint(RList);
	runlist2= SQL_Kin_Target(kin,tgt,suf);
//	if(debug){for(unsigned int z=0;z<runlist2.size();z++){cout<<runlist2[z]<<" ";}}
//	if(debug)cout<<"\n";

	int whichRL=2;
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

	//A functionk to get radcor for this kin...
	//this we mostly likly be a function of eprime and theta
	double rad_cor=1;
	////

	//Number of bins and step per kin
	//Number of bins and step per kin
	int nobs=bins;
	vector<double> xbj_base(bins,0.0);
	vector<double> xbj_center(bins,0.0);
	vector<double> xbj_step(bins,0.0);
	vector<double> theta_base(bins,0.0);
	vector<double> theta_center(bins,0.0);
	vector<double> theta_step(bins,0.0);
	vector<double> theta_xbj(bins,0.0);
	vector<double> e_base(bins,0.0);
	vector<double> e_center(bins,0.0);
	vector<double> ep_step(bins,0.0);
	vector<double> ep_xbj(bins,0.0);
	double th_step=0.0;
	double x_step=0.0;
	double e_step=0.0;
	double cmom = 3.1;
	double emin=tg_dp_L_min;
	double emax=tg_dp_L;
	if( kin.find("16") != std::string::npos){cmom=2.9;}

	if(nobs>=varible_xbase.size())
	{
		nobs=bins;

	  th_step = (37.7-15)/(nobs*1.0);
	  x_step  = (1.00-0.00)/(nobs*1.0);
		e_step  = ((emax) - (emin)) /(nobs*1.0);
	  for(int z=0;z<nobs;z++){
	          xbj_base[z]  =0.00+z*x_step;
						xbj_center[z]= (xbj_base[z]) + x_step/2.0;
	          theta_base[z]=15.0+z*th_step;
						theta_center[z]= (theta_base[z]) + th_step/2.0;
						e_base[z]  =(emin)+z*e_step;
						e_center[z]= (e_base[z]) + e_step/2.0;

//						cout << xbj_center[z] <<" "<<xbj_base[z]<<endl;
	  }
	}
	else
	{

		//select the vairble bin size
		//base = start of bin and step will be step size of that bin
		nobs=varible_xbase[bins].size();
		if(nobs <= 0 || nobs >= 25){nobs=bins;}
		xbj_base.resize(nobs,0.0);
		xbj_step.resize(nobs,0.0);
		xbj_center.resize(nobs,0.0);
	  theta_base.resize(nobs,0.0);
		theta_step.resize(nobs,0.0);
		theta_center.resize(nobs,0.0);
		e_base.resize(nobs,0.0);
		ep_step.resize(nobs,0.0);
		e_center.resize(nobs,0.0);

		//varible vectors stored in inc1.h
		xbj_base=varible_xbase[bins];
		xbj_step=varible_xstep[bins];

		th_step = (37.7-15)/(bins*1.0);
		e_step  = ((emax) - (emin)) /(bins*1.0);
		for(int z=0;z<nobs;z++){
			theta_base[z]=15.0+z*th_step;
			theta_center[z]= (theta_base[z]) + th_step/2.0;
			xbj_center[z]= (xbj_base[z]) + xbj_step[z]/2.0;
			e_base[z]  =(emin)+z*e_step;
			e_center[z]= (e_base[z]) + e_step/2.0;
		}
	}

	int OMmod = 0;
	int istenth = 0;

////////////////
	vector<int> 	theta_ele(nobs,0);	//Number of electrons in theta bin
	vector<double>	theta_yield(nobs,0.0);	//N of es corrected with DT, density..
	vector<double>  theta_total(nobs,0.0);
	vector<double>  th_y_error(nobs,0.0);

	vector<int> 	xbj_ele(nobs,0);	//Number of electrons in x bin
	vector<double>	xbj_yield(nobs,0.0);	//N of es corrected with DT, density....
	vector<double>  xbj_total(nobs,0.0);    //use to find the bin center
	vector<double> xbj_y_error(nobs,0.0);

	vector<int> 	e_ele(nobs,0);	//Number of electrons in e bin
	vector<double>	e_yield(nobs,0.0);	//N of es corrected with DT, density....
	vector<double>  e_total(nobs,0.0);    //use to find the bin center
	vector<double> e_y_error(nobs,0.0);

	//one run vectors
	vector<int> 	xbj_ele_run(nobs,0);	//Number of electrons in x bin
	vector<double>  xbj_total_run(nobs,0.0);    //use to find the bin center
	vector<double>	xbj_yield_run(nobs,0.0);	//N es corrected with DT,density..
	vector<double> BGerr_run_x(nobs,0.0);
	vector<double> BGerr_run_th(nobs,0.0);
	vector<double> Yx_err_run(nobs,0.0);
	vector<double> Yth_err_run(nobs,0.0);

///////////////
	int intkin = stoi(kin);
	double RCTEST = RC_factor(intkin,tgt,1,1,1);

////////////////
	double total_lumin=0.0;
	double total_lumin_err=0.0;
	double total_run_correction_err=0.0;


	//Clean up after each run!!
	TChain *T = new TChain; // TChain for root file
	TEventList *GoodEs;	// Only the good electron events.
	vector<double> Run_lumin(2,0.0);
	//////////////////

////////
////////Start the run loop

	for(unsigned int j = 0; j < runlist.size();j++)
	{
		int run = runlist[j]; 	//Get run number
		if(run==1421)continue;
		if(debug) printf("\n\n\tStarting run %d :  %d out of %zu \n",run,j+1, runlist.size());
		AnalysisInfo AI = GetAnalysisInfo(run);
		if(AI.status==-1)continue;

		T=LoadRun(run); 	//Load run using rootalias

		if(T==nullptr){
			vector<int> bruns_v;
			if(debug)cout << "!!!!No root tree!!!!\n";
			ifstream bruns; bruns.open("badruns.csv");
			string bruns_string;
			if(bruns.is_open())getline(bruns,bruns_string);
			if(bruns_string.size()>3)
				bruns_v =parse_csv_int(bruns_string);

			std::vector<int>::iterator it;
		  it = find (bruns_v.begin(), bruns_v.end(), run);
		  if (it == bruns_v.end())
			{
					ofstream badruns; badruns.open("badruns.csv",std::ofstream::app);
					badruns<<run<<",";
			}

			continue;
		}

		//Use SQL to retrieve run info;
		CODASetting coda = GetCODASetting(run);
		RunInfo runinfo  = GetRunInfo(run);
		PositronCor PC   = GetPosInfo(run);
		double 	ECC	 = ECC_Cor(stod(kin),tgt);
		double Eccerr=0.001;
		ECC = (1-ECC); //get % of endcap in nominal.

		double run_correction_err;
		double LT = SQLRunCorrection(run,run_correction_err);


		vector<double> RunC =SQLRunCorrection(run);
		if( RunC[0] == -1.0){cout << "Correction issue!!!!!!!!!!!!!!\n";continue;}

		LT=RunC[0];
		if(debug)cout<<" :::" << LT<<endl;
// LT=1.0;
		run_correction_err =RunC[1];
		total_run_correction_err = sqrt( pow(total_run_correction_err,2)+ pow(run_correction_err*RunC[1],2));


if(debug) cout << "Run cor " << LT<< " err " << run_correction_err<<"\n so far "<<total_run_correction_err<<endl;
if(debug) cout << "ECC   " << ECC <<endl;


		//Calculate the luminosity of this run, sum all the runs up for the total lumin,
		Run_lumin = Calc_lum(run,1);
		if(Run_lumin[0]==0 || Run_lumin[1] != Run_lumin[1])
		{
			if(debug) printf("Skipping bad lumin \n");
			continue;
		}

		total_lumin_err=sqrt(
			pow(total_lumin_err,2) +
			pow(Run_lumin[1]*Run_lumin[0],2)
		);
		total_lumin+=Run_lumin[0];

		double total_lumin_per_err = total_lumin_err/total_lumin;
if(debug) cout << "Lumin : "<<Run_lumin[0]<< "   err  " <<Run_lumin[1] <<"\n Total so far " << total_lumin<<" err% : "<< total_lumin_err/total_lumin<<endl<<endl;

	 	int totn = T->GetEntries();


		//Shorten up the tree to only look at good electron events;
		TCut total_cut;
		if(coda.arm=="L")total_cut = layers_electron_cut_L&&acc_cut_L&&L_dnew&&inv_m_L&&track_L;
		else{total_cut = electron_cut_R&&acc_cut_R&&R_dnew&&inv_m_R&&track_R;}
		T->Draw(">>GoodEs",total_cut);
		gDirectory->GetObject("GoodEs",GoodEs);
		T->SetEventList(GoodEs);
		int nentries =  GoodEs->GetN();
		if(debug) cout <<"Full tree : " << totn << " Cuts : "<< nentries <<"\n";

		//Reset branch address from any previous issues
		T->ResetBranchAddresses();
		T->SetBranchStatus("*",0);//Turn off all branches
		T->SetBranchStatus(Form("%s.tr.tg_dp",coda.arm.Data())	,1);//Turn on eprime
		T->SetBranchStatus(Form("EK%sx.angle",coda.arm.Data())	,1);//scattered angle
		T->SetBranchStatus(Form("EK%sx.x_bj",coda.arm.Data())	,1);//x_bj
		T->SetBranchStatus(Form("EK%sx.Q2",coda.arm.Data())	,1);//Q2

		///Address those branches
		double eprime=0.0;	double theta=0.0;
		double x_bj  =0.0;	double Qsqrd =0.0;
		T->SetBranchAddress(Form("%s.tr.tg_dp",coda.arm.Data()),  &eprime);
	  T->SetBranchAddress(Form("EK%sx.angle",coda.arm.Data()),&theta);
		T->SetBranchAddress(Form("EK%sx.x_bj",coda.arm.Data()), &x_bj);
		T->SetBranchAddress(Form("EK%sx.Q2",coda.arm.Data()),	&Qsqrd);
		///////////////////////

cout <<"Drawing" <<endl;
		TCanvas *C = new TCanvas("C","C");
		T->Draw(Form("EK%sx.x_bj>>H(%d,0,1)",coda.arm.Data(),nobs));



		//Loop throught the good events!
		for(int event=0; event<nentries;event++)
		{
			T->GetEntry(GoodEs->GetEntry(event)); //Use the GEs list to select good events.
			theta=theta/rad;// convert radians to degrees for scattering angle

			//Calculate positron correction from SQL table
			//Need x , the PC class and double pointr
			double PosCor_Error=0.0;
			double PosCor = GetPosCorFactor(x_bj,PC,PosCor_Error);
			//PosCor=0.0;
			//Calculate RC cor from table interpelate between 4 points
			double RC= 1.0;//RC_factor(intkin,tgt,10.6,eprime,theta);
			if(RC>=2 || RC<=0.75|| RC!=RC) cout<<"RC issue: "<< RC<<" " <<eprime << " "<<theta <<endl;
			if(RC!=RC) RC=1;

			if(PosCor>1)
			{
				PosCor=0.0;
				PosCor_Error=0.0;
			}
			if(event==20) cout <<"Pos  " << PosCor << "  RC  " << RC <<endl;
			if(event==120) cout <<"Pos  " << PosCor << "  RC  " << RC <<endl;
			double Event_corrections = 1.0;
			Event_corrections = 1.0/LT;

			//PosCor = e+/e-  -> need (e-)/(e- + e+)
			Event_corrections *= 1.0/(1.0 -  PosCor);
			//add in ECC
			Event_corrections *=1.0/ECC;
			//add in RC
			//Event_corrections *=RC;
			if(Event_corrections ==0 && debug>=4){
				cout << event<< "  ECC:" << ECC << " PC:"<< PosCor <<"  RC:"<< RC <<endl;
			}
			if(Event_corrections!=Event_corrections)
			{
				cout <<event<<" : " << ECC << " "<< RC << " " << PosCor <<endl;
			}

			//loop through all of the bins, checking both x and theta!
			for(int i=0;i<nobs;i++)
			{

				double xbj_diff   = x_bj-xbj_base[i];
				double theta_diff= theta-theta_base[i];
				double e_diff= eprime-e_base[i];
				if(bins<varible_xbase.size())x_step=xbj_step[i];

				if(abs(e_diff) <e_step && e_diff >0)
				{//This event falls into the ith bin for x
					e_ele[i]++;
					e_yield[i]+=1*Event_corrections;
					e_total[i]+=eprime;
					ep_xbj[i]+=x_bj;
				} //end of e bin


				if(abs(xbj_diff) <x_step && xbj_diff >0)
				{//This event falls into the ith bin for x
					xbj_ele[i]++;
					xbj_yield[i]+=1*Event_corrections;
					xbj_total[i]+=x_bj;

					//Get reset at end of run //
					xbj_ele_run[i]++;
					xbj_yield_run[i]+=1*Event_corrections;
					xbj_total_run[i]+=x_bj;
					if(debug==100) cout << x_bj << " "<< i <<"  "<< Event_corrections<<"\n";

					// Combinding error for background corrections_label
					double PC1=PosCor;
					double ECC1=ECC;
					if(PosCor==0)PC1=1;
					if(ECC==1)ECC1=0;
					double BGerr_ev= (ECC*(1-PosCor)) * sqrt(pow(PosCor_Error/PC1,2)  +   pow( Eccerr/(1-ECC1),2) );
					BGerr_run_x[i] =sqrt( pow(BGerr_ev,2) + pow(BGerr_run_x[i],2)  );

				}//end of xbin
				if(abs(theta_diff)<th_step && theta_diff>0)
				{//this event is in this theta bin
					theta_ele[i]++;
					theta_yield[i]+=1*Event_corrections;
					theta_total[i]+=theta;
					theta_xbj[i]+=x_bj;
					// Combinding error for background corrections_label
					double BGerr_ev=0.0;
					double PC1=PosCor;
					double ECC1=ECC;
					if(PosCor==0)PC1=1;
					if(ECC==1)ECC1=0;
					BGerr_ev= (ECC*(1-PosCor)) * sqrt(pow(PosCor_Error/PC1,2)  +   pow( Eccerr/(1-ECC1),2) );
					BGerr_run_th[i] =sqrt( pow(BGerr_ev,2) + pow(BGerr_run_th[i],2)  );

				}//end of th_bin

				double ev_per = event/(nentries*1.0) * 100;
				int OM = floor(log10(nentries));
				int modper = ( (int(ev_per*pow(10,OM))) % int(pow(10,OM)) );
				int OMmod = abs(log10(modper));
				int istenth = ( int(floor(ev_per)) % 10 == 0);
				//cout <<istenth<<" "<< ev_per <<" "<< modper <<" "<< OMmod <<endl;
			if(debug>=4){if(istenth && OMmod==1){if(xbj_ele[i]>0){		cout<<event/(nentries*1.0)<<" "<<i<<"\t : "<<xbj_total[i]/xbj_ele[i] <<"\t " << xbj_ele[i] <<" "<< xbj_yield[i]<<" "<<Event_corrections<<" "<<xbj_yield[i]/xbj_ele[i] <<"\n";}
				if(i==nobs-1)cout<<"\n";
				}}//debug statment for heavy debuging, debug>=4
			}//end of bins loop



		}//End of event loop!!

		//Calculate the err of the yeild for this run
		for(int z=0;z<nobs;z++)
		{		if(xbj_ele[z]>0)
				{	Yx_err_run[z] =sqrt( pow(RunC[1],2) + pow(BGerr_run_x[z]/(xbj_yield[z]*LT),2) ) * xbj_yield_run[z];}
				if(theta_ele[z]>0)
				{
			Yth_err_run[z] =sqrt( pow(RunC[1],2) + pow(BGerr_run_th[z]/(theta_yield[z]*LT),2) ) * theta_yield[z];}
		}


		if(debug>=3){
			cout << " Yx err: ";
			for(int z=0;z<nobs;z++){if(xbj_ele[z]>0){
				cout<< Yx_err_run[z]<<"\t:  "; }}
				cout << "\n BG err: ";
			for(int z=0;z<nobs;z++){if(xbj_ele[z]>0){
				cout<< BGerr_run_x[z]<<"\t:  "; }}
					cout << "\nRun cor err " << RunC[1] <<endl;
			cout<<"\n\n";
		}

		num_G_runs++;
		GoodEs=nullptr;
		T=nullptr;
		if(debug>=2)cout << "END of run\n\n";
		if(debug>=3)
		{
	 		cout << "xbj \t count \t mod. count \t yield"<<endl;
			for(int k=0; k<nobs;k++){
				if(xbj_ele_run[k]>0){
					cout<< k<<"  "<<setprecision(5)<< xbj_total_run[k]/xbj_ele_run[k]<<" \t"<< xbj_ele_run[k]<<"\t"<<xbj_yield_run[k] <<"\t"<<xbj_yield_run[k]/Run_lumin[0]<<endl;
					}
				}
			}//end of debug

			if(debug>=3)cout << "cleaning up run vectors\n\n";
			xbj_ele_run.erase(xbj_ele_run.begin(),xbj_ele_run.end());
			xbj_yield_run.erase(xbj_yield_run.begin(),xbj_yield_run.end());
			xbj_total_run.erase(xbj_total_run.begin(),xbj_total_run.end());
			xbj_ele_run.resize(nobs,0.0);
			xbj_yield_run.resize(nobs,0.0);
			xbj_total_run.resize(nobs,0.0);
			BGerr_run_x.erase(BGerr_run_x.begin(),BGerr_run_x.end());
			BGerr_run_x.resize(nobs,0.0);
			BGerr_run_th.erase(BGerr_run_th.begin(),BGerr_run_th.end());
			BGerr_run_th.resize(nobs,0.0);


			cout<<"::::: " << xbj_ele_run.size()<<" :::::::" <<endl;

			for(int z=0;z<nobs;z++)
			{
				if(xbj_ele[z]>0)
				{
					//the error for the yield cal of all the runs
					xbj_y_error[z] =  sqrt(pow(Yx_err_run[z],2) + pow(xbj_y_error[z],2));
				}
				if(theta_ele[z]>0)
				{
					th_y_error[z] =  sqrt(pow(Yth_err_run[z],2) + pow(th_y_error[z],2));
				}
			}
	}//End  of run loop!


	vector<double>   xbj_stat_error(nobs,0.0);
	vector<double> theta_stat_error(nobs,0.0);

	vector<double>   xbj_error(nobs,0.0);
	vector<double>   xbj_modcount(nobs,0.0);
	vector<double> theta_error(nobs,0.0);



	for(int z=0;z<nobs;z++)
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


	//Print all this info to file for plotting
	ofstream xout; xout.open(Form("./yield_output/%s%dbins/xbj/%s_kin%s.dat",cuttype.c_str(),bins,tgt.c_str(),kin.c_str()));
	xout << "Xbjc\tXbj\t"<<"Ne\t"<<"ModNe\t"<<"Yield\t"<<"Error\txbj\n";
	ofstream thout; thout.open(Form("./yield_output/%s%dbins/theta/%s_kin%s.dat",cuttype.c_str(),bins,tgt.c_str(),kin.c_str()));
	thout << "theta\t"<<"Ne\t"<<"Yield\t"<<"Error\txbj\n";

	ofstream eout; eout.open(Form("./yield_output/%s%dbins/theta/%s_kin%s_e.dat",cuttype.c_str(),bins,tgt.c_str(),kin.c_str()));
  eout <<"E`\t"<<"Ne\t"<<"Yield\t"<<"Error\txbj\n";


if(debug) cout << " RunC Err " << total_run_correction_err <<"  Lumin error " << total_lumin_err/total_lumin <<"\n";


if(debug)cout<<"\n\n\nXbj   counts  "  << "xbj_error " << "  yield" << "   stat error%     percent err"<<endl;

	for(int i=0;i<nobs;i++)
	{
		xbj_modcount[i]=xbj_yield[i];
		theta_yield[i] /= total_lumin;
		xbj_yield[i] /= total_lumin;


		//Calculate the total error for the bin
		//include stat,lumin,.......
		xbj_error[i] =  xbj_yield[i]*   sqrt( pow(xbj_stat_error[i],2)		//stat
					+ pow((total_lumin_err)/total_lumin,2)	//lumin inc(dens)
					+ pow(xbj_y_error[i]/(xbj_yield[i]*total_lumin),2)  );	//LT -run_c

if(xbj_ele[i]>0)cout <<xbj_center[i]<<" "<<xbj_ele[i]<<" "<< xbj_error[i] <<"  "<< xbj_yield[i]<<" "<< xbj_stat_error[i] <<" "<<xbj_error[i]/xbj_yield[i]<< endl;
		//double total_error_th = theta_yield[i]*sqrt( pow(theta_yield_err[i]/(theta_yield[i]*1.0),2) + pow(tot_lumin_err/total_lumin  ,2) );
		//double total_error_x= Xbj_yield[i]*sqrt( pow(Xbj_yield_err[i]/(Xbj_yield[i]*1.0),2) + pow(tot_lumin_err/total_lumin  ,2) );

		xout << xbj_center[i]<<"\t"<< xbj_total[i]/xbj_ele[i]<<"\t"<< xbj_ele[i] <<"\t"<< xbj_modcount[i]<<"\t"<< xbj_yield[i]<<"\t"<< xbj_error[i]<<"\n";
		if( i <bins){
			e_yield[i] /= total_lumin;
			theta_yield[i] /= total_lumin;
			double total_error_e= e_yield[i]* 1/sqrt(e_ele[i]);

			theta_error[i]= theta_yield[i]* sqrt( pow(theta_stat_error[i],2)
						+ pow((total_lumin_err)/total_lumin,2)
						+ pow(th_y_error[i]/(theta_yield[i]*total_lumin),2)  );

			thout << theta_center[i] <<"\t"<<theta_ele[i]<<"\t"<<theta_yield[i]<<"\t"<<theta_error[i] <<"\t"<<theta_xbj[i]/(theta_ele[i]*1.0) <<"\n";

			eout <<e_center[i]<<"\t"<<e_ele[i]<<"\t"<<e_yield[i]<<"\t"<<total_error_e <<"\t"<<ep_xbj[i]/(e_ele[i]*1.0)<<"\n";;
		}
	}

	xout.close();
	thout.close();
	eout.close();




//	exit(1);
}//end of program
