//Float_t Get


Double_t Calculate_Charge(Int_t runNN);
Float_t Get_CHARGE_dnew(Int_t runNo);
Float_t Get_DEAD_TIME(Int_t runNo);

void MARATHON_NORM()
{ //cout<<"PLEASE enter a run no!!"<<endl;
 // Int_t run_number;
  Float_t dead_time;
  Float_t charge_dnew; 

  Float_t C1;
  Double_t C2; 
  Double_t Total_CHARGE=0;
  Float_t Total_charge=0;
  


   vector<Int_t> runList;
   Int_t runNo = -1;
   Int_t kin = -1;

   cout<<"!!! If you don't know how to use it, read README "<<endl;
   TString filename;
   cout<<"Input file name:   "; cin>>filename;

   filename = "./Runlist/" + filename;
   ifstream file(filename.Data());
   if(!file.is_open()){cout<<"!!! file not found "<<endl;exit(0);}
   TString content;
   TString Targettype;

   if(content.ReadToken(file))Targettype = content;
   else {cout<<"! ! No target type"<<endl;exit(0);}
   if(content.ReadToken(file))kin = atoi(content);
   else {cout<<"! ! No Kin"<<endl;exit(0);}
 
   TString test; 
   Ssiz_t from=0;
   if(test.ReadLine(file)){
      while(test.Tokenize(content,from,","))
       {
            runNo = atoi(content);
            cout<<runNo<<",";
            if(runNo>0)runList.push_back(runNo);

       }

    }

   file.close();
  cout<<endl;


 //dead_time=Get_DEAD_TIME(run_number);
 //charge_dnew=Get_CHARGE_dnew(run_number);
 

  //vector<Int_t> runList;
  //Int_t runNo=1;
  //while(runNo>0)
  //{ cout<<"please add a run number to the Run List(-1 to finish)"<<endl;
  //  cin>>runNo;
  //  if (runNo>0)
  // { runList.push_back(runNo);}}

for(Int_t j=0;j<runList.size();j++)
 {  Int_t rr=runList[j];
      C1=Get_CHARGE_dnew(rr); 
      C2=Calculate_Charge(rr);
  if (C1>0) {Total_charge+=C1;}
  if  (C2>0) {Total_CHARGE+=C2;}
}

Float_t Get_CHARGE_dnew(Int_t runNo);
cout<<"******************************************"<<endl;
cout<<"TOTAL EPICS CHARGE ="<<Total_charge<<"uC"<<endl;
cout<<"TOTAL CALCULATED CHARGE ="<<Total_CHARGE<<"uC"<<endl;

}

Float_t Get_DEAD_TIME(Int_t runNo)

{ TString epics_name=Form("/adaqfs/home/adaq/epics/runfiles_tritium_L/halog_end_%d.epics",runNo);
 ifstream *infile=new ifstream(epics_name);
 cout<<epics_name<<endl;
 string line;
 string word[7];
 Float_t DT;
 while(getline(*infile,line)){
    istringstream str(line);
    str>>word[0]>>word[1]>>word[2]>>word[3];
    if(word[0]=="TS"&&word[1]=="DEAD"&&word[2]=="TIME:")
    {// word[3].pop_back();
    //  word[3].erase(word[3].end()-1);
     DT=atof(word[3].c_str());}
    
}
      return DT;}

Float_t Get_CHARGE_dnew(Int_t runNo)

{ TString epics_name=Form("/adaqfs/home/adaq/epics/runfiles_tritium_L/halog_end_%d.epics",runNo);
 ifstream *infile=new ifstream(epics_name);
 cout<<epics_name<<endl;
 string line;
 string word[7];
 Float_t Charge=-1;
 while(getline(*infile,line)){
    istringstream str(line);
    str>>word[0]>>word[1]>>word[2]>>word[3]>>word[4];
    if(word[0]=="Unew:"&&word[2]=="uC"&&word[3]=="Dnew:")
          {Charge=atof(word[4].c_str());}
    
           }

if(Charge>0) {cout<<"For run "<<runNo<< ", the Charge from EPICS file is "<<Charge<<"uC"<<endl;}
else {cout<<"For run "<<runNo<<",EPICS file cannot be found"<<endl;}
                 return  Charge;}
    



 
Double_t Calculate_Charge(Int_t runNN)
{

TChain *TS=new TChain("TSLeft");
//TS->Add(Form("/lustre/expphy/volatile/halla/triton/Tritium_Rootfiles/tritium_%d*.root",runNN));
TS->Add(Form("/chafs1/work1/tritium/Rootfiles/tritium_%d*.root",runNN));
Double_t Dnew_count;
Double_t D3_count;
Double_t CL_count;
Double_t Ev_No;
Double_t CL_count_previous=0.0;
Double_t Dnew_count_previous=0.0;
Double_t D3_count_previous=0.0;
Double_t Time;
Double_t BCM_dnew_count;
Double_t BCM_d3_count;
Double_t CHARGE=-1.0;
Double_t CHARGE_d3=-1.0;
const Double_t clock_rate=103700.0;
const Double_t coef1=0.0003264;
const Double_t coef2=0.1055;
const Double_t coef3=0.0001063;
const Double_t coef4=0.1985;
 TS->SetBranchStatus("*",0);
 TS->SetBranchStatus("Leftdnew",1);
 TS->SetBranchStatus("Leftd3",1);
 TS->SetBranchStatus("LeftLclock",1);
 TS->SetBranchAddress("LeftLclock",&CL_count);
 TS->SetBranchAddress("Leftdnew",&Dnew_count);
 TS->SetBranchAddress("Leftd3",&D3_count);

Ev_No=TS->GetEntries();
//cout<<Ev_No<<endl;
for(Int_t index=0;index<Ev_No;index++)
{ TS->GetEntry(index);
  
 Time+=(CL_count-CL_count_previous)/clock_rate;
 BCM_dnew_count+=Dnew_count-Dnew_count_previous;
 BCM_d3_count+=D3_count-D3_count_previous;
//cout<<"HAHA2="<<BCM_dnew_count<<endl;
//cout<<"TIME="<<(CL_count-CL_count_previous)/clock_rate<<endl; 
 CL_count_previous=CL_count;
 Dnew_count_previous=Dnew_count;
 D3_count_previous=D3_count;
}

CHARGE=coef1*BCM_dnew_count+coef2*Time*1e-6;
CHARGE_d3=coef3*BCM_d3_count+coef4*Time*1e-6;

//cout<<"HAHA="<<CHARGE<<endl;
if(CHARGE>0) {cout<<"For run "<<runNN<< ", the Charge from rootfile  is "<<CHARGE<<"uC"<<endl;}
else {cout<<"For run "<<runNN<<", charge is not calculated correct, please check!!!"<<endl;}
delete TS;
return CHARGE;

}Float_t Get_CHARGE_dnew(Int_t runNo);



