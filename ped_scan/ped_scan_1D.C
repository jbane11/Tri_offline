#include "../headers/inc1.h"
#include "../headers/rootalias1.h"




void ped_scan_1D( string det = "L.cer", int debug=1){

	//Correct arm varribles
	int pos=-1;pos = det.find("L.");
	string Arm="",ARM="",arm="";
	int first_run=0, last_run=0;
	if(pos>=0){
		ARM="Left";
		arm="l";
		first_run =1206;
		last_run  =1208;//2890;
		}
	else {
		ARM="Right";
		arm="r";
		first_run =90880;
		last_run  =99000;
		}
	//////////////////////////
	
	//Load one file for detector checks
	TChain *Temp = LoadCalib(first_run);
	if(Temp==nullptr)
	{
		cout << "Error on first run: run number ("<< first_run<<"). Exiting!!"<<endl;
		exit(0);
	}
	////////////////////////////////////
	

	//Find the correct name for the detector to plot
	string suffix_array[] = {".a",".la",".ra"};
	vector<string> suffix (suffix_array,suffix_array + sizeof(suffix_array)/sizeof(string) );
	vector<string> detector;
	vector<int> num_of_channels;
	Temp->GetEntry(10);
	for(int suf =0; suf<suffix.size();suf++)//start of short loop through suffix
	{
		string tmp = det + suffix[suf];
		if(debug)cout << tmp<<"\t";
		TBranch *TB = Temp->FindBranch(tmp.c_str());
		TBranch *TBN; int tbn=0;
		if(debug)cout <<"\t"<<TB;	
		if(TB!=nullptr)
		{
			detector.push_back(tmp);
			Temp->SetBranchAddress(Form("Ndata.%s",tmp.c_str()), &tbn);
			//I need this draw. Y, IDK F ME!!!
			Temp->Draw(Form("Ndata.%s",tmp.c_str()),"","goff");	
			if(tbn>0)num_of_channels.push_back(tbn);
		}
		if(debug)cout<<"\t"<< tbn;
		TB =nullptr;
		TBN=nullptr;
		if(debug)cout<<endl;
	}//end of suffix loop
	delete Temp;
	/////////////////////////////	

	
	//2D Vector of pedstals for each PMT for each run!!
	vector< vector<double>> pedestals;

	TChain *T=nullptr;
		
	int num_of_runs=0;

	for(int run = first_run; run <= last_run; run++)//Start of run loop!!!!
	{
		T=LoadCalib(run);
		if(T==nullptr)
		{
			cout << "Skipping run "<< run <<endl;
			continue;
		}	

		//Loop through each part of the detector
		for(int dets=0;dets<detector.size();dets++)
		{
			//make A canvas with the closest but sligtly more 
			//or = num of pads. 
			int x_d = floor(sqrt(num_of_channels[dets]));
			int y_d = ceil(sqrt(num_of_channels[dets]));
			if(x_d*y_d < num_of_channels[dets]) x_d++;
			TCanvas *C = new TCanvas(Form("C%d",dets),Form("%s",detector[dets].c_str()));
			C->Divide(x_d,y_d);

			//Array of 1d hist, one for each pmt
			TH1F *hist[num_of_channels[dets]];
	
		
			//Loop through each PMT of the detectro
			for(int pmt=0; pmt<num_of_channels[dets];pmt++)
			{
				if(debug)cout<< "pmt "<<pmt<<"  "; 
				//Make a hist for each pmt, need to keep till end of loop 
				//so I can add this canvas to a PDF.
				hist[pmt] = new TH1F(Form("hist%d",pmt),
					Form("%s[%d]",detector[dets].c_str(),pmt)
					,100000,0,100000);
				C->cd(pmt+1);
				T->Draw(Form("%s[%d]>>hist%d",detector[dets].c_str(),pmt,pmt),"","");
				int max_adc = hist[pmt]->FindLastBinAbove(1);
				int min_adc = hist[pmt]->FindFirstBinAbove(1);
				if(debug)cout<< "Range for adc  :" << min_adc<<" "<< max_adc<<" :"; 
				hist[pmt]->GetXaxis()->SetRangeUser(min_adc-10,max_adc+10);	
				gPad->SetLogy();
				double max_bin = hist[pmt]->GetMaximumBin();
				if(debug)cout <<"  bin "<< max_bin<<"  ";

				if(debug)cout<<endl;


				



			
				if(!debug)delete hist[pmt];
			}//End of PMT loop
			
			//Add the canvas to a pd
			string pdf_o = "";
			if(run==first_run) pdf_o="(";
			if(run==last_run) pdf_o=")";

			C->Print(Form("Pedestals_%s.pdf%s",detector[dets].c_str(),pdf_o.c_str()));
			if(debug) for(int pmt=0; pmt<num_of_channels[dets];pmt++) delete hist[pmt]; 	
			
			delete C;
		}//End of suffix loop of diff dets
			
		//Clean up the TChain!
		T=nullptr;
		num_of_runs++;
		if(num_of_runs>5)break;
	}//End of run loop!!!
		

}//end of program


