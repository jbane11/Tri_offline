#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"
////////////////////////////////////
// Sript for comparing phys varibles between MC and data
// Run with options -r list of runs, to look at a list of runs
// debug has levels, 0 off, 1 basic ....



void Study_acc(string options = "", string only="", int debug=3)
{
	SetStyles();
	gStyle->SetPadGridX(kTRUE);
	gStyle->SetPadGridY(kTRUE);
	gStyle->SetOptStat(0);
	gStyle->SetPalette(112); //virids 112
	gStyle->SetPaintTextFormat("2.2f");

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
	/////// Run list is made!!!!
	string hist_pre[2] = {"data", "mc"};

	///////////////////////////////////////////////////////////////
  vector <string> hist_names = {"dp_x", "ph_x", "th_x", "theta_x","dp_th","dp_ph","th:ph" };
	vector <string> mc_names  = {"delta/100.0:xbj" ,
															 "yptar:xbj",
															 "xptar:xbj",
														   "(th_spec*%.3f-%.3f):xbj",
														 	 "delta/100.0:xptar",
															 "delta/100.0:yptar",
														 	 "xptar:yptar"};
	vector <string>data_names= {"exL.dp:EKLx.x_bj",
															"L.tr.tg_ph:EKLx.x_bj",
															"L.tr.tg_th:EKLx.x_bj",
														 	"(EKLx.angle*%.3f-%.3f):EKLx.x_bj",
															"exL.dp:L.tr.tg_th",
															"exL.dp:L.tr.tg_ph",
															"L.tr.tg_th:L.tr.tg_ph"
														};
	vector <string> x_axis = {"xbj","xbj","xbj","xbj","dtheta(xptar)","dphi(yptar)","phi(yptar)"};
	vector <string> y_axis = {"delta (dp/p)",
														"Phi (yptar)",
														"Theta (xptar)",
														"dangle(deg)",
														"delta (dp/p)",
														"delta (dp/p)",
													  "dtheta(xptar)",};
  double x_bins=50;
	vector < vector<double>> binstats ={{x_bins,0,1,30,-0.06,0.06},
														 					{x_bins,0,1,40,-.04,0.04},
														 					{x_bins,0,1,40,-0.06,0.06},
																			{x_bins,0,1,40,-2,2 },
																		  {30,-0.06,0.06,40,-0.06,0.06},
																		  {30,-0.06,0.06,40,-0.06,0.06},
																		  {40,-0.06,0.06,40,-0.06,0.06}};
  vector <string> hist_titles ={"dp:xbj", "Ph_tar:xbj","th_tar:xbj","dangle:xbj","dp:thtar","dp:phitar","thtar:phitar"};
	////////////////////////////////////////////////////////////////
  TH2F *mc_hist[hist_names.size()][runs.size()];
	TH2F *data_hist[hist_names.size()][runs.size()];
	TH2F *ratio_hist[hist_names.size()][runs.size()];


	int total_hist = hist_names.size();
	int total_canvases = total_hist;
	//Defs with size of number of runs
	TCanvas *C_runs[total_canvases][runs.size()];
	TCanvas *C_ratios[total_canvases][runs.size()];
	int canvas_sel=0;
	int canvas_div=1;
	TChain *dataTree[runs.size()];
	TChain *mcTree[runs.size()];

	TLegend *leg[runs.size()];

	TCut data_cut="1";
	TCut mc_cut="1";
	string data_w = "1.0";
	string weight ="1.0";


	unsigned int num_of_runs=runs.size();
	for(unsigned int i=0;i<num_of_runs;i++)
	{

		int run = runs[i];
		TargetInfo TI = GetTarget(run);
		auto RI= GetRunInfo(run);
		double central_theta = RI.angle;

		dataTree[i]=new TChain("T");
		mcTree[i]=new TChain("h9040");

		if(debug){cout <<"\n\n\tLooking at run " << run <<"\n";}
		dataTree[i] = LoadRun(run);
		mcTree[i]=LoadMC(run);

		if(dataTree[i] == nullptr)
		 {cout << "Bad data tree" <<endl; exit(1);}
	 	if(mcTree[i] == nullptr)
		 {cout << "Bad mc tree"   <<endl; exit(1);}

		if(debug)
		 {
			cout << "number of data counts\t:\t";
			cout << dataTree[i]->GetEntries()<<"\n";
			cout << "number of MC counts\t:\t";
			cout << mcTree[i]->GetEntries()<<"\n\n";
		 }

		vector<double>luminosity_run={2,0.0};
 		vector<double> RunC =SQLRunCorrection(run);
 		PositronCor PC = GetPosInfo(run);
 		string pcstr=getposstringmc(PC);
 		string kinn = Form("%s",RI.kinematic.Data());
 		string tgtt = Form("%s",RI.target.Data());

 		tgtt = shortTgtName(tgtt);

 		double ECC = 1 - ECC_Cor(stoi(kinn),tgtt);



		 for(unsigned int hnum=0; hnum <hist_names.size(); hnum++)
		 {
		 	C_runs[hnum][i] = new TCanvas(
										Form("C_%d_%s",run,hist_names[hnum].c_str() ),
										Form("C_%d_%s",run, hist_names[hnum].c_str()),
			 									hnum*600,i*600,700,500) ;
			C_runs[hnum][i]->Divide(0,2);
			C_ratios[hnum][i] = new TCanvas(
										Form("Cr_%d_%s",run,hist_names[hnum].c_str() ),
										Form("ratios_%d_%s",run, hist_names[hnum].c_str()),
			 									hnum*600,i*600,700,500) ;



			data_hist[hnum][i] = new TH2F(	Form("H%s_%s_%d",hist_pre[0].c_str(),hist_names[hnum].c_str(),run),	Form("%s from data run %d(%s)", hist_names[hnum].c_str(),run, TI.name.Data() ),					(int)binstats[hnum][0],binstats[hnum][1],binstats[hnum][2],					(int)binstats[hnum][3],binstats[hnum][4],binstats[hnum][5]);

			mc_hist[hnum][i] = new TH2F(				Form("H%s_%s_%d",hist_pre[1].c_str(),hist_names[hnum].c_str(),run),	Form("%s from MC run %d(%s)", hist_names[hnum].c_str(),run, TI.name.Data() ),						(int)binstats[hnum][0],binstats[hnum][1],binstats[hnum][2],					(int)binstats[hnum][3],binstats[hnum][4],binstats[hnum][5]);



			/////////////////////////////////////////////////////////////////////
			string det_str = Form("%s",data_names[hnum].c_str());
			if(hist_names[hnum]=="theta_x")
			{det_str = Form(det_str.c_str(),1/rad, central_theta);}
			string det_draw = Form("%s>>H%s_%s_%d",det_str.c_str(),hist_pre[0].c_str(),hist_names[hnum].c_str(),run);

			string mc_str = mc_names[hnum].c_str();
			if(hist_names[hnum]=="theta_x")
			{mc_str =Form(mc_str.c_str(),1/rad, central_theta);
			cout <<"CHECK!!!"<<endl;
		cout << mc_str<<endl;}
			string mc_draw= Form("%s>>H%s_%s_%d",mc_str.c_str(),
			hist_pre[1].c_str(),hist_names[hnum].c_str(),run);
			data_cut = L_mara_trig + electron_cut_L+inv_m_L + dp_cut_L + th_cut_L + ph_cut_L + z_cut_L ; //acc_cut_L+track_L;
			mc_cut = total_mc_cut + Form(" yield==yield && yield>0 && w2>%.3f" , wsqr ) ;
			if(debug>=2)
			{ cout << det_draw <<endl;
				cout <<  mc_draw << endl;
			}
			if(debug>=2)
			{ cout << data_cut <<endl;
				cout <<   mc_cut << endl;
			}
			weight="1.0*yield";
      weight=Form("(1.0*(yield) *%f* (1 - %s) )" ,RunC[0]*ECC,pcstr.c_str());
			if(debug>=2)
			{
				cout <<   weight << endl;
			}


			C_runs[hnum][i]->cd(1);
			dataTree[i]->Draw( Form( "%s", det_draw.c_str()) , data_cut*Form("%s",data_w.c_str() ), "colz");

			C_runs[hnum][i]->cd(2);
			int mod=1;
			mcTree[i]->Draw( Form("%d*%s",mod,mc_draw.c_str()) , mc_cut*Form("%s",weight.c_str() ), "colz");
			///Resize x
			int max_x = data_hist[hnum][i]->FindLastBinAbove(1);
			int min_x = data_hist[hnum][i]->FindFirstBinAbove(1);
			data_hist[hnum][i]->GetXaxis()->SetRange(min_x-2,max_x+2);
			mc_hist[hnum][i]  ->GetXaxis()->SetRange(min_x-2,max_x+2);
			data_hist[hnum][i]->GetXaxis()->SetTitle(x_axis[hnum].c_str());
			data_hist[hnum][i]->GetYaxis()->SetTitle(y_axis[hnum].c_str());
			C_runs[hnum][i]->Update();

			ratio_hist[hnum][i] = (TH2F*)data_hist[hnum][i]->Clone();
			ratio_hist[hnum][i]->Divide(mc_hist[hnum][i]);
			ratio_hist[hnum][i]->SetTitle(Form("%s ratio data/MC for run %d(%s)", hist_titles[hnum].c_str(), run, TI.name.Data()));
			ratio_hist[hnum][i]->GetZaxis()->SetRangeUser(0.1,20);
			ratio_hist[hnum][i]->GetXaxis()->SetTitle(x_axis[hnum].c_str());
			ratio_hist[hnum][i]->GetYaxis()->SetTitle(y_axis[hnum].c_str());
			C_ratios[hnum][i]->cd();
			C_ratios[hnum][i]->SetLogz();
			ratio_hist[hnum][i]->SetContour(35);
			ratio_hist[hnum][i]->Draw("colz,TEXT,po");
			if( only =="ratio" || only == "ratios" || only=="r")
			{
				delete C_runs[hnum][i];
			}


		 }//end of histogram

		 ////



	}//end of the run!!




}//end of it all
