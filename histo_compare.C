#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <TSystem.h>
#include <TString.h>
#include <TStyle.h>
#include <Riostream.h>
#include "TObjString.h"
#include <TNamed.h>
#include <TPRegexp.h>
#include <TObjArray.h>
#include <TChain.h>
#include <TMath.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TROOT.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TDatime.h>
#include <TError.h>
#include <TVirtualFitter.h>
#include <TSQLServer.h>
#include <TSQLResult.h>
#include <TSQLRow.h>
#include <TCut.h>
#include <TMultiGraph.h>
#include <TCutG.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TLatex.h>
#include <TRandom3.h>


using namespace std;

void histo_compare(TString histo_name = "",int run1=0, int run2=0){
 gStyle->SetOptStat(0);
if(histo_name==""){cout<<"Please enter the name of the histogram "<<endl;
				cout <<" L.tr.vz = tr.vz ";cin>>histo_name;}
if(run1==0){cout << "please enter the first run number" <<endl;cin>>run1;}
if(run2==0){cout << "please enter the second run number" <<endl;cin>>run2;}


	TString rootfilePath="/chafs1/work1/tritium/Rootfiles/";
 	
 	//==========this to take care the split rootfiles========================
   	TString file_name1 = TString::Format("%stritium_%d.root",rootfilePath.Data(),run1);
	TString basename1 = TString::Format("%stritium_%d",rootfilePath.Data(),run1);
	TString rootfile1 = basename1 + ".root";
    TChain* T1 = new TChain("T");
    Long_t jk=0;
    while ( !gSystem->AccessPathName(rootfile1.Data()) ) {
        T1->Add(rootfile1.Data());
        cout << "ROOT file " << run1<< "_"<< jk << " added to TChain." << endl; jk++;
        rootfile1 = basename1 + "_" + jk + ".root"; 
       // if(i>10){break;}
        }



   TFile *file = new TFile(Form("%stritium_%d.root",rootfilePath.Data(),run1),"read");
  if(file->IsZombie()){
    cout<<" this rootfile doest not exist: "<<endl;
    cout<<"Please try again with a new run. "<<endl;
    return;
  }
////////////////////////////////////////////////////////////////////////////////////////

 	//==========this to take care the split rootfiles========================
   	TString file_name2 = TString::Format("%stritium_%d.root",rootfilePath.Data(),run2);
	TString basename2 = TString::Format("%stritium_%d",rootfilePath.Data(),run2);
	TString rootfile2 = basename2 + ".root";
    TChain* T2 = new TChain("T");
    Long_t jk2=0;
    while ( !gSystem->AccessPathName(rootfile2.Data()) ) {
        T2->Add(rootfile2.Data());
        cout << "ROOT file " << run2<< "_"<< jk2 << " added to TChain." << endl; jk2++;
        rootfile2 = basename2 + "_" + jk2 + ".root"; 
       // if(i>10){break;}
        }



   TFile *file2 = new TFile(Form("%stritium_%d.root",rootfilePath.Data(),run2),"read");
  if(file->IsZombie()){
    cout<<" rootfile 2 doest not exist: "<<endl;
    cout<<"Please try again with a new run. "<<endl;
    return;
  }
////////////////////////////////////////////////////////////////////////////////////////
	
	TString ARM,arm;
	if(run1<90000){ARM="Left"; arm="L";}
			 else{ARM="Right";arm="R";}
			 
	TH1F *histo1 = new TH1F("histo1",Form("%s",histo_name.Data()), 1000000,-5000,5000);
	TH1F *histo2 = new TH1F("histo2",Form("%s",histo_name.Data()), 1000000,-5000,5000);
	
	cout << "Drawing " << histo_name <<endl;
	TCanvas *C = new TCanvas("C","comparison");
	T1->Draw(Form("%s.%s>>histo1",arm.Data(),histo_name.Data()),"","");
	T2->Draw(Form("%s.%s>>histo2",arm.Data(),histo_name.Data()),"","same");	
	histo2->SetLineColor(2);
	histo1->Scale(1/(histo1->GetEntries()));
	histo2->Scale(1/(histo2->GetEntries()));
	TLegend *L = new TLegend(0.6,0.6,0.9,0.9);
	L->AddEntry(histo1,Form("%s for %d",histo_name.Data(),run1),"l");
	L->AddEntry(histo2,Form("%s for %d",histo_name.Data(),run2),"l");
	L->Draw("same");
	histo1->GetXaxis()->SetTitle("tr.vz(mm)");
	//C->Modified();
	//C->Update();
	
	
			histo1->GetXaxis()->SetRangeUser(-0.35,0.35);
	double mean,min,max;
	int stop=0;
	int counter=0;
	/*while(stop==0){counter++;
	C->Modified()
		cout<< "Please adjust the range of the plot, by entering the min and max bin"<<endl;
		cout <<" min ";cin>>min; cout << "   max  " ;cin>>max; 
		cout<<endl;
		histo1->GetXaxis()->SetRangeUser(min,max);
		C->Modified();
		C->Update();
		cout<<" Does this look good? 0 =no: 1 = yes !! "<<endl;
		cin >> stop;
		if(counter==10){break;}
		
		}*/
	
	
	TCanvas *C1= new TCanvas("C1","Ratio_graph");	
	L->Draw("same");
	TRatioPlot *R = new TRatioPlot(histo1,histo2,"diff");
	R->Draw();
	R->GetLowerRefXaxis()->SetTitle("tr.vz(mm)");
	R->GetLowerRefGraph()->GetYaxis()->SetTitle("difference");
	R->GetUpperRefYaxis()->SetTitle("normalized to number of entries");
	R->SetUpBottomMargin(0.25); 
	R->SetLowTopMargin(0.75); 		
	R->SetSeparationMargin(0.0);
	TPad *upper = R->GetUpperPad();
	upper->cd();
	L->Draw("same");
	TLatex *txt = new TLatex(0.0, 0.03, Form("Difference between %d and %d",run1,run2));
	TPad *lower = R->GetLowerPad();
	lower->cd();
	txt->SetTextSize(0.1);
	txt->Draw("same");
	
			 
			 
			 
			 
			 
			 
 }
