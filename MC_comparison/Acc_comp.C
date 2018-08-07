#include "../headers/inc1.h"
#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
////////////////////////////////////
// Sript for comparing acceptance varibles between MC and data
// Run with options -r list of runs, to look at a list of runs
// debug has levels, 0 off, 1 basic ....



void Acc_comp(string options = "", int rescaling=1, int tarid=0, int debug=1)
{
  	gStyle->SetPadGridX(kTRUE);
  	gStyle->SetPadGridY(kTRUE);
	gStyle->SetOptStat(0);
	std::size_t it; //Used for find

	//Universal defs
	vector <int> runs;
	string version="";


	



	//Comparing run numbers
	int run[2]={0};
	
	it=options.find("-r");
	if( it != string::npos ){
		//The -r(run) option has been used
		char delim=options[it+2];
		string substring = options.substr(it+3);
		runs = parse_int(substring,delim);	
	}
	//Comparing by kin with tgt
	//
	//
	//
	//


	//Defs with size of number of runs
	TCanvas *C_runs[3][runs.size()];
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
	vector<string> hist_names = { "dp", "ytar", "xfoc", "yfoc", "xpfoc", "ypfoc" };
	vector<string> unit = { "dp/p" , "ytar(cm)" , "xfoc" ,"Yfoc", "XP(dth/th)","YP(dph/ph)"};
	vector<string> det_name = {"ex%s.dp","%s.tr.tg_y*100","%s.tr.x","%s.tr.y","%s.tr.th","%s.tr.ph"};
	vector<string> mcv_name = {"delta/100.0", "ytar", "xfoc/100.0", "yfoc/100.0", "xpfoc", "ypfoc"};
	vector< vector<double> > histbininfo = { {20,-0.07,0.07},{20,-10,10},{25,-1.0,1.0},
						{25,-0.05,0.05},{20,-.2,0.2},{25,-0.05,0.05}};
	vector<string> hist_titles = {"Dp", "Y Target", "X focal plane", "Y focal plane", "Xp(dtheta/theta) focal", "Yp(dphi/phi) focal"};
						
	TH1F *hist[2][hist_names.size()][runs.size()];

	for(unsigned int i=0;i<runs.size();i++)
	{
		//Load of the tree, if it does not exist exit
		int run = runs[i];
		if(debug==1){cout <<"Looking at run " << run <<"\n";}
		dataTree[i] = LoadRun(run);
		if(dataTree[i]==nullptr){ if(debug){cout<<"No data";}exit(1);}	
		mcTree[i]=LoadMC(run,tarid);
		if(mcTree[i]==nullptr){ if(debug){cout<<"No MC";}exit(1);}	

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
			}
		////////////////////////////////////	
		C_runs[0][i] = new TCanvas(Form("C_0%d",i),Form("Canvas 0 for run %d",run));
		C_runs[0][i]->Divide(0,2); //Divid into 2 long pads
		C_runs[1][i] = new TCanvas(Form("C_1%d",i),Form("Canvas 1 for run %d",run));
		C_runs[2][i] = new TCanvas(Form("C_2%d",i),Form("Canvas 2 for run %d",run));
		C_runs[1][i]->Divide(0,2); //Divid into 2 long pads
		C_runs[2][i]->Divide(0,2); //Divid into 2 long pads
	
		//histograms from loop
		for( int hnum=0;hnum<hist_names.size();hnum++)
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
			TCut data_cut = electron_cut_L+acc_cut_L;
			string mc_cut = "(fabs(ypfoc)<=0.05&&abs(xpfoc)<=0.05&&fabs(delta)<0.05)";

			cout << data_cut <<endl;	
			dataTree[i]->Draw(Form("%s",det_draw.c_str()),data_cut);
			if(debug){
				cout<< det_draw <<"\t";
				cout<< mc_draw <<"\n";
			}
			mcTree[i]->Draw(Form("%s",mc_draw.c_str()),Form("%s/yield",mc_cut.c_str()),"same");

			double Ierro=0;
			double integral = hist[0][hnum][i]->IntegralAndError(0,histbininfo[hnum][0]-1,Ierro,"");

			double Ierromc=0;
			double integralmc = hist[1][hnum][i]->IntegralAndError(0,histbininfo[hnum][0]-1,Ierromc,"");
			double  norm_fact_data=hist[0][hnum][i]->GetBinContent((int)floor(histbininfo[hnum][0]/2.0));
			double  norm_fact_mc  =hist[1][hnum][i]->GetBinContent((int)floor(histbininfo[hnum][0]/2.0));	
			double ratio = norm_fact_data/norm_fact_mc;
			cout << integral<<" "<< Ierro<< " "<< integralmc<< " "<< Ierromc<<"\n";	
			if(debug) cout << norm_fact_data<<"  " << norm_fact_mc << " " << ratio <<endl;
			if(rescaling){
				hist[0][hnum][i]->Scale(norm_fact_mc);
				hist[1][hnum][i]->Scale(norm_fact_data);
			}
			if(rescaling==2){
				hist[0][hnum][i]->Scale(1/(integral*1.0));
				hist[1][hnum][i]->Scale(1/integralmc);
			}
			hist[0][hnum][i]->GetXaxis()->SetTitle(Form("%s",unit[hnum].c_str()));
			if(pad==1){
				leg[i] = new TLegend(0.8,0.7,0.9,1);
				leg[i]->AddEntry(hist[0][hnum][i], "Data" , "lp");
				leg[i]->AddEntry(hist[1][hnum][i], "MC" , "lp");
				leg[i]->Draw("same");
			}
		////////////////////////////////////////////dp
		}//end of loop for histgrams!!



/*

		//DP			
		h_dp[0][i] = new TH1F(Form("Hdata_dp_%d",run),Form("DP from data run %d(%s)",run,tgt_info.name.Data()),25,-0.07,0.07);
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

