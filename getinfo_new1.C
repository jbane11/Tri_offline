//======================================
// get run info from rootfile -shujie
// !! Run with analyzer NOT root!!
//======================================

#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h> 
#include "rootalias1.h"

using namespace std;


void getinfo_new1(Int_t run=0){

  cout<<"Please run this code with ANALYZER instead of ROOT"<<endl;
  Int_t irun;
  // const TString rootfilePath = "/volatile/halla/triton/Tritium_Rootfiles/";
  if(run==0){
	  cout << "\nreplay: Please enter a Run Number (-1 to exit):";
  	  cin >> irun;
  	  }
  else{irun=run;}
  cout<<" ----The roofile paths are maintained in rootalias.h----"<<endl;
  if(irun==-1) return;
  
  TChain* T = LoadRun(irun,"T");
  TChain* TS;
  // cout<<"temp="<<temp<<endl;
  if (irun<20000)
     TS = LoadRun(irun,"TSLeft");
  else
      TS= LoadRun(irun,"TSRight");

  TChain* Epics=LoadRun(irun,"E"); 
 
  TArrayI  ps=GetPS(T);
 
  Double_t PS[8]; // is it neccessary to pass ps to PS?
  for (int k=0; k<8; k++){
    PS[k]= ps[k];
    //     cout<< " Prescaler = : "<< PS[k]<<endl;
  }
 
  TString targname=GetTarget(Epics);

  Double_t p0, angle,pos, pos2,ebeam,clk,dnew,dnew_current, dnew_r; 
  TString arm;
  Epics->SetBranchAddress("HALLA_p",&ebeam);
  Epics->SetBranchAddress("haBDSPOS",&pos);

  
  if(irun<20000) {
    //tree1->SetBranchAddress("evLeftLclock",&clk);
    T->SetBranchAddress("evLeftLclock",&clk);
    T->SetBranchAddress("evLeftdnew",&dnew);
    TS->SetBranchAddress("Leftdnew_r",&dnew_r);
    Epics->SetBranchAddress("HacL_alignAGL",&angle); 
    Epics->SetBranchAddress("HacL_D1_P0rb",&p0);

    arm="LHRS";

  }
  else {
    T->SetBranchAddress("evRightLclock",&clk);
    T->SetBranchAddress("evRightdnew",&dnew);
    TS->SetBranchAddress("Rightdnew_r",&dnew_r);
    Epics->SetBranchAddress("HacR_alignAGL",&angle);
    Epics->SetBranchAddress("HacR_D1_P0rb",&p0);
    arm="RHRS";
  }
 

  Double_t bcmdnew=0.000327;
  T->GetEntry(T->GetEntries()-1);
  cout<<"---------------\n";
  cout<<"Events          : " << T->GetEntries()<<endl;
  cout<<"Time            : " << clk*1.0/103700/60<<" minutes"<<endl;
  cout<<"Charge          : " << dnew * bcmdnew << " uC "<<endl;
  cout<<"Average Current : " <<(dnew * bcmdnew)/(clk*1.0/103700) <<" uA"<<endl;
  
  cout<<"line "<<__LINE__<<"      modification ends here-shujie  "<<endl;
  return;
  //------------------------------------
  //      modification ends here-shujie
  //-----------------------------------
  
  Int_t mm=Epics->GetEntries();
  Epics->GetEntry(mm-1);
  

  char *rate = new char[500];
  char *clkrate = new char[50];
  char hname[10][50];
  char *hh = new char[50];
  Double_t LT[10], DT[10];
  int icount[10];
  int daqcount[10];
  TH1F *his[10];
 
  int trig;
  TCut t_cut; 
  if(irun < 20000){trig=2;}else{trig=5;}  

  if(irun < 20000){t_cut = Form("DL.evtypebits&(1<<%i)",trig); sprintf(rate,"evLeftT%i", trig);}
  else{ t_cut = Form("DR.evtypebits&(1<<%i)",trig); sprintf(rate,"evRightT%i", trig);}
  icount[trig] = T->GetMaximum(rate);
  sprintf(hname[trig],"t%i",trig);
  if(irun < 20000){sprintf(hh,"DL.evtypebits>>%s", hname[trig]);}else{sprintf(hh,"DR.evtypebits>>%s", hname[trig]);}
  his[trig] =new TH1F (hname[trig], hname[trig], 100,0,1000000);
  T->Draw(hh,t_cut, "goff");
  daqcount[trig] = his[trig]->GetEntries();
  if(PS[trig-1]>0){
     LT[trig] = 100.*PS[trig-1]*daqcount[trig]/icount[trig];
     DT[trig] = 100. - LT[trig];
     //cout<<setw(12)<<i<<setw(12)<<PS[i-1]<<setw(18)<<daqcount[i]<<setw(18)<<icount[i]<<setw(18)<<DT[i]<<endl;
   }
   
  cout<<"---------------\n";
  cout<< "Target name              = "<<targname<<endl;
  cout<< "Target Encoder Position  = "<<pos<<endl;
  cout<< "Beam Energy              = "<<ebeam<<" GeV"<<endl;
  cout<< arm.Data()<<" p0                  = "<<p0<<" GeV"<<endl;
   cout<< arm.Data()<<" angle               = "<<angle<<" degree"<<endl;
  
  cout<< Form("Dead time for Trigger %i = ", trig) << DT[trig]<<endl;
  cout<< Form("Counts for Trigger %i    = ",trig)<<daqcount[trig]<<endl;
   
  Double_t current = 0;
  Int_t count = 0;
  Int_t ev = TS->GetEntries();
  
  for(Int_t j=0; j<ev; j++){
     TS->GetEntry(j);
     if(dnew_r>0){
        current += 0.00033*dnew_r;
        count++; 
     }
  } 
  
  current /= count;

  cout<< "Av. Current with beam    = " << current << " uA" <<endl;
  
     // Append a log file with one line that contains the reqired info for the wiki run log
   TString line ="<tr>";
    
   if(irun<20000){  
 		line += TString::Format("<td> %d </td>",irun);//Run number[s]
   		line += TString::Format("<td>%s</td>",targname.Data());//Target
   		line += TString::Format("<td>%0.2f</td>",current);//Current with beam on
  		line += TString::Format("<td>%0.2f</td>",(dnew * 0.00033)/(clk*1.0/103700));//Average Current of all the run
   		line += TString::Format("<td>%0.2f</td>",dnew * 0.00033);//Charge
   		line += TString::Format("<td>PS1=%d PS2=%d PS3=%d PS8=%d </td>",ps[0],ps[1],ps[2],ps[7]);//Prescale
   		line += TString::Format("<td>%0.2f</td>",DT[2]);//Deadtime Trigger (2)
   		line += TString::Format("<td>%0.2f</td>",(clk*1.0/103700)/60);//Run time (minutes)  		   		 
        line += TString::Format("<td>%d</td>",daqcount[2]);//total counts Trigger 2 		
   		line += TString::Format("<td></td>");//Comments
		line += "</tr>";
   }else{

                line += TString::Format("<td> %d </td>",irun);//Run number[s]
                line += TString::Format("<td>%s</td>",targname.Data());//Target
                line += TString::Format("<td>%0.2f</td>",current);//Current with beam on
                line += TString::Format("<td>%0.2f</td>",(dnew * 0.00033)/(clk*1.0/1024));//Average Current of all the run
                line += TString::Format("<td>%0.2f</td>",dnew * 0.00033);//Charge
                line += TString::Format("<td>PS4=%d PS5=%d PS6=%d PS8=%d </td>",ps[3],ps[4],ps[5],ps[7]);//Prescale
                line += TString::Format("<td>%0.2f</td>",DT[5]);//Deadtime Trigger (2) 
                line += TString::Format("<td>%0.2f</td>",(clk*1.0/1024)/60);//Run time (minutes)                               
                line += TString::Format("<td>%d</td>",daqcount[5]);//total counts Trigger 2             
                line += TString::Format("<td></td>");//Comments
                line += "</tr>";
  }
  std::fstream list;
  std::string line_file;
  list.open (Form("./wiki_runlist_%s.txt",arm.Data()), std::fstream::in); 
  std::string stringrun;std::string::size_type sz;
  int inrun=0;
  int status=0;
  while (std::getline(list, line_file))
  {
  for(int jj =9; jj<30;jj++){
   if(line_file[jj]== ' '){break;}
   stringrun+=line_file[jj];
  }	
  inrun=stoi(stringrun,&sz);
  size_t leng = stringrun.length();	 
  //if(leng == 0){ status=2;}
  if(inrun==irun){status=1; }
  stringrun="";
  }
  list.close();
  FILE* list2;
  list2 =fopen (Form("./wiki_runlist_%s.txt",arm.Data()),"a"); 
  if(status==0){
     fprintf(list2,"%s \n",line.Data());	
  }
  else{
    cout <<"Run already added to the list for the wiki!! " <<endl; 
  }
  fclose(list2);
  exit(1);
  return;
}
