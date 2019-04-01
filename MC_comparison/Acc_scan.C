#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"
////////////////////////////////////
// Sript for comparing phys varibles between MC and data
// Run with options -r list of runs, to look at a list of runs
// debug has levels, 0 off, 1 basic ....



void Acc_scan(string options = "",int noc =30, string only="", int debug=3)
{
	int ifdp=0;	int ifth=0;	int ifph=0;
	if(only==""){ifdp=1;ifth=1;ifph=1;}
	if(only.find("dp")!=only.npos){ifdp=1; cout << "Do dp" <<endl;}
	if(only.find("th")!=only.npos){ifth=1; cout << "Do th" <<endl;}
	if(only.find("ph")!=only.npos){ifph=1; cout << "Do ph" <<endl;}

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
	Int_t Run = runs[0];
	TChain *T =new TChain("T");
	T= LoadRun(Run);
	TChain *mc=new TChain("h9040");
	mc= LoadMC(Run);


	//Pre defined standard cuts
	tg_ph_L_min= -0.020;
	tg_ph_L= 0.020;
	tg_th_L_min =-0.04;
	tg_th_L =0.04;
	tg_dp_L_min = -0.04;
	tg_dp_L = 0.04;

	mc_yptar_cut = Form("yptar>=%.3f && yptar <=%.3f",tg_ph_L_min,tg_ph_L);
	mc_xptar_cut = Form("-xptar>=%.3f && -xptar <=%.3f",tg_th_L_min,tg_th_L);
	mc_dp_cut = Form("delta>=%.3f && delta <=%.3f", tg_dp_L_min*100,tg_dp_L*100);
	mc_vz_cut = Form("ztar>=%.3f && ztar<=%.3f" , tg_vz_L_min*100,tg_vz_L*100);

	dp_cut_L_max=Form("fabs(L.tr.tg_dp)<%g",tg_dp_L);
	th_cut_L_max=Form("fabs(L.tr.tg_th)<%g",tg_th_L);
	ph_cut_L_max=Form("fabs(L.tr.tg_ph)<%g",tg_ph_L);
	dp_cut_L_min=Form("(L.tr.tg_dp)>%g",tg_dp_L_min);
	th_cut_L_min=Form("(L.tr.tg_th)>%g",tg_th_L_min);
	ph_cut_L_min=Form("(L.tr.tg_ph)>%g",tg_ph_L_min);

	dp_cut_L = dp_cut_L_max&&dp_cut_L_min;
	th_cut_L = th_cut_L_max&&th_cut_L_min;
	ph_cut_L = ph_cut_L_max&&ph_cut_L_min;

	vector < vector<double>> dp_min;
	vector < vector<double>> dp_max;
	vector < vector<double>> th_min;
	vector < vector<double>> th_max;
	vector < vector<double>> ph_min;
	vector < vector<double>> ph_max;


	//Histos
	TH1F *Hd_delta  = new TH1F("Hd_delta","Data Delta",50,-0.07,0.07);
	TH1F *Hmc_delta = new TH1F("Hmc_delta", "MC Delta",50,-0.07,0.07);
	TH1F *Hr_delta[noc];
	TH1F *Hd_phi  = new TH1F("Hd_phi","Data Phi tar",30,-0.07,0.07);
	TH1F *Hmc_phi = new TH1F("Hmc_phi", "MC Phi tar",30,-0.07,0.07);
	TH1F *Hr_phi[noc];
	TH1F *Hd_theta  = new TH1F("Hd_theta","Data Theta tar",30,-0.07,0.07);
	TH1F *Hmc_theta = new TH1F("Hmc_theta", "MC Theta tar",30,-0.07,0.07);
	TH1F *Hr_theta[noc];
	for(int i =0;i<noc;i++)
	{
	 Hr_theta[i]= new TH1F(Form("Hr_theta%d",i),"Ratio Theta tar",30,-0.07,0.07);
 	 Hr_delta[i]= new TH1F(Form("Hr_delta%d",i),"Ratio Delta",50,-0.07,0.07);
   Hr_phi[i]= new TH1F(Form("Hr_phi%d"  ,i),"Ratio Phi tar",30,-0.07,0.07);

	}

	vector <double> DensC = DensityCor(Run);
	vector<double> RunC =SQLRunCorrection(Run);
	PositronCor PC = GetPosInfo(Run);
	string pcstr=getposstringmc(PC);
	auto RI= GetRunInfo(Run);
	string kinn = Form("%s",RI.kinematic.Data());
	string tgtt = Form("%s",RI.target.Data());

	tgtt = shortTgtName(tgtt);

	double ECC = 1 - ECC_Cor(stoi(kinn),tgtt);

		string weight=Form("(1.0*(yield)*%f* %f/(%f* (1 - %s)) )" ,DensC[0],RunC[0],ECC,pcstr.c_str());
TLegend *leg1 = new TLegend(0.1,0.7,0.3,0.9);	leg1->SetFillStyle(0);
TLegend *leg2 = new TLegend(0.1,0.7,0.3,0.9);	leg2->SetFillStyle(0);
///////////////////////////////////////////////////////////
	if(ifdp){

	//first loop do delta
	TCanvas *C_delta = new TCanvas("Cd","delta cuts- plotting theta");
	C_delta->Divide(1,2);

	double dp_step_size = abs(tg_dp_L *1.50 - tg_dp_L *0.25)/(noc/2.0);



	for(int i=0; i<noc;i++)
	{
		vector <double> tmpv;
		//First half: do negative side;
		if(i<noc/2.0)
		{
			C_delta->cd(1);
			double new_dp_min = -tg_dp_L*1.75 + dp_step_size*i;
			string newdelta_mccut = Form("delta>=%.3f && delta <=%.3f", new_dp_min*100,tg_dp_L*100);
			string newdelta_cut = Form("(L.tr.tg_dp)>%g && (L.tr.tg_dp)<%g" ,new_dp_min,tg_dp_L);


			TCut mccut = Form("w2>%.3f && %s && %s && %s && %s",wsqr,mc_yptar_cut.c_str(), mc_xptar_cut.c_str(), mc_vz_cut.c_str(), newdelta_mccut.c_str());
			TCut Dcut  = th_cut_L + ph_cut_L+ z_cut_L+inv_m_L+ Form("%s",newdelta_cut.c_str()) + layers_electron_cut_L;


			if(i==0)cout << weight<<endl;

			T->Draw("L.tr.tg_th>>Hd_theta",Dcut,"goff");
			mc->Draw("-xptar>>Hmc_theta",mccut*Form("%s",weight.c_str()),"goff");
			Hr_theta[i] = (TH1F*)Hd_theta->Clone();
			Hr_theta[i]->Divide(Hmc_theta);

			Hr_theta[i]->SetLineColor(860 + (i-10));

			Hr_theta[i]->Draw("same");
			if(i==0) leg1->AddEntry(Hr_theta[i],Form("dp min %.3f",new_dp_min),"l");
			if(i==(noc/2)-1) {leg1->AddEntry(Hr_theta[i],Form("dp min %.3f",new_dp_min),"l"); leg1->Draw("same");}

			cout << new_dp_min <<"  "<<Hd_theta->Integral(0,29)<< "  "<<Hmc_theta->Integral(0,29)<<"  " << Hd_theta->Integral(0,29)/Hmc_theta->Integral(0,29) <<endl;
				tmpv.push_back(new_dp_min );
				tmpv.push_back(Hd_theta->Integral(0,29)/Hmc_theta->Integral(0,29));
				tmpv.push_back(Hd_theta->Integral(0,29));
				dp_min.push_back(tmpv);
				tmpv.clear();
				if(i==(noc/2)-1)cout << "\n\n";
			}

		if(i>=noc/2.0)
		{
			C_delta->cd(2);
			double new_dp_max = tg_dp_L*1.750 - dp_step_size*(i-noc/2.0);
			string newdelta_mccut = Form("delta>=%.3f && delta <=%.3f",   -tg_dp_L*100 ,new_dp_max*100);
			string newdelta_cut = Form("(L.tr.tg_dp)>%g && (L.tr.tg_dp)<%g" ,-tg_dp_L,new_dp_max);

			TCut mccut = Form("w2>%.3f && %s && %s && %s && %s",wsqr,mc_yptar_cut.c_str(), mc_xptar_cut.c_str(), mc_vz_cut.c_str(), newdelta_mccut.c_str());
			TCut Dcut  = th_cut_L + ph_cut_L+ z_cut_L+inv_m_L+ Form("%s",newdelta_cut.c_str()) + layers_electron_cut_L;
	//		string weight=Form("(1.0*(yield)*%f *%f* (1 - %s) )" ,DensC[0],RunC[0]*ECC,pcstr.c_str());

			cout <<newdelta_cut <<endl;

			T->Draw("L.tr.tg_th>>Hd_theta",Dcut,"goff");
			mc->Draw("-xptar>>Hmc_theta",mccut*Form("%s",weight.c_str()),"goff");
			Hr_theta[i] = (TH1F*)Hd_theta->Clone();
			Hr_theta[i]->Divide(Hmc_theta);
			Hr_theta[i]->SetLineColor(860 + (i-10));
			Hr_theta[i]->Draw("same");
			if(i==(noc/2)) leg2->AddEntry(Hr_theta[i],Form("dp max %.3f",new_dp_max),"l");
			if(i==(noc)-1) {leg2->AddEntry(Hr_theta[i],Form("dp max %.3f",new_dp_max),"l"); leg2->Draw("same");}
			cout << new_dp_max <<"  "<<Hd_theta->Integral(0,29)<< "  "<<Hmc_theta->Integral(0,29)<<"  " << Hd_theta->Integral(0,29)/Hmc_theta ->Integral(0,29) <<endl;
			tmpv.push_back(new_dp_max);
			tmpv.push_back(Hd_theta->Integral(0,29)/Hmc_theta->Integral(0,29));
			tmpv.push_back(Hd_theta->Integral(0,29));
			dp_max.push_back(tmpv);
			tmpv.clear();
		}
	}
}//onlydp

if(ifth){
	cout << "\n\nstarting theta cut\n";
TCanvas *C_theta = new TCanvas("Ct","theta cuts- plotting phi");
C_theta->Divide(1,2);
leg1 = new TLegend(0.1,0.7,0.3,0.9);	leg1->SetFillStyle(0);
leg2 = new TLegend(0.1,0.7,0.3,0.9);	leg2->SetFillStyle(0);
th_step_size = abs(tg_th_L *1.50 - tg_th_L *0.25)/(noc/2.0);


for(int i=0; i<noc;i++)
{
	vector <double> tmpv;
	//First half: do negative side;
	if(i<noc/2.0)
	{
		C_theta->cd(1);
		double new_th_min = -tg_th_L*1.750 + th_step_size*i;
		string newth_mccut = Form("-xptar>=%.3f && -xptar <=%.3f", new_th_min,tg_th_L);
		string newth_cut = Form("(L.tr.tg_th)>%g && (L.tr.tg_th)<%g" ,new_th_min,tg_th_L);


		TCut mccut = Form("w2>%.3f && %s && %s && %s && %s",wsqr,mc_yptar_cut.c_str(), mc_dp_cut.c_str(), mc_vz_cut.c_str(), newth_mccut.c_str());
		TCut Dcut  = dp_cut_L + ph_cut_L+ z_cut_L+inv_m_L+ Form("%s",newth_cut.c_str()) + layers_electron_cut_L;
		//string weight=Form("(1.0*(yield)*%f *%f* (1 - %s) )" ,DensC[0],RunC[0]*ECC,pcstr.c_str());
		T->Draw("L.tr.tg_ph>>Hd_phi",Dcut,"goff");
		mc->Draw("yptar>>Hmc_phi",mccut*Form("%s",weight.c_str()),"goff");
		Hr_phi[i] = (TH1F*)Hd_phi->Clone();
		Hr_phi[i]->Divide(Hmc_phi);

		Hr_phi[i]->SetLineColor(860 + (i-10));

		Hr_phi[i]->Draw("same");
		if(i==0) leg2->AddEntry(Hr_phi[i],Form("phi %3f",new_th_min),"l");
		if(i==(noc/2)-1) {leg2->AddEntry(Hr_phi[i],Form("phi %3f",new_th_min),"l"); leg2->Draw("same");}

		cout << new_th_min <<"  "<<Hd_phi->Integral(0,29)<< "  "<<Hmc_phi->Integral(0,29)<<"  " << Hd_phi->Integral(0,29)/Hmc_phi->Integral(0,29) <<endl;

			tmpv.push_back(new_th_min );
			tmpv.push_back(Hd_phi->Integral(0,29)/Hmc_phi->Integral(0,29));
			tmpv.push_back(Hd_phi->Integral(0,29));
			th_min.push_back(tmpv);
			tmpv.clear();

			if(i==(noc/2)-1)cout << "\n\n";
		}

		if(i>=noc/2.0)
		{
			C_theta->cd(2);
			double new_th_max = tg_th_L*1.750 - th_step_size*(i-noc/2.0);
			string newth_mccut = Form("-xptar>=%.3f && -xptar <=%.3f" ,-tg_th_L, new_th_max);
			string newth_cut = Form("(L.tr.tg_th)>%g && (L.tr.tg_th)<%g" ,-tg_th_L,new_th_max);


			TCut mccut = Form("w2>%.3f && %s && %s && %s && %s",wsqr,mc_yptar_cut.c_str(), mc_dp_cut.c_str(), mc_vz_cut.c_str(), newth_mccut.c_str());
			TCut Dcut  = dp_cut_L + ph_cut_L+ z_cut_L+inv_m_L+ Form("%s",newth_cut.c_str()) + layers_electron_cut_L;
		//	string weight=Form("(1.0*(yield)*%f *%f* (1 - %s) )" ,DensC[0],RunC[0]*ECC,pcstr.c_str());
			T->Draw("L.tr.tg_ph>>Hd_phi",Dcut,"goff");
			mc->Draw("yptar>>Hmc_phi",mccut*Form("%s",weight.c_str()),"goff");
			Hr_phi[i] = (TH1F*)Hd_phi->Clone();
			Hr_phi[i]->Divide(Hmc_phi);

			Hr_phi[i]->SetLineColor(860 + (i-10));
		cout <<newth_cut <<endl;
			Hr_phi[i]->Draw("same");
			if(i==(noc/2)) leg1->AddEntry(Hr_phi[i],Form("phi %3f",new_th_max),"l");
			if(i==(noc)-1) {leg1->AddEntry(Hr_phi[i],Form("phi %3f",new_th_max),"l"); leg1->Draw("same");}

			cout << new_th_max <<"  "<<Hd_phi->Integral(0,29)<< "  "<<Hmc_phi->Integral(0,29)<<"  " << Hd_phi->Integral(0,29)/Hmc_phi->Integral(0,29) <<endl;
				tmpv.push_back(new_th_max );
				tmpv.push_back(Hd_phi->Integral(0,29)/Hmc_phi->Integral(0,29));
				tmpv.push_back(Hd_phi->Integral(0,29));
				th_max.push_back(tmpv);
				tmpv.clear();
			}

		}
}//onlyth

if(ifph){
cout << "\n\nstarting phi cut\n";

		TCanvas *C_phi = new TCanvas("Cp","phi cuts- plotting dp");
		C_phi->Divide(1,2);
		leg1 = new TLegend(0.1,0.7,0.3,0.9);	leg1->SetFillStyle(0);
		leg2 = new TLegend(0.1,0.7,0.3,0.9);	leg2->SetFillStyle(0);
		ph_step_size = abs(tg_ph_L *1.50 - tg_ph_L *0.25)/(noc/2.0);


		for(int i=0; i<noc;i++)
		{
			vector <double> tmpv;
			//First half: do negative side;
			if(i<noc/2.0)
			{
				C_phi->cd(1);
				double new_ph_min = -tg_ph_L*1.750 + ph_step_size*i;
				string newph_mccut = Form("yptar>=%.3f && yptar <=%.3f", new_ph_min,tg_ph_L);
				string newph_cut = Form("(L.tr.tg_ph)>%g && L.tr.tg_ph<%g" ,new_ph_min,tg_ph_L);


				TCut mccut = Form("w2>%.3f && %s && %s && %s && %s",wsqr,mc_xptar_cut.c_str(), mc_dp_cut.c_str(), mc_vz_cut.c_str(), newph_mccut.c_str());
				TCut Dcut  = dp_cut_L + th_cut_L+ z_cut_L+inv_m_L+ Form("%s",newph_cut.c_str()) + layers_electron_cut_L;
		//		string weight=Form("(1.0*(yield)*%f *%f* (1 - %s) )" ,DensC[0],RunC[0]*ECC,pcstr.c_str());
				T->Draw("L.tr.tg_dp>>Hd_delta",Dcut,"goff");
				mc->Draw("delta/100.0>>Hmc_delta", mccut*Form("%s",weight.c_str()),"goff");
				Hr_delta[i] = (TH1F*)Hd_delta->Clone();
				Hr_delta[i]->Divide(Hmc_delta);

				Hr_delta[i]->SetLineColor(860 + (i-10));

				Hr_delta[i]->Draw("same");
				if(i==0) leg2->AddEntry(Hr_delta[i],Form("delta %3f",new_ph_min),"l");
				if(i==(noc/2)-1) {leg2->AddEntry(Hr_delta[i],Form("delta %3f",new_ph_min),"l"); leg2->Draw("same");}

				cout << new_ph_min <<"  "<<Hd_delta->Integral(0,29)<< "  "<<Hmc_delta->Integral(0,29)<<"  " << Hd_delta->Integral(0,29)/Hmc_delta->Integral(0,29) <<endl;
					tmpv.push_back(new_ph_min );
					tmpv.push_back(Hd_delta->Integral(0,29)/Hmc_delta->Integral(0,29));
					tmpv.push_back(Hd_delta->Integral(0,29));
					ph_min.push_back(tmpv);
					tmpv.clear();
					if(i==(noc/2)-1)cout << "\n\n";
				}

				if(i>=noc/2.0)
				{
					C_phi->cd(2);
					double new_ph_max = tg_ph_L*1.750 + ph_step_size*i;
					string newph_mccut = Form("yptar>=%.3f && yptar <=%.3f",tg_ph_L, new_ph_max);
					string newph_cut = Form("(L.tr.tg_ph)>%g && L.tr.tg_ph<%g" ,tg_ph_L,new_ph_max);


					TCut mccut = Form("w2>%.3f && %s && %s && %s && %s",wsqr,mc_xptar_cut.c_str(), mc_dp_cut.c_str(), mc_vz_cut.c_str(), newph_mccut.c_str());
					TCut Dcut  = dp_cut_L + th_cut_L+ z_cut_L+inv_m_L+ Form("%s",newph_cut.c_str()) + layers_electron_cut_L;
			//		string weight=Form("(1.0*(yield)*%f *%f* (1 - %s) )" ,DensC[0],RunC[0]*ECC,pcstr.c_str());
					T->Draw("L.tr.tg_dp>>Hd_delta",Dcut,"goff");
					mc->Draw("delta/100.0>>Hmc_delta", mccut*Form("%s",weight.c_str()),"goff");
					Hr_delta[i] = (TH1F*)Hd_delta->Clone();
					Hr_delta[i]->Divide(Hmc_delta);

					Hr_delta[i]->SetLineColor(860 + (i-10));

					Hr_delta[i]->Draw("same");

					Hr_delta[i]->SetLineColor(860 + (i-10));

					Hr_delta[i]->Draw("same");
					if(i==(noc/2)) leg1->AddEntry(Hr_delta[i],Form("delta %3f",new_ph_max),"l");
					if(i==(noc)-1) {leg1->AddEntry(Hr_delta[i],Form("delta %3f",new_ph_max),"l"); leg1->Draw("same");}
		cout <<newph_cut <<endl;
					cout << new_ph_max <<"  "<<Hd_delta->Integral(0,29)<< "  "<<Hmc_delta->Integral(0,29)<<"  " << Hd_delta->Integral(0,29)/Hmc_delta->Integral(0,29) <<endl;
						tmpv.push_back(new_ph_max );
						tmpv.push_back(Hd_delta->Integral(0,29)/Hmc_delta->Integral(0,29));
						tmpv.push_back(Hd_delta->Integral(0,29));
						ph_max.push_back(tmpv);
						tmpv.clear();
					}

			}
		}//onlyph
			ofstream output; output.open(Form("./acc_scan_%d.csv",Run));
			output<<"i,dp_min,theta_rmin,dp_min_count,dp_max,theta_rmax,dp_max_count,th_min,phi_rmin,th_min_count,th_max,phi_rmax,th_max_count";
			output<<",ph_min,dp_rmin,ph_min_count,ph_max,dp_rmax,ph_max_count\n";
			for(int i=0;i<noc/2;i++)
			{
				output<<i<<",";
				if(ifdp)output<< dp_min[i][0]<<","<<dp_min[i][1]<<","<< dp_min[i][2] << ",";
				if(ifdp)output<< dp_max[i][0]<<","<<dp_max[i][1]<<","<< dp_max[i][2]<<",";
				if(ifdp)output<< th_min[i][0]<<","<<th_min[i][1]<<","<<th_min[i][2]<<",";
				if(ifth)output<< th_max[i][0]<<","<<th_max[i][1]<<","<<th_max[i][2]<<",";
				if(ifph)output<< ph_min[i][0]<<","<<ph_min[i][1]<<","<<ph_min[i][2]<<",";
				if(ifph)output<< ph_max[i][0]<<","<<ph_max[i][1]<<","<<ph_max[i][2]<<"\n";

			}
			output.close();

			Hr_theta[0]->GetXaxis()->SetTitle("Target Theta");
			Hr_theta[0]->GetYaxis()->SetTitle("Data/MC");
			Hr_theta[noc/2]->GetXaxis()->SetTitle("Target Theta");
			Hr_theta[noc/2]->GetYaxis()->SetTitle("Data/MC");
/*



			T->Draw("exL.dp>>Hd_delta",Dcut,"goff");
			mc->Draw("delta/100>>Hmc_delta",mccut*Form("%s",weight.c_str()),"goff");
			Hr_delta = (TH1F*)Hd_delta->Clone();
			Hr_delta->Divide(Hmc_delta);
			cout << new_dp_max <<"  " << Hd_delta->Integral(0,29)/Hmc_delta->Integral(0,29) <<endl;
*/

}
