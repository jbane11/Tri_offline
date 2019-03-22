#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"
////////////////////////////////////
// Sript for comparing acceptance varibles between MC and data
// Run with options -r list of runs, to look at a list of runs
// debug has levels, 0 off, 1 basic ....


void Acc_comp(string options = "", int rescaling=0, int tarid=0,int weighter=4, int weighter1=0,int only=0, int debug=1)
{

	vector<int> Hcolor= {1,2,3,4,6,7,8,38,14,2,6,7};
	vector<int> rmarker={21,22,23,36,38,29,33,34,25,40,37,31};

	SetStyles();
	string ver="";
	if(debug){
		cout<< "rescaling " << rescaling<<endl;
		cout<< "Target ID " << tarid<<endl;
		cout<< "Weight MC " << weighter<<endl;
		cout<< "Weight data " <<weighter1<<endl;
	}
  	gStyle->SetPadGridX(kTRUE);
  	gStyle->SetPadGridY(kTRUE);
	gStyle->SetOptStat(0);
	std::size_t it; //Used for find

	//Universal defs
	vector <int> runs;
	string version="";

	//Comparing run numbers
	int run[2]={0};
	//Used for SQL option
	string target="";
	string kin="";
	it=options.find("-r");
	if( it != string::npos ){
		//The -r(run) option has been used
		char delim=options[it+2];
		string substring = options.substr(it+3);
		runs = parse_int(substring,delim);
		if(debug>2)cout <<options<<"  :"<< substring <<":  :"<< delim <<":"<<endl;
		ver="r";
	}
	else if(options.find("-c")!=string::npos ){

		char delim=options[it+4];
		string substring = options.substr(it+5);
		runs = parse_int(substring,delim);
		if(debug>2)cout << options<<"  :"<<substring <<":  :"<< delim <<":"<<endl;
		ver="c";
	}
	else if(options.find("-sql")!=string::npos || options.find("-SQL")!=string::npos){
		it=options.find("-sql");
		if(options.find("-sql")==string::npos)it=options.find("-SQL");
		char delim = options[it+4];
		string substring = options.substr(it+5);
		if(substring.size()>=3){
			it=substring.find(delim);
			target=substring.substr(0,it);
			it=substring.find(delim);
			kin=substring.substr(target.size()+1);
		}
		else{
			cout << "Please enter the target\n";
			cin >>target;
			cout <<"Please enter the kin\n";
			cin >>kin;
			}

		TString T_tgt= Form("%s",target.c_str());
		TString T_kin= Form("%s",kin.c_str());
		vector <RunList> RL = SQL_Kin_Target_RL(T_kin,T_tgt);
		runs=RLtoint(RL);
		ver="sql";
	}

	if(debug>2)cout << "Length of list and version " << runs.size()<<"\t"<< ver<<endl;
	//Comparing by kin with tgt
	//
	//
	//
	//



	//Defs with size of number of runs
	TCanvas *C_runs[5][runs.size()];
	TCanvas *C_ratio[runs.size()];
	TCanvas *C_d2[runs.size()];


	TChain *dataTree[runs.size()];
	TChain *mcTree[runs.size()];
	TH1F *h_dp[2][runs.size()];
	TH1F *h_ytar[2][runs.size()];
	TH1F *h_xfoc[2][runs.size()];
	TH1F *h_yfoc[2][runs.size()];
	TH1F *h_xpfoc[2][runs.size()];
	TH1F *h_ypfoc[2][runs.size()];
	TLegend *leg[runs.size()];



	string hist_pre[2] = {"data", "mc"};
	vector<string> hist_names = { "dp", "ytar", "xfoc", "yfoc", "xpfoc", "ypfoc","xptar","yptar","ztar","xbj"};
	vector<string> unit = { "dp/p" , "ytar(cm)" , "xfoc" ,"Yfoc", "XP(theta r)","YP(phi r)","XP(theta r)", "YP(phi r)","Reaction z(cm)","xbj"};
	vector<string> det_name = {"ex%s.dp","%s.tr.tg_y*100","%s.tr.x","%s.tr.y","%s.tr.th","%s.tr.ph","%s.tr.tg_th","%s.tr.tg_ph","%s.tr.vz*100","EKLx.x_bj"};
  //det_name[1] = "rpl.y*100";
	//det_name[8] = "rpl.z*100";

	vector<string> mcv_name = {"delta/100.0", "-ytar", "xfoc/100.0", "yfoc/100.0", "xpfoc", "ypfoc","-xptar","yptar","ztar","xbj"};
	vector< vector<double> > histbininfo = { {30,-0.06,0.06}, {250,-5,5}, {60,-1.0,1.0}, {60,-0.05,0.05},{40,-.2,0.2},{40,-0.05,0.05},{80,-0.08,0.08},{80,-0.08,0.08},{250,-15,15},{50,0,1}};
	vector<string> hist_titles = {"Dp ", "Y Target", "X focal plane", "Y focal plane", "Xp(dtheta) focal", "Yp(dphi) focal","Xp/theta tar","YP/phi tar","Ztar","Xbj"};

	TH1F *hist[2][hist_names.size()][runs.size()];
	//TH2F *hist2[2][2dhist_names.size()][runs.size()];

	TH1F *RP[hist_names.size()][runs.size()];

	//Information that might be used for both 1d and 2d histos
	double lumin=1.0;
	TCut data_cut="1";
	TCut mc_cut="1";
	string data_w = "1.0";
	string weight ="1.0";
	double lumin_noncharge=1.0;
	double mc_lumin=1.0;

	unsigned int num_of_runs=runs.size();
	if(ver=="c")num_of_runs=1;

	for(unsigned int i=0;i<num_of_runs;i++)
	{
		//Load of the tree, if it does not exist exit
		int run;
		RunInfo RI;
		if(ver=="c"){
			dataTree[i]=new TChain("T");
			mcTree[i]=new TChain("h9040");
			for(unsigned int ij=0; ij<runs.size();ij++){
				run = runs[ij];
				if(debug){cout <<"\n\n\tLooking at adding run " << run <<"\n";}
				RI= GetRunInfo(run);
				if(RI.good_run==0){PrintRunInfo(run);continue;}
				dataTree[i]->Add(LoadRun(run));
				if(dataTree[i]==nullptr){ if(debug){cout<<"No data";}
					if(runs.size()==1){exit(1);}
					else{continue;}}
				mcTree[i]->Add(LoadMC(run,tarid));
				if(mcTree[i]==nullptr){ if(debug){cout<<"No MC";}
					if(runs.size()==1){exit(1);}
					else{continue;}}
				}
			}
		else{
			run = runs[i];
			if(debug){cout <<"\n\n\tLooking at run " << run <<"\n";}
			if( run <1000){RI= GetRunInfo(1207);}
			else{RI = GetRunInfo(run);}
			if(RI.good_run==0){PrintRunInfo(run);}
			dataTree[i] = LoadRun(run);
			if(dataTree[i]==nullptr){ if(debug){cout<<"No data";}
				if(runs.size()==1){exit(1);}
				else{continue;}}
			mcTree[i]=LoadMC(run,tarid);
			if(mcTree[i]==nullptr){ if(debug){cout<<"No MC";}
				if(runs.size()==1){exit(1);}
				else{continue;}}
		}
		//Get run info from the SQL db
		CODASetting coda = GetCODASetting(run);
		TargetInfo tgt_info= GetTargetInfo("",-999,run);
		if(tgt_info.name=="unknown") tgt_info=GetTargetInfo("",-999,1207);
		if(debug){
			cout << "Run and tgt info" <<"\n";
			cout << coda.trigger <<" "<< tgt_info.name <<"\n";
			}
		//Get analysis info
		AnalysisInfo runinfo;
		if(RI.good_run) runinfo= GetAnalysisInfo(run);

		if(debug){
			cout << "Run info" <<"\n";
			cout << runinfo.charge<<"\n";
			cout << "number of data counts\n";
			cout << dataTree[i]->GetEntries()<<"\n";
			cout << "number of MC counts\n";
			cout << mcTree[i]->GetEntries()<<"\n";
			}

		//determine the amount of runs in the merged file
		double nop=1.0;
		TCanvas *cc = new TCanvas("cc","cc");
		cc->cd();
		TH1F *tarid = new TH1F("tid","tarid",200,0,20);
		mcTree[i]->Draw("tarid>>tid","","goff");
		TSpectrum *sp = new TSpectrum(10,5);
		sp->Search(tarid,1);
//		nop = sp->GetNPeaks();
		if(debug) cout<<"number of files merge in the mc run ->" << nop <<"\n";
		tarid=nullptr;
		delete tarid;
		delete cc;
		double Q_e =0.0;
    int debug_flag=0; if(debug){debug_flag=1;}
    vector<double>luminosity_run={2,0.0};
		vector<double> RunC =SQLRunCorrection(run);
		PositronCor PC = GetPosInfo(run);
		string pcstr=getposstringmc(PC);
		string kinn = Form("%s",RI.kinematic.Data());
		string tgtt = Form("%s",RI.target.Data());

		tgtt = shortTgtName(tgtt);

		double ECC = 1 - ECC_Cor(stoi(kinn),tgtt);




		if(run>1200){
    	if(RI.good_run)luminosity_run=Calc_lum(run,debug_flag);
			Q_e = runinfo.charge/(Qe*1e6);
		}
		lumin=luminosity_run[0];

		lumin_noncharge = lumin*Q_e;
		mc_lumin = lumin_noncharge;//*0.00022;

		cout << mc_lumin << " " << lumin << " " << lumin/mc_lumin <<endl;

		////////////////////////////////////
		C_runs[0][i] = new TCanvas(Form("C_0%d",i),Form("Canvas 0 for run %d",run),i*500,0,700,500);
		C_runs[0][i]->Divide(0,2); //Divid into 2 long pads
		C_runs[1][i] = new TCanvas(Form("C_1%d",i),Form("Canvas 1 for run %d",run),i*500,150,700,500);
		C_runs[2][i] = new TCanvas(Form("C_2%d",i),Form("Canvas 2 for run %d",run),i*500,300,700,500);
		C_runs[1][i]->Divide(0,2); //Divid into 2 long pads
		C_runs[2][i]->Divide(0,2); //Divid into 2 long pads

		C_runs[3][i] = new TCanvas(Form("C_3%d",i),Form("Canvas 3 for run %d",run),i*500,450,700,500);
		C_runs[3][i]->Divide(0,2); //Divid into 2 long pads
		C_runs[4][i] = new TCanvas(Form("C_4%d",i),Form("Canvas 4 for run %d",run),i*500,600,700,500);
		C_runs[4][i]->Divide(0,2); //Divid into 2 long pads

		C_ratio[i] = new TCanvas(Form("C_r%d",i),Form("Canvas for ratio run %d",run),i*500,750,700,500);

	//	C_ratio[i]->Divide(2,ceil(hist_names.size()/2.0));

		//histograms from loop
		for( unsigned int hnum=0;hnum<hist_names.size();hnum++)
		{
			int can = (int)floor((hnum/2.0));
			int pad = abs((int)(pow(-1,hnum)+hnum%2)-1)+1;
			C_runs[can][i]->cd(pad);
			hist[0][hnum][i] = new TH1F(Form("H%s_%s_%d",hist_pre[0].c_str(),hist_names[hnum].c_str(),run),
				Form("%s from data run %d(%s)",hist_names[hnum].c_str(),run,tgt_info.name.Data()),
				(int)histbininfo[hnum][0],histbininfo[hnum][1],histbininfo[hnum][2]);
			hist[1][hnum][i] = new TH1F(Form("H%s_%s_%d",hist_pre[1].c_str(),hist_names[hnum].c_str(),run),
				Form("%s from MC run %d(%s)",hist_names[hnum].c_str(),run,tgt_info.name.Data()),
				(int)histbininfo[hnum][0],histbininfo[hnum][1],histbininfo[hnum][2]);

			hist[0][hnum][i]->SetLineColor(2);
			hist[0][hnum][i]->SetMarkerColor(2);
			hist[0][hnum][i]->SetMarkerSize(1.5);
			hist[0][hnum][i]->SetMarkerStyle(33);
			hist[0][hnum][i]->SetFillColor(2);
			hist[0][hnum][i]->SetFillStyle(3352);
			hist[1][hnum][i]->SetLineColor(4);
			hist[1][hnum][i]->SetMarkerColor(4);
			hist[1][hnum][i]->SetMarkerStyle(33);
			hist[1][hnum][i]->SetFillColor(4);
			hist[1][hnum][i]->SetFillStyle(3354);
			string det_str = Form(det_name[hnum].c_str(),coda.arm.Data());

			string det_draw = Form("%s>>H%s_%s_%d",det_str.c_str(),hist_pre[0].c_str(),hist_names[hnum].c_str(),run);

			string mc_draw= Form("%s>>H%s_%s_%d",mcv_name[hnum].c_str(),
			hist_pre[1].c_str(),hist_names[hnum].c_str(),run);
			//needs to be made switchable with arm
			data_cut = L_mara_trig + electron_cut_L+ z_cut_L + dp_cut_L + th_cut_L + ph_cut_L ; //acc_cut_L+track_L;
			mc_cut = total_mc_cut + Form(" yield==yield && yield>0 && w2>%.3f" , wsqr ) ;

			if(debug>=2){cout << data_cut <<endl;
				cout <<   mc_cut << endl;}
			if(weighter==1) weight=Form("1.0/born*(1/%f)",mc_lumin);
			else if(weighter==2) weight=Form("1.0*yield/%f",nop);
			else if(weighter==3) weight=Form("(1.0*(born)/%f*12.0)",lumin);
			else if(weighter==4) weight=Form("(1.0*(yield) *%f*(%f* (1 - %s)) )" ,RunC[0],ECC,pcstr.c_str());


			if(weighter1==1)data_w=Form("1.0/%f",lumin);
			if(weighter1==2)data_w=Form("1.0/%f",runinfo.charge);

			dataTree[i]->Draw(Form("%s",det_draw.c_str()),data_cut*Form("%s",data_w.c_str()), "E");
			if(debug>2){
				cout<< det_draw <<"\t";
				cout<< data_w  <<"\n";
			}
			int mod=1;
			if(RI.target != "Carbon" && hnum==1) mod =-1;
			if(strcmp(RI.target.Data(),"Carbon")==0 || strcmp(RI.target.Data(),"Carbon ")==0|| run==1107)if(hnum==1)mod=-1;
			mcTree[i]->Draw(Form("%d*%s",mod,mc_draw.c_str()),mc_cut*Form("%s",weight.c_str()),"same");

if(debug)cout << Form("%s",det_draw.c_str())<<" "<<data_cut <<Form("*%s",data_w.c_str()) <<endl;

if(debug)cout << Form("%s",mc_draw.c_str())<<" "<<mc_cut<<Form("*%s",weight.c_str()) <<endl;

			double Ierro=0;
			double integral = hist[0][hnum][i]->IntegralAndError(0,histbininfo[hnum][0]-1,Ierro,"");

			double Ierromc=0;
			double integralmc = hist[1][hnum][i]->IntegralAndError(0,histbininfo[hnum][0]-1,Ierromc,"");
			double  norm_fact_data=hist[0][hnum][i]->GetBinContent((int)floor(histbininfo[hnum][0]/2.0));
			double  norm_fact_mc  =hist[1][hnum][i]->GetBinContent((int)floor(histbininfo[hnum][0]/2.0));
			double ratio = norm_fact_data/norm_fact_mc;

if(debug)cout<< 1/(sqrt(hist[0][hnum][i]->GetEntries())*1.0)<<" " << integral<<" "<< integralmc<<"   "<< integral/integralmc<<"\n";
if(debug>2) cout << norm_fact_data<<"  " << norm_fact_mc << " " << ratio <<endl;
double max_h1 = hist[0][hnum][i]->GetBinContent(hist[0][hnum][i]->GetMaximumBin());
double max_h2 = hist[1][hnum][i]->GetBinContent(hist[1][hnum][i]->GetMaximumBin());
if(hnum==8){if(debug)cout << "data " << max_h1 << "  mc "<<max_h2 << "    ratio  " <<max_h1/max_h2 <<endl;}

			if(rescaling==1){
				hist[0][hnum][i]->Scale(norm_fact_mc);
				hist[1][hnum][i]->Scale(norm_fact_data);
			}
			if(rescaling==2){
				hist[0][hnum][i]->Scale(1/(integral*1.0));
				hist[1][hnum][i]->Scale(1/integralmc);
			}
			hist[0][hnum][i]->GetXaxis()->SetTitle(Form("%s",unit[hnum].c_str()));
			hist[0][hnum][i]->GetXaxis()->SetTitleSize(0.07);
			hist[0][hnum][i]->GetXaxis()->SetTitleOffset(-0.5);

			//Rescale the pad to fit both histrograms!
			double maxh0=0.0; double maxh1=0.0;
			maxh0=hist[0][hnum][i]->GetBinContent(hist[0][hnum][i]->GetMaximumBin());
			maxh1=hist[1][hnum][i]->GetBinContent(hist[1][hnum][i]->GetMaximumBin());
			double max=maxh0;
			if(max<maxh1)max= maxh1;
			hist[0][hnum][i]->GetYaxis()->SetRangeUser(0, max*1.1);
			/////////////////////////////////////////////
			hist[0][hnum][i]->SetTitle(Form("%s %s(%d)",hist_titles[hnum].c_str(),tgt_info.name.Data(),run));


			if(pad==1)
			{
				leg[i] = new TLegend(0.8,0.7,0.9,1);
				leg[i]->AddEntry(hist[0][hnum][i], "Data" , "lp");
				leg[i]->AddEntry(hist[1][hnum][i], "MC" , "lp");
				leg[i]->Draw("same");
			}

			RP[i][hnum] = (TH1F*)hist[0][hnum][i]->Clone();
			RP[i][hnum]->Divide(hist[1][hnum][i]);
 	                RP[i][hnum]->GetYaxis()->SetRangeUser(RP[i][hnum]->GetBinContent(RP[i][hnum]->GetMinimumBin())*0.9,RP[i][hnum]->GetBinContent(RP[i][hnum]->GetMaximumBin())*1.1);
			if(hnum>=0)
			{
				C_ratio[i]->cd();
				RP[i][hnum]->SetLineColor(Hcolor[hnum]);
				RP[i][hnum]->SetMarkerColor(Hcolor[hnum]);
				RP[i][hnum]->SetMarkerStyle(rmarker[hnum]);
				RP[i][hnum]->SetMarkerSize(2);
				if(hnum==0)RP[i][hnum]->Draw();
				RP[i][hnum]->Draw("same");
				//RP[i][hnum]->Fit("pol0","","",-0.035,0.035);

				cout <<"dp ratio mean " << RP[i][hnum]->GetMean(2) <<endl;

				double err[(int)histbininfo[i][0]];
				//loop over dp bins
				for(int j=0; j<histbininfo[i][0];j++)
				{
					int count = hist[0][0][i]->GetBinContent(j);
					err[j]=count*sqrt(1.0/(count*1.0));
	//				err.push_back( sqrt(1/count));
					if(count <=10)err[j]=0;
					cout << err[j]<< "  ";
				}

				hist[0][0][i]->SetError(err);
		  }



			////////////////////////////////////////////dp
			}//end of loop for histgrams!!
			//resize ytar
			double ytar_max=hist[0][1][i]->GetBinCenter(hist[0][1][i]->GetMaximumBin());
			double spread=hist[0][1][i]->GetRMS();
	//		cout << ytar_max<<" "<< hist[0][1][i]->GetRMS() <<endl;
	//		hist[0][1][i]->GetXaxis()->SetRangeUser(-(abs(ytar_max)+spread*5.5),abs(ytar_max)+spread*5.5);
			//if(RI.target=="Carbon" || RI.target=="Carbon Hole"){
			//	hist[0][1][i]->GetXaxis()->SetRangeUser(-2,2);}

			//Add some 2D histograms
			C_d2[i]=new TCanvas(Form("Canvas %d",run),Form("2d hists for run -> %d",run),i*200,400,700,500);
			C_d2[i]->Divide(2,1);
			C_d2[i]->cd(1);

			TH2F *data_phth = new TH2F(Form("h2_data_phth_%d",run),"Data ph:th",40,-0.2,0.2,40,-.05,.05);
			dataTree[i]->Draw(Form("L.tr.ph:L.tr.th>>h2_data_phth_%d",run), electron_cut_L&&dp_cut_L,"colz");
			data_phth->GetXaxis()->SetTitle("dth");
			data_phth->GetYaxis()->SetTitle("dph");
			C_d2[i]->cd(2);
			TH2F *mc_phth = new TH2F(Form("h2_mc_phth_%d",run),"MC ph:th",40,-0.2,0.2,40,-.05,.05);
			mcTree[i]->Draw(Form("ypfoc:xpfoc>>h2_mc_phth_%d",run),Form("1/%s",weight.c_str()),"colz");
			data_phth->SetTitle(Form("ypfoc:xpfoc run%d",run));
			mc_phth->GetXaxis()->SetTitle("dth");
			mc_phth->GetYaxis()->SetTitle("dph");

			C_runs[0][i]->Print(Form("images/dp_ytar_%d.png",run));
			C_runs[1][i]->Print(Form("images/x_y_foc_%d.png",run));
			C_runs[2][i]->Print(Form("images/xp_yp_foc_%d.png",run));
			C_runs[3][i]->Print(Form("images/xp_yp_tar_%d.png",run));
			C_runs[4][i]->Print(Form("images/ztar_%d.png",run));

			C_d2[i]->Print(Form("images/xpyp_foc_%d.png",run));


//			C_d2[i]=new TCanvas(Form("Canvas %d",run),Form("2d hists for run -> %d",run),i*200,400,700,500);
//			C_d2[i]->Divide(2,1);
//			C_d2[i]->cd(1);

			//TH2F *mc_dpth = new TH2F(Form("h2_mc_dpth_%d",run),"MC dp:th",40,-0.06,0.06,40,-.05,.05);
		//TH2F *mc_dpph = new TH2F(Form("h2_mc_dpph_%d",run),"MC dp:ph",40,-0.06,0.06,40,-.2,.2);
			if(only>0){
				if(only!=1 && only!=15 && only!=157) delete C_runs[0][i];
				if(only!=2) delete C_runs[1][i];
				if(only!=3) delete C_runs[2][i];
				if(only!=4) delete C_runs[3][i];
				if(only!=5&& only!=15 && only!=157) delete C_runs[4][i];
				if(only!=6) delete C_d2[i];
				if(only!=7&& only!=157) delete C_ratio[i];

			}



			luminosity_run.clear();
			RunC.clear();


		cout <<"\n\n\n";
		}//end of loop over runs




	}
