#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"



void pointing_plots(int run=0){

	SetStyles();
	//survery results
	double CH_loc = 5.49*1E-3;	
	double MT1= 113.48*1E-3;
	double MT2= -195.151E-3;	


	if(run == 0){ 
		cout << "please enter the run number you would like to look at?\n";
		cin >> run;
	}

	TChain *T = LoadRun(run);
	if(T==nullptr)exit(1);
	RunInfo RI = GetRunInfo(run);
	cout <<RI.angle <<endl;	


	TH1F *H_tgy = new TH1F("h_tgy","Target y",500,-0.05 ,0.05);
	TH1F *H_rz  = new TH1F("h_rz" ,"reaction z",1000,-0.2,0.2);
	
	TH1F *H_th = new TH1F("H_th","angle", 1000,0,45);
	T->Draw("EKLx.angle>>H_th",electron_cut_L&&dp_cut_L_e&&th_cut_L_e&&ph_cut_L_e,"goff");
	double angle = H_th->GetMean();
	angle = RI.angle*rad;

cout << angle << " "<< angle/rad <<endl;
	TCanvas *C = new TCanvas("C","C");
	T->Draw("L.tr.tg_y>>h_tgy",electron_cut_L&&dp_cut_L_e&&th_cut_L_e&&ph_cut_L_e);
	TCanvas *C1 =new TCanvas("C1","C1");
	T->Draw("rpl.z>>h_rz",electron_cut_L&&dp_cut_L&&th_cut_L&&ph_cut_L);

	TargetInfo TI = GetTarget(run);

	
	TLine *Ly[11];
	TLine *Lz[11];
	double z_max = H_rz->GetMaximum();
	double y_max = H_tgy->GetMaximum();

	if(TI.name=="Multifoils"){
		H_rz->SetTitle("Reaction Z on MultiFoils");
		H_rz->GetXaxis()->SetTitle("rpl.z(meters)");	

		for(int i =0; i<11;i++){
			Ly[i]= new TLine(((i-5)*.025)*sin(angle), 0.0,((i-5)*0.025)*sin(angle) ,y_max);
			Lz[i]= new TLine(((i-5)*.025)         ,0.0,((i-5)*0.025)          ,z_max);

			C->cd();
			Ly[i]->SetLineColor(2);
			Ly[i]->Draw("same");
			C1->cd();
			Lz[i]->SetLineColor(2);
			Lz[i]->Draw("same");
	}	

		H_tgy->SetTitle("Target Y on MultiFoils");
		H_tgy->GetXaxis()->SetTitle("tr.tg_y(meters)");

		}

	gStyle->SetOptStat(1);	
	gStyle->SetOptFit(1);


	if(TI.name=="Carbon Hole")
	{
		H_rz->SetTitle("Reaction Z on Carbon hole");
		H_rz->GetXaxis()->SetTitle("rpl.z(meters)");	
		TF1 *f2 = new TF1("f2","gaus",-0.4,0.4);
		f2->SetLineColor(7);
		H_rz->Fit("f2");
		double Fit_peak2 =  f2->GetParameter(1);
		double diff2 = (Fit_peak2 - CH_loc);
		cout << "diff = " <<diff2<<" "<<Fit_peak2<<" "<< CH_loc<<endl;
		Lz[5]=new TLine(CH_loc,0.0,CH_loc,H_rz->GetMaximum());
		Lz[5]->SetLineColor(2);
		Lz[5]->SetLineWidth(2);
		Lz[5]->Draw("same");

		C->cd();	
		H_tgy->SetTitle("Target Y on Carbon hole");
		H_tgy->GetXaxis()->SetTitle("tr.tg_y(meters)");
		TF1 *f1 = new TF1("f1","gaus",-0.1,0.1);
		f1->SetLineColor(7);
		double mean = H_tgy->GetMean();
cout << mean <<endl;
		H_tgy->Fit("f1","","",mean-abs(mean*0.7),mean+abs(mean*0.7));
		double Fit_peak =  f1->GetParameter(1);
		double diff = abs(abs(Fit_peak) - abs(CH_loc*sin(angle)));
		cout << "diff = " <<diff<<endl;
		Ly[5]=new TLine(-CH_loc*sin(angle),0.0,-CH_loc*sin(angle),H_tgy->GetMaximum());
		Ly[5]->SetLineColor(2);
		Ly[5]->SetLineWidth(2);
		Ly[5]->Draw("same");
		cout <<"hole"<<endl;	
	}

	C->cd();
	TSpectrum *sy = new TSpectrum(2,1);
	TSpectrum *sz = new TSpectrum(2,1);
	if(TI.name=="Empty Cell"){
//		Ly = new TLine()
		sy->Search(H_tgy,2,"",0.05);
		cout << sy->GetNPeaks() <<endl;
		sy->Print();
	}

}
