#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"
////////////////////////////////////
// Sript for comparing phys varibles between MC and data
// Run with options -r list of runs, to look at a list of runs
// debug has levels, 0 off, 1 basic ....



void Acc_comp(string options = "", int tarid=0,int only=0, int debug=1)
{

	if(debug){
		cout<< "Target ID " << tarid<<endl;
		cout<< "Only plot(0=all)  " <<weighter1<<endl;
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
	}		
			
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
	TLegend *leg[runs.size()];	

	
	
	string hist_pre[2] = {"data", "mc"};
	vector<string> hist_names = { "Ep", "th", "Q2", "W", "Xbj"};
	vector<string> unit = { "Eprime(Gev)", "Theta(degree)", "Q squared(GeV^2)" ,"W", "Xbj"};
	vector<string> det_name = {"%s.gold.p","EK%sx.angle*180/3.1415900","EK%sx.Q2","sqrt(EK%sx.W2","EK%sx.x_bj"};
	vector<string> mcv_name = {"3.1 + 3.1*(delta/100.0)", "th_spec", "q2", "sqrt(w2)", "xbj"};
	vector< vector<double> > histbininfo = { {20,-0.07,0.07},{80,-8,8},{25,-1.0,1.0},
	{25,-0.05,0.05},{20,-.2,0.2},{25,-0.05,0.05},{20,-0.08,0.08},{20,-0.08,0.08},{30,-15,15}};
	vector<string> hist_titles = {"Dp ", "Y Target", "X focal plane", "Y focal plane", "Xp(dtheta/theta) focal", "Yp(dphi/phi) focal","Xp/theta tar","YP/phi tar","Ztar"};
						
	TH1F *hist[2][hist_names.size()][runs.size()];
	//TH2F *hist2[2][2dhist_names.size()][runs.size()];

	TH1F *RP[hist_names.size()][runs.size()];

	//Information that might be used for both 1d and 2d histos
	double lumin=1.0;
	TCut data_cut="1";
	string mc_cut="1";
	string data_w = "1.0";
	string weight ="1.0";
	double lumin_noncharge=1.0;
	double mc_lumin=1.0;

	for(unsigned int i=0;i<runs.size();i++)
	{
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
                int debug_flag=0; if(debug){debug_flag=1;}
                vector<double>luminosity_run={2,0.0};
                luminosity_run=Calc_lum(run,debug_flag);
		lumin=luminosity_run[0];
		double Q_e = runinfo.charge/(Qe*1e6);	
		lumin_noncharge = lumin*Q_e;
		mc_lumin = lumin_noncharge;//*0.00022;

		cout << mc_lumin << " " << lumin << " " << lumin/mc_lumin <<endl;

		////////////////////////////////////	
		C_runs[0][i] = new TCanvas(Form("C_0%d",i),Form("Canvas 0 for run %d",run),i*200,0,700,500);
		C_runs[0][i]->Divide(0,2); //Divid into 2 long pads
		C_runs[1][i] = new TCanvas(Form("C_1%d",i),Form("Canvas 1 for run %d",run),i*200,100,700,500);
		C_runs[2][i] = new TCanvas(Form("C_2%d",i),Form("Canvas 2 for run %d",run),i*200,200,700,500);
		C_runs[1][i]->Divide(0,2); //Divid into 2 long pads
		C_runs[2][i]->Divide(0,2); //Divid into 2 long pads
	
		C_runs[3][i] = new TCanvas(Form("C_3%d",i),Form("Canvas 3 for run %d",run),i*200,300,700,500);
		C_runs[3][i]->Divide(0,2); //Divid into 2 long pads
		C_runs[4][i] = new TCanvas(Form("C_4%d",i),Form("Canvas 4 for run %d",run),i*200,400,700,500);
		C_runs[4][i]->Divide(0,2); //Divid into 2 long pads

		C_ratio[i] = new TCanvas(Form("C_r%d",i),Form("Canvas for ratio run %d",run),i*200,500,700,500);

		C_ratio[i]->Divide(2,ceil(hist_names.size()/2.0));

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
			hist[0][hnum][i]->SetMarkerSize(2);
			hist[0][hnum][i]->SetMarkerStyle(31);
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
			data_cut = electron_cut_L+dp_cut_L+th_cut_L+ph_cut_L+z_cut_L;//acc_cut_L+track_L;
			mc_cut = "(fabs(yptar)<=0.05&&fabs(xptar)<=0.04&&fabs(ztar)<=9.0&&fabs(delta)<4)";
			
			if(debug>=2){cout << data_cut <<endl;	
				cout <<   mc_cut << endl;}
			if(weighter==1) weight=Form("1.0/born*(1/%f)",mc_lumin);
			else if(weighter==2) weight=Form("1.0*yield/%f",nop);
			else if(weighter==3) weight=Form("(1.0*(born)/%f*12.0)",lumin);

			if(weighter1==1)data_w=Form("1.0/%f",lumin);
			if(weighter1==2)data_w=Form("1.0/%f",runinfo.charge);

			dataTree[i]->Draw(Form("%s",det_draw.c_str()),data_cut*Form("%s",data_w.c_str()), "E3");
			if(debug){
				cout<< det_draw <<"\t";
				cout<< data_w  <<"\n";
			}
			int mod=1;
			if(RI.target=="Carbon")if(hnum==1)mod=-1;
			mcTree[i]->Draw(Form("%d*%s",mod,mc_draw.c_str()),Form("%s*%s",mc_cut.c_str(),weight.c_str()),"same");

if(debug)cout << Form("%s",det_draw.c_str())<<" "<<data_cut <<Form("*%s",data_w.c_str()) <<endl;

if(debug)cout << Form("%s",mc_draw.c_str())<<" "<<Form("%s*%s",mc_cut.c_str(),weight.c_str()) <<endl;

			double Ierro=0;
			double integral = hist[0][hnum][i]->IntegralAndError(0,histbininfo[hnum][0]-1,Ierro,"");

			double Ierromc=0;
			double integralmc = hist[1][hnum][i]->IntegralAndError(0,histbininfo[hnum][0]-1,Ierromc,"");
			double  norm_fact_data=hist[0][hnum][i]->GetBinContent((int)floor(histbininfo[hnum][0]/2.0));
			double  norm_fact_mc  =hist[1][hnum][i]->GetBinContent((int)floor(histbininfo[hnum][0]/2.0));	
			double ratio = norm_fact_data/norm_fact_mc;

if(debug)cout<< 1/(sqrt(hist[0][hnum][i]->GetEntries())*1.0)<<" " << integral<<" "<< Ierro<< " "<< integralmc<< " "<< Ierromc<<" "<< integral/integralmc<<"\n";	
if(debug) cout << norm_fact_data<<"  " << norm_fact_mc << " " << ratio <<endl;

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
			hist[0][hnum][i]->SetTitle(Form("%s %s(%d)",hist_titles[hnum].c_str(),RI.target.Data(),run));			


			if(pad==1){
				leg[i] = new TLegend(0.8,0.7,0.9,1);
				leg[i]->AddEntry(hist[0][hnum][i], "Data" , "lp");
				leg[i]->AddEntry(hist[1][hnum][i], "MC" , "lp");
				leg[i]->Draw("same");
			}
			C_ratio[i]->cd(1+hnum);
			RP[i][hnum] = (TH1F*)hist[0][hnum][i]->Clone();
			RP[i][hnum]->Divide(hist[1][hnum][i]);
 	                RP[i][hnum]->GetYaxis()->SetRangeUser(0.85,1.15);
			RP[i][hnum]->Draw();

			
			
		
			////////////////////////////////////////////dp
			}//end of loop for histgrams!!
			//resize ytar
			double ytar_max=hist[0][1][i]->GetBinCenter(hist[0][1][i]->GetMaximumBin());
			double spread=hist[0][1][i]->GetRMS();
	//		cout << ytar_max<<" "<< hist[0][1][i]->GetRMS() <<endl;
	//		hist[0][1][i]->GetXaxis()->SetRangeUser(-(abs(ytar_max)+spread*5.5),abs(ytar_max)+spread*5.5);
			if(RI.target=="Carbon" || RI.target=="Carbon Hole"){
				hist[0][1][i]->GetXaxis()->SetRangeUser(-2,2);}
		
			//Add some 2D histograms
			C_d2[i]=new TCanvas(Form("Canvas %d",run),Form("2d hists for run -> %d",run),i*200,400,700,500);
			C_d2[i]->Divide(2,1);
			C_d2[i]->cd(1);

			TH2F *data_phth = new TH2F(Form("h2_data_phth_%d",run),"Data ph:th",40,-0.2,0.2,40,-.05,.05);
//			dataTree[i]->Draw("L.tr.ph:L.tr.th>>h2_data_phth",data_cut*Form("%s",data_w.c_str()),"colz");
			data_phth->GetXaxis()->SetTitle("dth");	
			data_phth->GetYaxis()->SetTitle("dph");
			C_d2[i]->cd(2);
			TH2F *mc_phth = new TH2F(Form("h2_mc_phth_%d",run),"MC ph:th",40,-0.2,0.2,40,-.05,.05);
//			mcTree[i]->Draw("ypfoc:xpfoc>>h2_mc_phth",Form("%s/%s",mc_cut.c_str(),weight.c_str()),"colz");	
			data_phth->SetTitle(Form("ypfoc:xpfoc run%d",run));
			mc_phth->GetXaxis()->SetTitle("dth");	
			mc_phth->GetYaxis()->SetTitle("dph");

			C_runs[0][i]->Print(Form("images/dp_ytar_%d.png",run));
			C_runs[1][i]->Print(Form("images/x_y_foc_%d.png",run));
			C_runs[2][i]->Print(Form("images/xp_yp_foc_%d.png",run));
			C_runs[3][i]->Print(Form("images/xp_yp_tar_%d.png",run));
			C_runs[4][i]->Print(Form("images/ztar_%d.png",run));
			
			C_d2[i]->Print(Form("images/xpyp_foc_%d.png",run));

			if(only>0){
				if(only!=1) delete C_runs[0][i];
				if(only!=2) delete C_runs[1][i];
				if(only!=3) delete C_runs[2][i];
				if(only!=4) delete C_runs[3][i];
				if(only!=5) delete C_runs[4][i];
				if(only!=5) delete C_d2[i];

			}

	/*

			//DP			
			
			h_dp[1][i] = new TH1F(Form("Hmc_dp_%d",run),  Form("DP from mc run %d",run),25,-0.07,0.07);
			//ytar
			h_ytar[0][i] = new TH1F(Form("Hdata_ytar_%d",run),Form("Y target from data run %d(%s)",run,tgt_info.name.Data()),20,-10,10);
			h_ytar[1][i] = new TH1F(Form("Hmc_ytar_%d",run),  Form("Y target from mc run %d",run),20,-10,10);
			//xfoc
			h_xfoc[0][i] = new TH1F(Form("Hdata_xfoc_%d",run),Form("x focal plane from data run %d",run),20,-1,1);
			h_xfoc[1][i] = new TH1F(Form("Hmc_xfoc_%d",run),Form("x focal plane from mc run %d",run),20,-1,1);
			//need to dived xfoc/100 unit correct?
			//yfoc
			h_yfoc[0][i] = new TH1F(Form("Hdata_yfoc_%d",run),Form("y focal plane from data run %d",run),20,-0.05,0.05);
			h_yfoc[1][i] = new TH1F(Form("Hmc_yfoc_%d",run),Form("y focal plane from mc run %d",run),20,-0.05,0.05);
			//Need to dived yfoc/100
			//xpfoc
			h_xpfoc[0][i] = new TH1F(Form("Hdata_xpfoc_%d",run),Form("xp(theta) focal plane from data run %d",run),20,-0.2,0.2);
			h_xpfoc[1][i] = new TH1F(Form("Hmc_xpfoc_%d",run),Form("xp(theta) focal plane from mc run %d",run),20,-0.2,0.2);
			//ypfoc
			h_ypfoc[0][i] = new TH1F(Form("Hdata_ypfoc_%d",run),Form("yp(phi) focal plane from data run %d",run),20,-0.05,0.05);
			h_ypfoc[1][i] = new TH1F(Form("Hmc_ypfoc_%d",run),Form("yp(phi) focal plane from mc run %d",run),20,-0.05,0.05);
			//divied by 100
			

			// Delta p
			C_runs[0][i]->cd(1);
			h_dp[0][i]->SetLineColor(2);
			h_dp[0][i]->SetMarkerColor(2);
			h_dp[0][i]->SetMarkerSize(2);
			h_dp[0][i]->SetMarkerStyle(31);
			h_dp[0][i]->SetFillColor(2);
			h_dp[0][i]->SetFillStyle(3352);
			h_dp[1][i]->SetLineColor(4);
			h_dp[1][i]->SetMarkerColor(4);
			h_dp[1][i]->SetMarkerStyle(33);
			h_dp[1][i]->SetFillColor(4);
			h_dp[1][i]->SetFillStyle(3354);
			dataTree[i]->Draw(Form("ex%s.dp>>Hdata_dp_%d",coda.arm.Data(),run),electron_cut_L&&z_cut_L);
			mcTree[i]->Draw(Form("delta/100>>Hmc_dp_%d",run),"(fabs(ztar)<10.0)/yield","same");
			
			int norm_fact_data=h_dp[0][i]->GetBinContent(13);//h_dp[0][i]->GetMaximumBin());
			int norm_fact_mc  =h_dp[1][i]->GetBinContent(13);//h_dp[0][i]->GetMaximumBin());
			if(rescaling){
	cout <<"dp data_norm " <<  norm_fact_data << " mc_norm " << norm_fact_mc << " "<< h_dp[0][i]->GetMaximumBin() << " "<< h_dp[1][i]->GetMaximumBin() << endl;
				h_dp[0][i]->Scale(norm_fact_mc);
				h_dp[1][i]->Scale(norm_fact_data);
			}
			h_dp[0][i]->GetXaxis()->SetTitle("dp/p");
			//legend
			h_ytar[0][i]->GetXaxis()->SetTitle("ytar(cm)");
			leg[i] = new TLegend(0.8,0.7,0.9,1);
			leg[i]->AddEntry(h_dp[0][i], "Data" , "lp");
			leg[i]->AddEntry(h_dp[1][i], "MC" , "lp");
			leg[i]->Draw("same");
			////////////////////////////////////////////dp
			// ytar 
			C_runs[0][i]->cd(2);
			dataTree[i]->Draw(Form("%s.tr.tg_y*100>>Hdata_ytar_%d",coda.arm.Data(),run),electron_cut_L);
			mcTree[i]->Draw(Form("1*ytar>>Hmc_ytar_%d",run),"(fabs(ytar)<10.0)/yield","same");
			h_ytar[0][i]->SetLineColor(2);
			h_ytar[0][i]->SetMarkerColor(2);
			h_ytar[0][i]->SetMarkerStyle(31);
			h_ytar[0][i]->SetMarkerSize(2);
			h_ytar[0][i]->SetFillColor(2);
			h_ytar[0][i]->SetFillStyle(3352);
			h_ytar[1][i]->SetLineColor(4);
			h_ytar[1][i]->SetMarkerColor(4);
			h_ytar[1][i]->SetMarkerStyle(33);
			h_ytar[1][i]->SetFillColor(4);
			h_ytar[1][i]->SetFillStyle(3354);
			
			
			norm_fact_data=h_ytar[0][i]->GetBinContent(10);//h_ytar[0][i]->GetMaximumBin());
			norm_fact_mc  =h_ytar[1][i]->GetBinContent(10);//h_ytar[1][i]->GetMaximumBin());

			double norm_fact = norm_fact_mc/(norm_fact_data*1.0);

	cout <<"ytar data_norm " <<  norm_fact_data << " mc_norm " << norm_fact_mc << " "<< h_ytar[0][i]->GetMaximumBin() << " "<< h_ytar[1][i]->GetMaximumBin() << endl;
			if(rescaling){
				//h_ytar[0][i]->Scale(norm_fact_mc);
				h_ytar[1][i]->Scale(1/norm_fact);
			}

			///////////////////////////////////ytar
			// xfoc 
			C_runs[1][i]->cd(1);
			dataTree[i]->Draw(Form("%s.tr.x>>Hdata_xfoc_%d",coda.arm.Data(),run),(electron_cut_L&&z_cut_L));
			mcTree[i]->Draw(Form("xfoc/100>>Hmc_xfoc_%d",run),"(fabs(ztar)<10.0)/yield","same");
			h_xfoc[0][i]->SetLineColor(2);
			h_xfoc[0][i]->SetMarkerColor(2);
			h_xfoc[0][i]->SetMarkerStyle(31);
			h_xfoc[0][i]->SetFillColor(2);
			h_xfoc[0][i]->SetFillStyle(3352);
			h_xfoc[1][i]->SetLineColor(4);
			h_xfoc[1][i]->SetMarkerColor(4);
			h_xfoc[1][i]->SetMarkerStyle(33);
			h_xfoc[1][i]->SetFillColor(4);
			h_xfoc[1][i]->SetFillStyle(3354);
			
			
			norm_fact_data=h_xfoc[0][i]->GetBinContent(h_xfoc[0][i]->GetMaximumBin());
			norm_fact_mc  =h_xfoc[1][i]->GetBinContent(h_xfoc[1][i]->GetMaximumBin());

	cout <<"xfoc data_norm " <<  norm_fact_data << " mc_norm " << norm_fact_mc << " "<< h_xfoc[0][i]->GetMaximumBin() << " "<< h_xfoc[1][i]->GetMaximumBin() << endl;
			if(rescaling){
				h_xfoc[0][i]->Scale(norm_fact_mc);
				h_xfoc[1][i]->Scale(norm_fact_data);
			}
			h_xfoc[0][i]->GetXaxis()->SetTitle("xfoc)");
			leg[i]->Draw("same");
			///////////////////////////////////////////////xfoc
			//yfoc 
			C_runs[1][i]->cd(2);
			dataTree[i]->Draw(Form("%s.tr.y>>Hdata_yfoc_%d",coda.arm.Data(),run),electron_cut_L&&z_cut_L);
			mcTree[i]->Draw(Form("1*yfoc/100>>Hmc_yfoc_%d",run),"(fabs(ztar)<10.0)/yield","same");
			h_yfoc[0][i]->SetLineColor(2);
			h_yfoc[0][i]->SetMarkerColor(2);
			h_yfoc[0][i]->SetMarkerStyle(31);
			h_yfoc[0][i]->SetFillColor(2);
			h_yfoc[0][i]->SetFillStyle(3352);
			h_yfoc[1][i]->SetLineColor(4);
			h_yfoc[1][i]->SetMarkerColor(4);
			h_yfoc[1][i]->SetMarkerStyle(33);
			h_yfoc[1][i]->SetFillColor(4);
			h_yfoc[1][i]->SetFillStyle(3354);
			
			norm_fact_data=h_yfoc[0][i]->GetBinContent(h_yfoc[0][i]->GetMaximumBin());
			norm_fact_mc  =h_yfoc[1][i]->GetBinContent(h_yfoc[1][i]->GetMaximumBin());

			if(rescaling){
				h_yfoc[0][i]->Scale(norm_fact_mc);
				h_yfoc[1][i]->Scale(norm_fact_data);
			}
			h_yfoc[0][i]->GetXaxis()->SetTitle("yfoc");
			/////////////////////////////yfoc
			//xpfoc 
			C_runs[2][i]->cd(1);
			dataTree[i]->Draw(Form("%s.tr.th>>Hdata_xpfoc_%d",coda.arm.Data(),run),electron_cut_L&&z_cut_L);
			mcTree[i]->Draw(Form("xpfoc>>Hmc_xpfoc_%d",run),"(fabs(ztar)<10.0)/yield","same");
			h_xpfoc[0][i]->SetLineColor(2);
			h_xpfoc[0][i]->SetMarkerColor(2);
			h_xpfoc[0][i]->SetMarkerStyle(31);
			h_xpfoc[0][i]->SetFillColor(2);
			h_xpfoc[0][i]->SetFillStyle(3352);
			h_xpfoc[1][i]->SetLineColor(4);
			h_xpfoc[1][i]->SetMarkerColor(4);
			h_xpfoc[1][i]->SetMarkerStyle(33);
			h_xpfoc[1][i]->SetFillColor(4);
			h_xpfoc[1][i]->SetFillStyle(3354);
			
			norm_fact_data=h_xpfoc[0][i]->GetBinContent(h_xpfoc[0][i]->GetMaximumBin());
			norm_fact_mc  =h_xpfoc[1][i]->GetBinContent(h_xpfoc[1][i]->GetMaximumBin());

			if(rescaling){
				h_xpfoc[0][i]->Scale(norm_fact_mc);
				h_xpfoc[1][i]->Scale(norm_fact_data);
			}
			h_xpfoc[0][i]->GetXaxis()->SetTitle("xpfoc(tr.th)");
			leg[i]->Draw("same");
			/////////////////////////////yfoc
			//pfoc 
			C_runs[2][i]->cd(2);
			dataTree[i]->Draw(Form("%s.tr.ph>>Hdata_ypfoc_%d",coda.arm.Data(),run),electron_cut_L&&z_cut_L);
			int factor=-1;
			mcTree[i]->Draw(Form("%d*ypfoc>>Hmc_ypfoc_%d",factor,run),"(fabs(ztar)<10.0)/yield","same");
			h_ypfoc[0][i]->SetLineColor(2);
			h_ypfoc[0][i]->SetMarkerColor(2);
			h_ypfoc[0][i]->SetMarkerStyle(31);
			h_ypfoc[0][i]->SetFillColor(2);
			h_ypfoc[0][i]->SetFillStyle(3352);
			h_ypfoc[1][i]->SetLineColor(4);
			h_ypfoc[1][i]->SetMarkerColor(4);
			h_ypfoc[1][i]->SetMarkerStyle(33);
			h_ypfoc[1][i]->SetFillColor(4);
			h_ypfoc[1][i]->SetFillStyle(3354);
			
			norm_fact_data=h_ypfoc[0][i]->GetBinContent(h_ypfoc[0][i]->GetMaximumBin());
			norm_fact_mc  =h_ypfoc[1][i]->GetBinContent(h_ypfoc[1][i]->GetMaximumBin());

			if(rescaling){
				h_ypfoc[0][i]->Scale(norm_fact_mc);
				h_ypfoc[1][i]->Scale(norm_fact_data);
			}
			h_ypfoc[0][i]->GetXaxis()->SetTitle("ypfoc(tr.ph)");
			/////////////////////////////yfoc
			
	*/

			

	 
		
		}//end of loop over runs




	}

