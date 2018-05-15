#include "../headers/inc1.h"
#include "../headers/rootalias1.h"
#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"

void pid_limit_scan(TString tgt ="EM"){
	TString loc = "/work/halla/triton/Runlist/";
        TCanvas *C = new TCanvas("C","C");

	TChain *T[20];
	TH1F *cer[20];	
	vector<int> cor;

	int count =0;
	for(int i=1;i<=16;i++){
		//T[i] = LoadKin(Form("%s_kin%d.dat",tgt.Data(),i));
		TString KIN = Form("%s_kin%d.dat",tgt.Data(),i);
		kin_file *K_F = new kin_file;
		K_F->set_file(Form("%s%s",loc.Data(),KIN.Data()));
		if(K_F->run_file_status==0){cout <<"skipping kin " << i <<endl;continue;}	
		vector<int> Run_String = gGet_RunNoChain(K_F->run_string);
		T[i] = gGetTree(Run_String);
	//	if(T[i]==nullptr){cout <<"skipping kin " << i <<endl;continue;}	
		
		TString ARM,Arm;
	        if(Run_String[0]<90000){ARM="Left"; Arm="L";}
        	else{ARM="Right";Arm="R";}


		set_limits(Form("%s_kin%d",tgt.Data(),i));
		TString GECs = Good_Electron_Cuts(ARM,0,1,1,1,1);


		cer[i]= new TH1F(Form("cer%d",i),Form("cer_kin%d",i),500,-1000,49000);
		T[i]->Draw(Form("%s.cer.asum_p>>cer%d",Arm.Data(),i),TCut(GECs),"goff");
		cer[i]->GetXaxis()->SetTitle("Cer asum");
		cor.push_back(i);

		delete T[i];
		count++;
		if(i>=20){break;}	
	}//end of for loop for all kin

	TSpectrum *S[count];
	double count_sqrt = sqrt(count);
	double xdivsd;
	int ydivs =0; int xdivs =0;
	ydivs = ceil(count_sqrt); xdivsd = (count/(ydivs*1.0));
	xdivs= ceil(xdivsd);
	C->Divide(ydivs,xdivs);
	for(int i=0;i<count;i++){
		C->cd(1+i);
		cout << cer[cor[i] ] <<endl;
		if(cer[cor[i] ]!=nullptr){
			cer[cor[i] ]->Draw();
			cer[cor[i] ]->GetXaxis()->SetRangeUser(-1000,19000);}
		S[i] = new TSpectrum(4,1);
		S[i]->Search(cer[cor[i]],5);
		double *pos = S[i]->GetPositionX(); 
		cout << "  : " << pos[0] <<endl;
	}
	

	C->Print(Form("/volatile/halla/triton/Bane/images2/limit_scan_%s.eps",tgt.Data()));

}//end of code
