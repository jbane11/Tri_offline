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

void MCyield_test(string tgt ="", string kin="",string suf ="1", int bins=50, int clean=0, int model=0, int debug=3)
{
	//setcut();
	//int clean =0;
	if(kin=="" || tgt ==""){
		cout << "Please enter the  kin and tgt you would like to use" <<"\n";
		cin >> kin;
		cin >> tgt;
		}
		int tarid =0;
		int onerun=0;
//	double kin_angle =GetKinAngle
	double p0=3.1;
	if(kin=="16")p0=2.9;


	double g_bins=0;
	vector<int>missing_runs;

	vector<int>runlist2;
	vector<int>runlist1;

	//open up the run list file
	string runl_name = runl_dir+tgt+"_kin"+kin+".dat";
	kin_file K_File;
	K_File.set_file(runl_name,debug);
	if(K_File.run_file_status==0) {}// exit(1);}
	runlist1=K_File.run_vect;
	//////////////////////////////////
	/// SQL to get run list
	vector<RunList> RList;
	RList = SQL_Kin_Target_RL(kin,tgt);
	runlist2 =SQL_Kin_Target(kin,tgt,suf);

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

	int num_G_runs=0;

	//A functionk to get radcor for this kin...
	//this we mostly likly be a function of eprime and theta
	double rad_cor=1;
	////

	double avg_bin=0;
	//Number of bins and step per kin
	int nobs=bins;
	vector<double> xbj_base(bins,0.0);
	vector<double> xbj_center(bins,0.0);
	vector<double> xbj_step(bins,0.0);
	vector<double> theta_base(bins,0.0);
	vector<double> theta_center(bins,0.0);
	vector<double> theta_step(bins,0.0);
	vector<double> e_base(bins,0.0);
	vector<double> e_center(bins,0.0);
	vector<double> ep_step(bins,0.0);
	double th_step=0.0;
	double x_step=0.0;
	double e_step=0.0;
	double cmom = 3.1;
	double emin=tg_dp_L_min*100;
	double emax=tg_dp_L*100;
	if( kin.find("16") != std::string::npos){cmom=2.9;}

	if(nobs>=int(varible_xbase.size()))
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

////////////////
	vector<int>     theta_ele(nobs,0);      //Number of electrons in theta bin
  vector<int>     xbj_ele(nobs,0);        //Number of electrons in x bin
  vector<double>  theta_yield(nobs,0.0);  //N of es corrected with DT, density....
  vector<double>  xbj_yield(nobs,0.0);    //N of es corrected with DT, density....
  vector<double>  xbj_total(nobs,0.0);    //use to find the bin center
  vector<double>  Q_total(nobs,0.0);    //use to find the bin center
  vector<double>  theta_total(nobs,0.0);
  vector<double>  Ep_total(nobs,0.0);

	vector<int> 	e_ele(nobs,0);	//Number of electrons in e bin
	vector<double>	e_yield(nobs,0.0);	//N of es corrected with DT, density....
	vector<double>  e_total(nobs,0.0);    //use to find the bin center
	vector<double> e_y_error(nobs,0.0);

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
		AnalysisInfo AI = GetAnalysisInfo(run);
		if(AI.status==-1)continue;

		if(clean==0){
			if(tarid==0) T=LoadMC(run,model);
			else{ T= LoadMC(run,model,tarid);}
		}
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
		newfile = isnewer( Form("/run/media/jbane/Slim/model%d/mc%d.root",model,run));
		cout<<"Is the mc root file newish : " << newfile <<endl;
		if(T == nullptr || !newfile || clean){
			gSystem->Exec(Form("rm -vf ./stdout/%d.txt",run));
	    gSystem->Exec(Form("cd %s",MCdir.c_str()));
			cout<< Form("Making MC file with %s %d %d ",tgt.c_str(),intkin,run)<<endl;
	    gSystem->Exec(Form("sh %s/T2_MC %s %d %d %d %d %d >> %s/stdout/%d.txt", MCdir.c_str(), tgt.c_str(), intkin, run, tarid_t, model,1, curdir.c_str(),  run));
			//gSystem->Exec(Form("%s/T2_MC %s %d %d %d", MCdir.c_str(), tgt.c_str(), intkin, run, tarid_t));
	    gSystem->Exec(Form("cd %s",curdir.c_str()));
			if(tarid==0) T=LoadMC(run,model);
			else{ T= LoadMC(run,tarid);}
		}

    if(T==nullptr){ continue;  }
		if(T==nullptr && num_G_runs >5)continue;

    int totn = T->GetEntries();

		Run_lumin = Calc_lum(run,1);
		if(debug) cout << "Lumins :" << Run_lumin[0] <<"\n";

		CODASetting coda = GetCODASetting(run);

		double phase_space = 24.799;

		//get gen events from root file.
		float ngenf;
		T->SetBranchAddress("n_trials",&ngenf);
		T->GetEntry(2);
		int ngen = int(ngenf);
		T->ResetBranchAddresses();
		if(debug) cout <<"Generated " << ngen << " event\n";
		//Shorten up the tree to only look at good electron events;
		TCut total_cut;
		if(coda.arm=="L"){
		//total_cut = Form("(w2>%.3f&&fabs(yptar)<=%.3f&&fabs(xptar)<=%.3f&&fabs(ztar)<=%.3f&&fabs(delta)<=%.3f)", wsqr, tg_ph_L, tg_th_L, tg_vz_L*100,  tg_dp_L*100.0 ) ;
		total_cut = total_mc_cut + Form("yield==yield && yield>0 && w2>%.3f",wsqr);

				//total_cut+=mc_ap_L;
			}
		else{
			total_cut = Form("(w2>%.3f&&fabs(yptar)<=%.3f&&fabs(xptar)<=%.3f&&fabs(ztar)<=%.3f&&fabs(delta)<=%.3f)", wsqr, tg_ph_R, tg_th_R, tg_vz_R*100, tg_dp_R*100.0 ) ;	}
		T->Draw(">>GoodEs",total_cut);
		gDirectory->GetObject("GoodEs",GoodEs);
		T->SetEventList(GoodEs);
		cout << total_cut<<endl;
	//	TH1F *tmp_h; T->Draw(

		float eprime =0;   float theta  =0;
		Float_t x_bj   =0;   float Qsqrd  =0;
		Float_t weight=0;  Float_t born=0;
		Float_t rc =1;
		//Reset branch address from any previous issues
		T->ResetBranchAddresses();
		T->SetBranchStatus("*",1);//Turn on all branches
		///Address those branches
		T->SetBranchAddress("delta",  &eprime);
	  T->SetBranchAddress("th_spec",&theta);
		T->SetBranchAddress("xbj", &x_bj);
		T->SetBranchAddress("q2", &Qsqrd);
		T->SetBranchAddress("yield", &weight);
		T->SetBranchAddress("born", &born);
		T->SetBranchAddress("rci", &rc);

		TCanvas *C = new TCanvas("C","C");
		T->Draw(Form("xbj>>H(%d,0,1)",nobs));

//Loop through all the events in the Good electron list over event
		int nentries =  GoodEs->GetN();
		if(debug)cout << nentries << " " <<totn <<"\n";
//Loop throught the goof events!
                for(int event=0; event<nentries;event++)
                {
	                T->GetEntry(GoodEs->GetEntry(event)); //Use the GEs list to select good events.
                        theta=theta/rad;// convert radians to degrees for scattering anglei

			//loop through all of the bins, checking both x and theta!
			for(int i=0;i<nobs;i++)
                        {
																if(bins<int(varible_xbase.size()))x_step=xbj_step[i];
                                double xbj_diff   = x_bj-xbj_base[i];
                                double theta_diff= theta-theta_base[i];
																double e_diff= eprime-e_base[i];

																if(abs(e_diff) <e_step && e_diff >0)
																{//This event falls into the ith bin for x
																	e_ele[i]++;
																	e_yield[i]+=1*(born/rc)*phase_space/(ngen*1.0);
																	e_total[i]+=eprime;
																} //end of e bin

                                if(abs(xbj_diff) <x_step && xbj_diff >0)
                                {//This event falls into the ith bin for x
                                        xbj_ele[i]++;
                                        xbj_yield[i]+=1*(born/rc)*phase_space/(ngen*1.0);
                                        xbj_total[i]+=x_bj;
					Q_total[i]+=Qsqrd;
                                }//end of xbin
                                if(abs(theta_diff)<th_step && theta_diff>0)
                                {//this event is in this theta bin
                                        theta_ele[i]++;
                                        theta_yield[i]+=1*(born/rc)*phase_space/(ngen*1.0);
                                        theta_total[i]+=theta;
					Ep_total[i]+=p0*(1+eprime/100.0);
                                }//end of th_bin
                        }//end of bins loop


		}//End of event loop!!

                GoodEs=nullptr;
                T=nullptr;
		num_G_runs++;
		int first_bin=-1;
		int last_bin=-1;
		g_bins=0;
		avg_bin=0;
		for(int k=0;k<nobs;k++){
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
			for(int k=0; k<nobs;k++){
				if(xbj_ele[k]>0){
					cout<<  k<<"  "<<setprecision(5)<<xbj_total[k]/xbj_ele[k]<<"\t"<<xbj_ele[k]<<"\t"<<xbj_yield[k]/(num_G_runs*1.0)<<endl;
					}
				}
			}



		if(debug){if(avg_bin>=85000){cout <<" Got enough data "<<endl;break;}}
	if(onerun) break;
        }//End  of run loop!
	if(num_G_runs<1){
		cout << "No good runs " <<endl;}

	if(debug) cout << "Bin average = "<<avg_bin << " with " <<g_bins <<" good bins. \n";

        vector<double>   xbj_stat_error(nobs,0.0);
        vector<double> theta_stat_error(nobs,0.0);

        vector<double>   xbj_error(nobs,0.0);
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

	cout << "Output file ::"<<Form("./yield_output/%s%dbins/xbj/%s_kin%s.dat\n\n",cuttype.c_str(),bins,tgt.c_str(),kin.c_str());

	ofstream xout; xout.open(Form("./yield_output/%s%dbins/xbj/%s_kin%s.dat",cuttype.c_str(),bins,tgt.c_str(),kin.c_str()));
       	xout << "Xbjc\tXbj\t"<<"Q2\t"<<"Ne\t"<<"Yield\t"<<"Error\n";
  ofstream thout; thout.open(Form("./yield_output/%s%dbins/theta/%s_kin%s.dat",cuttype.c_str(),bins,tgt.c_str(),kin.c_str()));
        thout << "theta\t"<<"E`\t"<<"Ne\t"<<"Yield\t"<<"Error\n";

	ofstream eout; eout.open(Form("./yield_output/%s%dbins/theta/%s_kin%s_e.dat",cuttype.c_str(),bins,tgt.c_str(),kin.c_str()));
  eout <<"E`\t"<<"Ne\t"<<"Yield\t"<<"Error\n";

if(debug)cout << "xbjc \t" << " yield\t" << "   stat eror"<<endl;

        for(int i=0;i<nobs;i++)
        {
					double total_error_x= xbj_yield[i]*xbj_stat_error[i];// + pow(tot_lumin_err/total_lumin  ,2) );
          xout << xbj_center[i]<<"\t"<<xbj_total[i]/xbj_ele[i]<<"\t"<<Q_total[i]/xbj_ele[i]<<"\t"<<xbj_ele[i]<<"\t"<<xbj_yield[i]/(num_G_runs*1.0)<<"\t"<<total_error_x/(num_G_runs*1.0)<<"\n";

					if( i <bins){
						double total_error_e= e_yield[i]* 1/sqrt(e_ele[i]);
						double total_error_th = theta_yield[i]*theta_stat_error[i];// + pow(tot_lumin_err/total_lumin  ,2) );

          	thout << theta_center[i]<<"\t"<<Ep_total[i]/theta_ele[i]<<"\t"<<theta_ele[i]<<"\t"<<theta_yield[i]/(num_G_runs*1.0)<<"\t"<<total_error_th/(num_G_runs*1.0)<<"\n";

						eout << e_center[i] <<"\t"<< e_ele[i] <<"\t"<< e_yield[i] <<"\t"<< total_error_e<<"\n";
						//cout << e_center[i] <<"\t"<< e_ele[i] <<"\t"<< e_yield[i] <<"\t"<< total_error_e<<"\n";

					}

	if(xbj_ele[i]>0) cout << xbj_center[i] <<"\t"<<xbj_yield[i]/(num_G_runs*1.0) <<"\t" <<(total_error_x/(num_G_runs*1.0))/(xbj_yield[i]/(num_G_runs*1.0)) <<endl;

        }

        xout.close();
        thout.close();
				eout.close();

	//exit(1);
}//end of program
