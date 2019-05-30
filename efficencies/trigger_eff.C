#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"



vector<double> trigger_eff(string kin, string tgt="H3", int debug=0)
{
  vector<double> blank ={0.0,0.0};
  vector<int> runlist = SQL_Kin_Target(kin,tgt);

  if(runlist.size()==0)return blank ;
  TChain *T = new TChain("T");
  for(unsigned int i =0; i<runlist.size();i++)
  {
    T->Add(LoadRun(runlist[i]));
    int total_ent = T->GetEntries();
    if(total_ent>=5000000)break;
    if(i>=40){break;}
  }


  TCut goodE = track_L+acc_cut_L+layers_electron_cut_L;

  TCut T2 = L_mara_trig;
  TCut T3 = "DL.evtypebits&(1<<3)";
  TCut T1 = "DL.evtypebits&(1<<1)";

  double T2_GE_events = T->GetEntries(goodE+T2);
  double T1_GE_events = T->GetEntries(goodE+T1);
  double trig_eff = T2_GE_events/T1_GE_events;
  double trig_err = Bino_Err(trig_eff,T1_GE_events);

  if(debug) cout << "sample " << T1_GE_events<< "  T2_events" << T2_GE_events <<endl;

  vector <double> trig_effV={trig_eff, trig_err};
  return trig_effV;
}


void trigger_eff_csv(string tgt="H3",int debug=1)
{
  ofstream output; output.open(Form("./trigger_eff_%s.cvs",tgt.c_str()));
	output<<"kin,Trigger_eff,Trigger_err\n";
  for( int i=0;i<16;i++)
  {
    vector <double> trig_eff = trigger_eff(Form("%d",i),tgt,debug);
    output<<i<<","<<trig_eff[0]<<","<<trig_eff[1]<<"\n";
  }
  output.close();
}


vector<double> read_trigger_csv(string kin, string tgt="")
{
  ifstream input; input.open(Form("./trigger_eff_%s.cvs",tgt.c_str()));
  vector<double> trig_eff={1.0,0.0};
  string line;

  if(!input.is_open()){cout << "File issues " <<endl;return trig_eff;}
  vector <double> invec;
  getline(input,line);

  while(input.good())
  {
    getline(input,line);
    invec = parse_csv_d(line,',');
    if( invec[0] == stod(kin)) break;
    invec.clear();
  }
  trig_eff[0]=invec[1];
  trig_eff[1]=invec[2];

  return trig_eff;
}

void trigger_sql(int runnum, int debug=0)
{
  if(debug) cout << "Run " << runnum <<endl;
  if(!SQLCheck(runnum)) exit(1);
  CODASetting  coda = GetCODASetting(runnum);
  AnalysisInfo Ainfo= GetAnalysisInfo(runnum);
  if(Ainfo.status == -1){cout <<"Not in analysis DB \n"; exit(1);}

  TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(), mysql_user.Data(), mysql_password.Data());

  TString query_check = Form("select Trigger_eff, Trigger_err from %sanalysis where run_number = %d;", coda.experiment.Data(), runnum);

  TSQLResult* result  = Server->Query(query_check.Data());



  double eff=1.0; double err=0.0;
  if(result->GetFieldCount()!=0)
  {
    TSQLRow *row = result->Next();
    if(row->GetField(0)!=nullptr) eff=atof(row->GetField(0));
    else eff=1.0001;
    if(row->GetField(1)!=nullptr) err=atof(row->GetField(1));
    else err =0.0;
  }

  string kin = Form("%s",Ainfo.kin.Data());
  string target = Form("%s",Ainfo.target.Data());
  string tgt = shortTgtName(target);

  vector<double> eff_vec= read_trigger_csv(kin,tgt);

  if(eff_vec[0]!=eff)
  {
    cout << "check" <<endl;
    TString update = Form("UPDATE %sanalysis SET Trigger_eff = %f, Trigger_err= %f where run_number = %d;", coda.experiment.Data(),eff_vec[0],eff_vec[1], runnum);
    Server->Query(update.Data());
  }

  Server->Close();
}
