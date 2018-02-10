#include "TRI_Main.h" //Used for th RunNoChain to Tchain all of the TTrees together
#include "TRI_Tools.h" //Used for th RunNoChain to Tchain all of the TTrees together

void DT_kin( TString filename=""){

	//input the name opf the kin file and parse it
	if(filename==""){ cout<<"Input file name:   "; cin>>filename;}
	filename = "/adaqfs/home/a-onl/tritium_work/Runlist/" + filename;
    ifstream file(filename.Data());
    if(!file.good()){cout << filename.Data() << " does not exist! "<<endl; exit(1);}
    TString content;
    TString Target,Kin,Run_String;
    const char* kin;
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
   ///Call tri_tools functions and combine root files
    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);

	TString rootfilePath="/chafs1/work1/tritium/Rootfiles/";
	
	TVector DT_1(1);	TVector DT_2(1);	TVector DT_3(1);	
	TVector DT_4(1);	TVector DT_5(1);	TVector DT_6(1);
	TVector RunList(1); 
	
	vector<string>  labels;

	Int_t num_of_runs=0;
	int first_run = RunNoChain[0];
	int last_run = RunNoChain[RunNoChain.size()-1];

//Start of each run
	for(int i = 0; i<RunNoChain.size()-1;i++){
		int run=RunNoChain[i]; 
				
		cout << "This is the start of run " << run <<endl;
	
	//==========this to take care the split rootfiles==============================================
	   	TString file_name = TString::Format("%stritium_%d.root",rootfilePath.Data(),run);
		TString basename = TString::Format("%stritium_%d",rootfilePath.Data(),run);
		TString rootfile = basename + ".root";

    	TChain* T = new TChain("T");
   	 	Long_t jk=0;
    	while ( !gSystem->AccessPathName(rootfile.Data()) ) {
        	T->Add(rootfile.Data());
        	cout << "ROOT file " << run<< "_"<< jk << " added to TChain." << endl; jk++;
        	rootfile = basename + "_" + jk + ".root"; 
       		// if(i>10){break;}
        	}
		TFile *file = new TFile(Form("%stritium_%d.root",rootfilePath.Data(),run),"read");
		//Make Sure this file is replayed!!!!
	  	if(file->IsZombie()){
	  		TString string_run_file=TString::Format("%stritium_%d.root",rootfilePath.Data(),run);
	    	cout<< string_run_file<<" doest not exist: "<<endl;
	    	cout<<"Skipping this file. "<<endl;
	    	continue;
	  		}
	//add one elemnt to the vecotors	  		
  		DT_1.ResizeTo(num_of_runs+1);	DT_2.ResizeTo(num_of_runs+1);
  		DT_3.ResizeTo(num_of_runs+1);	DT_4.ResizeTo(num_of_runs+1);
  		DT_5.ResizeTo(num_of_runs+1);	DT_6.ResizeTo(num_of_runs+1);
  		RunList.ResizeTo(num_of_runs+1);
  		
  		labels.push_back(std::to_string(run));
        	
	////////////////////////////////////////////////////////////////////////////////////////
	//============Defineing which arm/////////////////////////////
		TString ARM,arm;
		if(run<90000){ARM="Left"; arm="L";}
			 else{ARM="Right";arm="R";}
	//============Defineing which arm/////////////////////////////


	///////////////////////////Calculate deadtime;
  		THaRun *aRun = (THaRun*)file->Get("Run_Data");
  		THaRunParameters *para=aRun->GetParameters();
  		TArrayI ps = aRun->GetParameters()->GetPrescales();	
	
		char *rate = new char[500];
		char *clkrate = new char[50];
  		char hname[10][50];
  		char *hh = new char[50];
  		Double_t LT[10], DT[10];
  		int icount[10];
  		int daqcount[10];
  		TH1F *his[10];
  		double PS[8];
   	
    	for (int k=0; k<8; k++){PS[k]= ps[k];}
	
    	for (int i=1; i<7; i++){
    	  TCut t_cut = Form("D%s.evtypebits&(1<<%i)",arm.Data(),i);
    	  sprintf(rate,"ev%sT%i",ARM.Data(), i);
    	  icount[i] = T->GetMaximum(rate);
    	  sprintf(hname[i],"t%i",i);
    	  sprintf(hh,"D%s.evtypebits>>%s", arm.Data(),hname[i]);
    	  his[i] =new TH1F (hname[i], hname[i], 100,0,1000000);
    	  T->Draw(hh,t_cut, "goff");
    	  daqcount[i] = his[i]->GetEntries();
    	  if(PS[i-1]>0){
			LT[i] = 100.*PS[i-1]*daqcount[i]/icount[i];
			DT[i] = 100. - LT[i];
    	  }
    	}
	///////////////////////////Calculate deadtime;
		
		DT_1(num_of_runs)=DT[1];	DT_2(num_of_runs)=DT[2];
		DT_3(num_of_runs)=DT[3];	DT_4(num_of_runs)=DT[4];
		DT_5(num_of_runs)=DT[5];	DT_6(num_of_runs)=DT[6];
  
  		RunList(num_of_runs)=run;
		int entries= T->GetEntries();
		cout << "Run number : " << run << "  DT 1  " << DT_1(num_of_runs) <<endl;
		cout << "With " << entries << " number of entries. "<<endl;
	
		num_of_runs++;//iterate the number of runs!!!
		
				cout << "This is the end of run " << run <<endl;
				cout << "We have seen "<< num_of_runs<< " runs! "<<endl;
	}	
//End of each run			

	int color[7] = {1,1,632,600,432,418,807};
	int marker_style[7] = {1,33,34,39,41,43,45};
		
	gStyle->SetOptStat(0);
	TCanvas *C = new TCanvas("C", "DT runlist");  
		
//Make a TGraph and plot the results!!!
	TMultiGraph *DT_graph = new TMultiGraph();
		TGraph *DT_g_1 = new TGraph(RunList,DT_1);
		TGraph *DT_g_2 = new TGraph(RunList,DT_2);
		TGraph *DT_g_3 = new TGraph(RunList,DT_3);
		TGraph *DT_g_4 = new TGraph(RunList,DT_4);
		TGraph *DT_g_5 = new TGraph(RunList,DT_5);
		TGraph *DT_g_6 = new TGraph(RunList,DT_6);
if(RunList[1]<90000){
	DT_graph->Add(DT_g_1);
	DT_g_1->SetMarkerColor(color[1]);DT_g_1->SetMarkerStyle(marker_style[1]);
	DT_g_1->SetMarkerSize(2.0);
	DT_graph->Add(DT_g_2);
	DT_g_2->SetMarkerColor(color[2]);DT_g_2->SetMarkerStyle(marker_style[2]);
	DT_g_2->SetMarkerSize(1.5);
	DT_graph->Add(DT_g_3);
	DT_g_3->SetMarkerColor(color[3]);DT_g_3->SetMarkerStyle(marker_style[3]);
	DT_g_3->SetMarkerSize(1.0);
	}
else{
	DT_graph->Add(DT_g_4);
	DT_g_4->SetMarkerColor(color[4]);DT_g_4->SetMarkerStyle(marker_style[4]);
	DT_g_4->SetMarkerSize(2.0);
	DT_graph->Add(DT_g_5);
	DT_g_5->SetMarkerColor(color[5]);DT_g_5->SetMarkerStyle(marker_style[5]);
	DT_g_5->SetMarkerSize(1.5);
	DT_graph->Add(DT_g_6);
	DT_g_6->SetMarkerColor(color[6]);DT_g_6->SetMarkerStyle(marker_style[6]);
	DT_g_6->SetMarkerSize(1.0);
	}
	DT_graph->Draw("ap");
	DT_graph->GetXaxis()->SetTitle("Run Number");
	DT_graph->GetYaxis()->SetRangeUser(0,5);
	DT_graph->GetYaxis()->SetTitle("DT in %");

	TLegend *leg = new TLegend(0.1,0.9,0.9,1.0);
	leg->SetNColumns(3);
if(RunList[1]<90000){
	leg->AddEntry(DT_g_1,"DT for T1", "p");	
	leg->AddEntry(DT_g_2,"DT for T2", "p");	
	leg->AddEntry(DT_g_3,"DT for T3", "p");	
	}
else{
	leg->AddEntry(DT_g_4,"DT for T4", "p");	
	leg->AddEntry(DT_g_5,"DT for T5", "p");	
	leg->AddEntry(DT_g_6,"DT for T6", "p");	
	}
	leg->Draw("same");

	
	TText *t = new TText();
   	t->SetTextAlign(32);
   	t->SetTextSize(0.035);
   	t->SetTextFont(72);
	t->SetTextAngle(90);
   	const char *label_Array[100];
   	for (Int_t j=0;j<num_of_runs;j++) {
   	  label_Array[j]= labels[j].c_str();
      //t->DrawText(RunList[j],-0.00,label_Array[j]);
   	}

		
		
		

}
