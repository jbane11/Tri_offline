
#include "/home/jbane/HEADERS/inc1.h"
#include "/home/jbane/HEADERS/TRI_Main.h"
#include "/home/jbane/HEADERS/TRI_Tools.h"
#include "/home/jbane/HEADERS/rootalias1.h"
#include "./Pid_eff_3.C"
#include "./efficencies/Pid_eff_seperate_macro.C"
/*This script will calculate the the efficency of a PID cut scan;
	Need to supply the kin runlist to use;

Limitations/ issues


*/

using namespace std;

void PID_eff_scan(TString filename=""){


	//input the name opf the kin file and parse it
	if(filename==""){ cout<<"Input file name:   "; cin>>filename;}
	filename = "./Runlist/" + filename;
    ifstream file(filename.Data());
    if(!file.good()){cout << filename.Data() << " does not exist! "<<endl; exit(1);}
    time_t start_time; time(&start_time);
    TString content;
    TString Target,Kin,Run_String;
    const char* kin="";
    for (int ii=0; content.ReadLine(file) && ii<3;ii++ ) {  
    	cout<<"!!!:  "<<content<<endl;
        if(ii==0)Target = content;
        if(ii==1){
           kin = content.Data();
           Kin = Form("Kin%s", kin);
         }
        if(ii==2)Run_String = content;         
      }
      file.close();
   ///////////////////////////////////////////////////
   
    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
    TChain *T = gGetTree(RunNoChain,"T");
    cout << T->GetEntries()<<endl;
  	 
    	 
  	TString ARM,arm;
 	if(RunNoChain[0]<90000){ARM="Left"; arm="L";}
  	else{ARM="Right";arm="R";}
  	  	
  	  	
	//These vecotors will be used to make graphs!
	TVector PID_eff; 	//This will be the over all correction value for this run.
	TVector PID_e_eff;	//This will be the eff of electron acceptance
	TVector PID_p_eff;	//This will br the eff of pion rejection.
	TVector PID_eff_calo; 	//This will be the over all correction value for this run.
	TVector PID_e_eff_calo;	//This will be the eff of electron acceptance
	TVector PID_p_eff_calo;	//This will br the eff of pion rejection.
	TVector PID_e_er_calo;	//This will be the eff of electron acceptance
	TVector PID_p_er_calo;	//This will br the eff of pion rejection.
	TVector PID_eff_cer; 	//This will be the over all correction value for this run.
	TVector PID_e_eff_cer;	//This will be the eff of electron acceptance
	TVector PID_p_eff_cer;	//This will br the eff of pion rejection.
	TVector PID_e_er_cer;	//This will be the eff of electron acceptance
	TVector PID_p_er_cer;	//This will br the eff of pion rejection.
	TVector calo_e;     //Threshold cuts for PID with caloremeter
	TVector cer_min;	//Threshold cuts for PID with cerek
	TVector x1_error;
	TVector x2_error;
	

	TVector PID_e_eff_cor;	//This will be the eff of electron acceptance
	TVector PID_p_eff_cor;	//This will be the eff of pion rejection.
	TVector PID_e_er_cor;	//This will be the eff of electron acceptance
	TVector PID_p_er_cor;	//This will be the eff of pion rejection.	
	
	////////////////////////////////////////////////////////	

	
	

///Stat of the cut scan: Will Run  Calo scan first: then run Cer scan
		int num_calo =25; 
		double min_calo = 0.25; double max_calo = 1.25;
		double calo_step = (max_calo-min_calo)/(num_calo*1.0);
		int num_cer =25; 
		double min_cer = 100; double max_cer = 5100;
		double cer_step = (max_cer-min_cer)/(num_cer*1.0);
	
	
	for(int i =0; i<num_calo; i++){
		Double_t calo_thres = i*calo_step+min_calo;
		vector<Double_t> in_put=Pid_eff_seperate_macro(T,ARM.Data(),1,0,calo_thres,0);
		
		calo_e.ResizeTo(i+1);		calo_e[i]=calo_thres;
		x1_error.ResizeTo(i+1);		x1_error[i]=0.0;
		PID_e_eff_calo.ResizeTo(i+1);	PID_p_eff_calo.ResizeTo(i+1);
		PID_e_er_calo.ResizeTo(i+1);	PID_p_er_calo.ResizeTo(i+1);
		PID_e_er_calo[i]=in_put[1] ;	PID_p_er_calo[i]=in_put[3];
		PID_e_eff_calo[i]=in_put[0] ;	PID_p_eff_calo[i]=in_put[2];
		
	//}	

		
	//for(int i =0; i<num_cer; i++){
		
		Double_t cer_thres = i*cer_step+min_cer; 
		vector<Double_t> in_put1=Pid_eff_seperate_macro(T,ARM.Data(),0,cer_thres,0.0,0);
		
		cer_min.ResizeTo(i+1);		cer_min[i]=cer_thres;
		x2_error.ResizeTo(i+1);		x2_error[i]=0.0;
		PID_e_eff_cer.ResizeTo(i+1);PID_p_eff_cer.ResizeTo(i+1);
		PID_e_er_cer.ResizeTo(i+1);	PID_p_er_cer.ResizeTo(i+1);
		PID_e_er_cer[i]=in_put1[1];	PID_p_er_cer[i]=in_put1[3];
		PID_e_eff_cer[i]=in_put1[0]; PID_p_eff_cer[i]=in_put1[2];
	
	
		vector<Double_t> in_cor =Pid_eff_3(T,ARM,0,0,calo_thres,cer_thres,1.0,1);
			
		PID_e_eff_cor.ResizeTo(i+1);	PID_p_eff_cor.ResizeTo(i+1);
		PID_e_er_cor.ResizeTo(i+1);		PID_p_er_cor.ResizeTo(i+1);		
		PID_e_er_cor[i]=in_cor[1];		PID_p_er_cor[i]=in_cor[3];
		PID_e_eff_cor[i]=in_cor[0]; 	PID_p_eff_cor[i]=in_cor[2];
		
		
		
		time_t current_time; time(&current_time);
		double time_seconds = difftime(current_time,start_time);
		cout << i << " " << calo_thres <<" " << cer_thres<< "  "<< time_seconds/60.0 <<endl;
		}		
		
	TCanvas *com = new TCanvas("com","com",0,0,1920,1080);
	TMultiGraph *mg_com = new TMultiGraph();
        mg_com->SetTitle(Form("PID eff scan on %s for kin%s", Target.Data(),kin));	
	TGraphErrors *com_e = new TGraphErrors(calo_e,PID_e_eff_cor,x1_error,PID_e_er_cor);	
	TGraphErrors *com_p = new TGraphErrors(calo_e,PID_p_eff_cor,x1_error,PID_p_er_cor);	
	com_e->SetMarkerStyle(33);
	com_e->SetMarkerColor(2);
	com_e->GetXaxis()->SetTitle("E/P cut value");
	

	mg_com->Add(com_e);
	mg_com->Add(com_p);
	mg_com->Draw("ap");


		TCanvas *C = new TCanvas("C","C",0,0,1920,1080);
		C->Divide(1,2);
		C->cd(1);
		TMultiGraph *mg_calo = new TMultiGraph();
		mg_calo->SetTitle(Form("PID eff scan for the Calorimeters on %s for kin%s", Target.Data(),kin));
		
		TGraphErrors *Ge_calo = new TGraphErrors(calo_e,PID_e_eff_calo,x1_error,PID_e_er_calo);
		Ge_calo->SetMarkerStyle(33);
		Ge_calo->SetMarkerColor(2);
		Ge_calo->GetXaxis()->SetTitle("E/p cut value");
		
		TGraphErrors *Gp_calo = new TGraphErrors(calo_e,PID_p_eff_calo,x1_error,PID_p_er_calo);
		Gp_calo->SetMarkerStyle(23);
		Gp_calo->SetMarkerColor(4);
				
		mg_calo->Add(Ge_calo);
		mg_calo->Add(Gp_calo);
		mg_calo->Draw("ap");
		
		auto Leg = new TLegend(0.75,0.1,0.9,0.2);
		Leg->AddEntry(Ge_calo, "Electron acceptance","p");
		Leg->AddEntry(Gp_calo, "Pion Rejection", "p");
		Leg->Draw("same");
		
		
		C->cd(2);
		TMultiGraph *mg_cer = new TMultiGraph();
		mg_cer->SetTitle("PID eff scan for Cerekov");		
		TGraphErrors *Ge_cer = new TGraphErrors(cer_min,PID_e_eff_cer,x2_error,PID_e_er_cer);
		Ge_cer->SetMarkerStyle(33);
		Ge_cer->SetMarkerColor(2);
		Ge_cer->GetXaxis()->SetTitle("Cer asum threshold");
		
		TGraphErrors *Gp_cer = new TGraphErrors(cer_min,PID_p_eff_cer,x2_error,PID_p_er_cer);
		Gp_cer->SetMarkerStyle(23);
		Gp_cer->SetMarkerColor(4);

		mg_cer->Add(Ge_cer);
		mg_cer->Add(Gp_cer);
		mg_cer->Draw("ap");	
		Leg->Draw("same");	
		
		C->Print(Form("PID_eff_%s_kin%s.png", Target.Data(),kin));
		C->Print("temp.png");
		C->Print("temp.gif");
		time_t end_time; time(&end_time);
		double time_min = difftime(end_time,start_time)/60.0;
		cout << "This took " << time_min <<" minutes to run!" <<endl;
		
		
	//	in_put.print();
	//cout <<endl;
	
	
	
}
	

