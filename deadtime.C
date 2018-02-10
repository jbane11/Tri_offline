//======================================
// DeadTime Calculation for Tritium run
// Sheren Alsalmi &  Dien Nguyen
// Nov, 8, 2017 
//======================================


void deadtime(Int_t run=0){


  Int_t irun;
  const TString rootfilePath = "/chafs1/work1/tritium/Rootfiles/";
  int PS[8];
  char *rate = new char[500];
  char *clkrate = new char[50];
  char hname[10][50];
  char *h = new char[50];
  Double_t LT[10], DT[10];
  int icount[10];
  int daqcount[10];
  TH1F *his[10];
  TTree *tree;

  if(run==0){
	  cout << "\nreplay: Please enter a Run Number (-1 to exit):";
  	  cin >> irun;
  	  }
  else{irun=run;}
  
  if(irun==-1) return;

  
  TFile *file = new TFile(Form("%stritium_%d.root",rootfilePath.Data(),irun),"read");
  tree = (TTree*)file->Get("T");
  if(file->IsZombie()){
    cout<<" this rootfile doest not exist: "<<endl;
    cout<<"Please try again with a new run. "<<endl;
    return;
  }

  THaRun *aRun = (THaRun*)file->Get("Run_Data");
  TArrayI ps = aRun->GetParameters()->GetPrescales();

  for (int k=0; k<8; k++){
    PS[k]= ps[k];
    //cout<< " Prescaler = : "<< PS[k]<<endl;
  }

  cout<<"Run # "<<"   "<<irun<<endl; 
  cout<<"     |  Trigger |   Prescale |  Recorded Trigger |  Raw Trigger |   Deadtime (%)\n";
  
  int i=0;
  
  if (irun>80000){
   	
    for (i=4; i<7; i++){
      TCut t_cut = Form("DR.evtypebits&(1<<%i)",i);
      sprintf(rate,"evRightT%i", i);
      icount[i] = tree->GetMaximum(rate);
      sprintf(hname[i],"t%i",i);
      sprintf(h,"DR.evtypebits>>%s", hname[i]);
      his[i] =new TH1F (hname[i], hname[i], 100,0,1000000);
      tree->Draw(h,t_cut, "goff");
      daqcount[i] = his[i]->GetEntries();

      //cout<<"DAQ counts "<<daqcount[i]<<endl;
      if(PS[i-1]>0){
	LT[i] = 100.*PS[i-1]*daqcount[i]/icount[i];
	DT[i] = 100. - LT[i];
      }
      cout<<setw(12)<<i<<setw(12)<<PS[i-1]<<setw(18)<<daqcount[i]<<setw(18)<<icount[i]<<setw(18)<<DT[i]<<endl;
    }

    // Clock DeadTime :
    //================
    TCut t8_cut = Form("DR.evtypebits&(1<<8)");
    i=8;
    sprintf(clkrate,"evRightLclock");     
    icount[8] = tree->GetMaximum(clkrate);
    sprintf(hname[8],"t8");
    sprintf(h,"DR.evtypebits>>%s", hname[8]);
    his[8] =new TH1F (hname[8], hname[8], 100,0,1000000);
    tree->Draw(h,t8_cut, "goff");
    daqcount[8] = his[8]->GetEntries();
    if(PS[i-1]>0){
      LT[8] = 100.*PS[7]*daqcount[8]/icount[8];
      DT[8] = 100. - LT[8]; 
     
      cout<<setw(12)<<i<<setw(12)<<PS[i-1]<<setw(18)<<daqcount[i]<<setw(18)<<icount[i]<<setw(18)<<DT[i]<<endl;
    
    }
  }

  else{
  
    for (i=1; i<4; i++){
      TCut t_cut = Form("DL.evtypebits&(1<<%i)",i);
      sprintf(rate,"evLeftT%i", i);
      icount[i] = tree->GetMaximum(rate);
      sprintf(hname[i],"t%i",i);
      sprintf(h,"DL.evtypebits>>%s", hname[i]);
      his[i] =new TH1F (hname[i], hname[i], 100,0,1000000);
      tree->Draw(h,t_cut, "goff");
      daqcount[i] = his[i]->GetEntries();
      if(PS[i-1]>0){
	LT[i] = 100.*PS[i-1]*daqcount[i]/icount[i];
	DT[i] = 100. - LT[i];
	cout<<setw(12)<<i<<setw(12)<<PS[i-1]<<setw(18)<<daqcount[i]<<setw(18)<<icount[i]<<setw(18)<<DT[i]<<endl;
      }
    }

    // Clock DeadTime :
    //================
    TCut t8_cut = Form("DL.evtypebits&(1<<8)");
    i=8;
    sprintf(clkrate,"evLeftLclock");     
    icount[8] = tree->GetMaximum(clkrate);
    sprintf(hname[8],"t8");
    sprintf(h,"DL.evtypebits>>%s", hname[8]);
    his[8] =new TH1F (hname[8], hname[8], 100,0,1000000);
    tree->Draw(h,t8_cut, "goff");
    daqcount[8] = his[8]->GetEntries();
    if(PS[i-1]>0){
      LT[8] = 100.*PS[7]*daqcount[8]/icount[8];
      DT[8] = 100. - LT[8]; 
      cout<<setw(12)<<i<<setw(12)<<PS[i-1]<<setw(18)<<daqcount[i]<<setw(18)<<icount[i]<<setw(18)<<DT[i]<<endl;
    }	   
  }
  exit(1);
}
