#include "../headers/inc1.h"
#include "../headers/rootalias1.h"
#include <time.h>



void ped_scan( int run = 0 , int debug=0, int single_det=-1){
	if(run==0){cout << "Please Enter run number" <<"\n"; cin >> run;}
	sleep(10);
	if(debug) cout << "Debug is turn on: if you supply a run number this will be the first statement printed!! "<<"\n";

  	time_t rawtime;
  	struct tm * timeinfo;

  	time (&rawtime);
  	timeinfo = localtime (&rawtime);
  	cout << asctime(timeinfo) << "\n";

	gStyle->SetOptStat(0);
	std::vector< string> tmp_v(34);
	std::vector< std::vector<string>> ped_scan(7,tmp_v);
	ped_scan.begin();
	ped_scan.clear();

	string image = "./det";
	//Correct arm varribles
	string Arm="",ARM="",arm="";
	int first_run=0, last_run=0;
	if(run<=89000){
		Arm="L";
		ARM="Left";
		arm="l";
		}
	else {
		Arm="R";
		ARM="Right";
		arm="r";
		}
	////////////////////////	//Load one file for detector checks
	TChain *Temp = LoadCalib(run);
	if(Temp==nullptr)
	{
		cout << "Error on first run: run number ("<< first_run<<"). Exiting!!"<<"\n";
		exit(0);
	}
	Temp->SetBranchStatus("*",0);
	////////////////////////////////////

	int draw_cnt=0;
	ofstream output;
	//make a buffer for the output
	int mybuffer {131072};
	vector<char> buf;
	buf.resize(mybuffer);
	output.rdbuf()->pubsetbuf(&buf[0],mybuffer);	
	output.open(Form("./ped_table/%d.csv",run));
	if(!output.is_open()){
		cout<<"Ped table not open!"<<"\n";;
		exit(0);}	
	//Find the correct name for the detector to ploti
	string det_array[] ={".s0",".s2",".prl1",".prl2",".cer",".sh",".ps"};
	string suffix_array[] = {".a",".la",".ra"};
	vector<string> suffix (suffix_array,suffix_array + sizeof(suffix_array)/sizeof(string) );
	vector<string> det_vector (det_array,det_array + sizeof(det_array)/sizeof(string) );
	vector<string> detector;
	vector<int> num_of_channels;
	Temp->GetEntry(10);
	int max_channel=0;
	int tbn=0;
	int nod=0;
	for(unsigned int det_l=0; det_l<det_vector.size();det_l++)//start of short loop through suffix
	{	
		for(unsigned int suf =0; suf<suffix.size();suf++)//start of short loop through suffix
		{
			Temp->ResetBranchAddresses();
			string tmp =Arm+ det_vector[det_l] + suffix[suf];
			if(debug)cout << tmp<<"   \t";
			TBranch *TB = Temp->FindBranch(tmp.c_str());
			TBranch *TBN; 
			if(debug)cout <<TB<<" ";	
			if(TB!=nullptr)
			{
			
				detector.push_back(tmp);
				Temp->SetBranchStatus(Form("%s",tmp.c_str()),1);
				Temp->SetBranchStatus(Form("Ndata.%s",tmp.c_str()),1);
				Temp->SetBranchAddress(Form("Ndata.%s",tmp.c_str()), &tbn);
				//I need this draw. Y, IDK F ME!!!
//				Temp->Draw(Form("Ndata.%s",tmp.c_str()),"","goff");	
				Temp->GetEntry(10+nod);
				if(tbn>0)num_of_channels.push_back(tbn);

				nod++;
			}
			if(debug)cout<<"  \t"<< tbn;
			TB =nullptr; 
			TBN=nullptr;
			if(tbn>max_channel)max_channel=tbn;
			
			tbn=0;
			if(debug)cout<<"\n";
		}//end of suffix loop
	}//end of prefix loop
	///////////////////////
	////////	

	if(debug)cout<< "Found " << detector.size() << " applical(sp) dets!" <<"\n";
	if(debug)cout<< "The maximum number of channels is " <<max_channel<<"\n";


	vector <string> oneD; oneD.push_back("Detector/PMT#");
	for(int j=0;j<max_channel;j++)
	{
		oneD.push_back(to_string(j));
		oneD.push_back(Form("Sigma_%d",j));
		if(debug) cout << oneD[j*2] <<",";
	}
	if(debug)cout<<oneD[oneD.size()-1];
	if(debug)cout<<"\n";
	ped_scan.push_back(oneD);
	oneD.clear();
	
	//2D Vector of pedstals for each PMT for each run!!
	vector<string> pedestals;

	TChain *T=nullptr;
		
	int num_of_runs=0;
	unsigned int detss =0;
	if(single_det>=0)detss=single_det;
///////////////////////////Which det!!!!///////////////////////
	//Loop through each part of the detector
	for(unsigned int dets=detss;dets<detector.size();dets++)
	{
	
if(debug)cout<<"Debug: start of dets loop:  " << dets <<"\n";
		//make A canvas with the closest but sligtly more 
		//or = num of pads. 
		int x_d = floor(sqrt(num_of_channels[dets]));
		int y_d = ceil(sqrt(num_of_channels[dets]));
		if(x_d*y_d < num_of_channels[dets]) x_d++;
		int num_of_Cs = ceil(num_of_channels[dets]/12.0);
		TCanvas *C[num_of_Cs];
		int C_select=0;
		if(debug)cout <<"There will be " << num_of_Cs<< " canvases!"<<"\n";

		for(int noc =0;noc<num_of_Cs;noc++)
		{
			if(debug) cout << "Canvas :"<< noc+1<<"  ";
			C[noc] = new TCanvas(Form("C%d_%d",dets,noc),
				Form("%s canvas %d/%d",detector[dets].c_str(),noc+1,num_of_Cs));
			if((num_of_channels[dets]-(noc*12))>12){
				 C[noc]->Divide(3,4);
				if(debug) cout << "is 3x4"<< "  ";
				}
			else{ int num_left = (num_of_channels[dets]-(noc*12));
				x_d = floor(sqrt(num_left*1.0));
				y_d = ceil(sqrt(num_left*1.0));
				C[noc]->Divide(x_d,y_d);
				if(debug) cout << "is"<< x_d<< " x " <<y_d<< "  ";
			}
			if(debug)cout << "there are "<< (num_of_channels[dets]-(noc*12)) <<" left"<< "\n";
		
		}
		oneD.push_back(Form("%s",detector[dets].c_str()));

		//Array of 1d hist, one for each pmt
		TH1F *hist[num_of_channels[dets]];
		TF1  *F1[num_of_channels[dets]];
		TF1  *F2[num_of_channels[dets]];
		TSpectrum *S[num_of_channels[dets]];
		TLegend *leg[num_of_channels[dets]];
		//Loop through each PMT of the detectro
		int pad=1; //pad for drawing
		int pmt=0;
if(debug)cout<<"Debug: start of pmt loop:  " <<"\n";
		for(pmt=0; pmt<num_of_channels[dets];pmt++)
		{

			if(pmt/12.0 == pmt/12&&pmt>>0){C_select++;pad=1; if(debug){cout << "Next canvas" <<"\n";}}
			if(debug)cout<< "pmt "<<pmt<<" "; 
			//Make a hist for each pmt, need to keep till end of loop 
			//so I can add this canvas to a PDF.
			hist[pmt] = new TH1F(Form("hist%d",pmt),
				Form("%s[%d]",detector[dets].c_str(),pmt)
				,10000,0,50000);
			if(debug) cout<< "Debug: intialize hist" <<"\n";	
			
			C[C_select]->cd(pad);
			//Draw the branch.

			 
  			time (&rawtime);
 			timeinfo = localtime (&rawtime);
			cout<<"Draw at " << asctime(timeinfo)<<"\n";
			Temp->Draw(Form("%s[%d]>>hist%d",detector[dets].c_str(),pmt,pmt),"","");
			draw_cnt++;
			//Rezise the histogram. 
			Int_t max_adc_bin = hist[pmt]->FindLastBinAbove(3);
			Int_t min_adc_bin = hist[pmt]->FindFirstBinAbove(0);
			Int_t max_adc = hist[pmt]->GetXaxis()->GetBinCenter(max_adc_bin);
			Int_t min_adc = hist[pmt]->GetXaxis()->GetBinCenter(min_adc_bin);
			hist[pmt]->GetXaxis()->SetRangeUser(min_adc-100,max_adc+100);	
			gPad->SetLogy();
			if(debug)cout<< "Range 4 adc:" << min_adc<<" "<< max_adc<<" "; 
			/////////////////////////////////////////////////////////////////

			//Find up to 2 peaks and store their values into peaks!
			S[pmt] = new TSpectrum(10);
			S[pmt]->Search(hist[pmt],1.025,"nodraw",0.001);
			double *peaks = S[pmt]->GetPositionX();
			int Npeaks = S[pmt]->GetNPeaks();	
			if(debug) cout<< Npeaks <<" Peaks ";
			if(debug)for(int np = 0;np<Npeaks;np++)cout<< peaks[np]<<" ";
						

			double peak_1= peaks[0];
			double peak_2=peak_1;
			if(Npeaks>=2){
				if(peaks[1]<peaks[0]){peak_1=peaks[1];peak_2=peaks[0];}
				else{peak_2=peaks[1];}
			}	
			
			F1[pmt]=new TF1(Form("%s[%d]",detector[dets].c_str(),pmt),
					      "gaus(0)",peak_1*0.85,peak_1*1.15);
			F1[pmt]->SetLineColor(6);	
			F1[pmt]->SetLineStyle(7);
			F1[pmt]->SetLineWidth(3);
			F1[pmt]->SetParLimits(2,0,peak_1*0.125);
			F1[pmt]->SetParLimits(1,peak_1*0.95,peak_1*1.05);
			hist[pmt]->Fit(Form("%s[%d]",detector[dets].c_str(),pmt),"Q","",
						peak_1*0.9,peak_1*1.07);
			TF1 *tmpF1 = F1[pmt];
			int counter=0;
			double red_x_F1 = F1[pmt]->GetChisquare()/(F1[pmt]->GetNDF()*1.0);
			double old_red =red_x_F1;
			if(debug)cout <<"\t"<< F1[pmt]->GetParameter(1)<<" " <<red_x_F1<<" "<<"\n";
			while(red_x_F1>7.00)
			{
				counter++;
				double min_fit = peak_1*(0.95+(counter*0.005));	
				double max_fit = peak_1*(1.05-(counter*0.005)); 
				F1[pmt]->SetParLimits(1,min_fit,max_fit);
				hist[pmt]->Fit(Form("%s[%d]",detector[dets].c_str(),pmt),"Q","",
						min_fit,max_fit);
				red_x_F1 = F1[pmt]->GetChisquare()/(F1[pmt]->GetNDF()*1.0);
				if(debug)cout<<" Refit " << F1[pmt]->GetParameter(1)
					     <<" " <<red_x_F1<<" " <<max_fit<<" "<<"\n";
				if(old_red<=red_x_F1){
					if(counter==1){
						//F1[pmt]->SetParLimits(1,peak_1*0.95,peak_1*1.05);
						//hist[pmt]->Fit(Form("%s[%d]",detector[dets].c_str(),pmt),"Q","",
						//peak_1*0.95,peak_1*1.05);
						F1[pmt]=tmpF1;
						break;
					}
					F1[pmt]->SetParLimits(1,peak_1*(0.95+((counter-1)*0.005)),
								peak_1*(1.05-((counter-1)*0.005)));
					hist[pmt]->Fit(Form("%s[%d]",detector[dets].c_str(),pmt),"Q","",
					peak_1*(0.95+((counter-1)*0.005)),peak_1*(1.05-((counter-1)*0.005)));
					break;
				}

				old_red=red_x_F1;
				if(counter>8)break;

			}//out of refit

			F1[pmt]->Draw("same");

			F2[pmt]=new TF1(Form("2_%s[%d]",detector[dets].c_str(),pmt),
					      "gaus(0)",peak_2*0.75,peak_2*1.25);
			F2[pmt]->SetLineStyle(7);
			F2[pmt]->SetLineColor(1);	
			F2[pmt]->SetParameter(1,peak_2);
			F2[pmt]->SetParLimits(2,0,peak_2*0.025);
			F2[pmt]->SetParLimits(1,peak_2*0.85,peak_2*1.15);
			hist[pmt]->Fit(Form("2_%s[%d]",detector[dets].c_str(),pmt),"QN","",
						peak_2*0.9,peak_2*1.1);
			
			double red_x_F2 = F2[pmt]->GetChisquare()/(F2[pmt]->GetNDF()*1.0);
			red_x_F1 = F1[pmt]->GetChisquare()/(F1[pmt]->GetNDF()*1.0);
	
			if(debug){cout <<"\n";
				cout <<"\t"<< F1[pmt]->GetParameter(1)<<" " <<red_x_F1<<" ";
				cout << "\t"<< F2[pmt]->GetParameter(1)<<" " <<red_x_F2<<" ";
				cout << peaks[0]<<" "<<peaks[1] <<"\n";
			}
			
		
			oneD.push_back(to_string(F1[pmt]->GetParameter(1)));
			oneD.push_back(to_string(F1[pmt]->GetParameter(2)));	
			if(debug)cout<<"\n";
			//////////////////////////////////
			//write the ped and draw legend//
			//
			if((max_adc+100)/(min_adc-100) >= 0.2)
				hist[pmt]->GetXaxis()->SetRangeUser(min_adc-20,max_adc+130);
			leg[pmt] = new TLegend(0.4,0.70,0.9,0.9);
			leg[pmt]->SetNColumns(2);
			leg[pmt]->AddEntry(hist[pmt],Form("%s[%d]",detector[dets].c_str(),pmt),"l");
			leg[pmt]->AddEntry(F1[pmt],Form("Pedestal = %0.2f",F1[pmt]->GetParameter(1)),"l");
			leg[pmt]->Draw("same"); 
			hist[pmt]->GetXaxis()->SetTitle("raw adc");
			if(!debug){delete hist[pmt]; delete F1[pmt];delete F2[pmt];}
			pad++; //pad for drawing on canvas
			peaks=nullptr;
		}//End of PMT loop
	
		int new_z=0;
		while(pmt<34){
			oneD.push_back("0.0");
			oneD.push_back("0.0");
			pmt++;
			new_z++;
		}

		if(debug)cout<< "Added in " <<new_z <<" zeros for python script!!" <<"\n";						

		if(debug)cout<<" End of PMT loop "<<dets<<"\n";	
		ped_scan.push_back(oneD);
		if(debug)cout << "Push back "<< oneD[0]<<" "<< ped_scan[dets+1][0]<<"\n";
		
		oneD.clear();
		if(debug)cout<<"Cleared the oneD vector"<<"\n";

		//Add the canvas to a pdf
		for(int noc=0;noc<num_of_Cs;noc++){
			string opt="";
			if(noc==0&&num_of_Cs>1)opt="(";
			if(noc==(num_of_Cs-1)&&noc!=0)opt=")";
			//Add the run number to the canvas
			C[noc]->cd();
			TLatex t(0.2,0.95,Form("Run - %d", run));
			t.SetNDC(kTRUE);
			t.Draw(); 

			C[noc]->Print(Form("%s/Pedestals_%s_%d.pdf%s",image.c_str(),
						detector[dets].c_str(),run,opt.c_str()));
		}
		
		if(single_det>=0)break;
		if(debug){
			 for(int pmt=0; pmt<num_of_channels[dets];pmt++){
				 delete hist[pmt]; 	
				 delete F1[pmt];
				 delete F2[pmt];
				}
		}
		if(dets>20)break;	
		if(!debug||single_det==-1)for(int noc=0;noc<num_of_Cs;noc++){delete C[noc];}
	}//End of suffix loop of diff dets

	
	for(unsigned int k = 0; k<=detector.size();k++){
		for(unsigned int m=0;m<ped_scan[k].size();m++){
			output<<ped_scan[k][m]<<",";
			if(debug)cout<<ped_scan[k][m]<<",";
			}
		output<<"\n";
	}
		if(debug)cout<<"\nDrew "<<draw_cnt << " pmts total!"<<"\n";
  		time (&rawtime);
  		timeinfo = localtime (&rawtime);
	 	cout << asctime(timeinfo) << "\n";

	output.close();
	delete(Temp);
}//end of programemp)

