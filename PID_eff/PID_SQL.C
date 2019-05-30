#include "./PID_eff_fun.C"

void PID_SQL(int runnum =0,int debug=0)
{
  double PID_sh_eff_sql=1;
  double PID_sh_err_sql=0;
  double PID_ps_eff_sql=1;
  double PID_ps_err_sql=0;
  double PID_cer_eff_sql=1;
  double PID_cer_err_sql=0;
  if(!SQLCheck(runnum)) exit(1);


  CODASetting  coda = GetCODASetting(runnum);
  AnalysisInfo Ainfo= GetAnalysisInfo(runnum);

  if(Ainfo.status == -1){cout <<"Not in analysis DB \n"; exit(1);}

  TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(), mysql_user.Data(), mysql_password.Data());

  TString query_check_ps = Form("select PID_ps_eff, PID_ps_err  from %sanalysis where run_number = %d;", coda.experiment.Data(), runnum);
  TSQLResult* result_ps  = Server->Query(query_check_ps.Data());


  if(result_ps->GetFieldCount()!=0)
  {
    TSQLRow *row = result_ps->Next();
    if(row->GetField(0)==nullptr)PID_ps_eff_sql=1;
    else{PID_ps_eff_sql = atof(row->GetField(0));}
    if(row->GetField(0)==nullptr)PID_ps_err_sql=1;
    else{PID_ps_err_sql = atof(row->GetField(0));}
  }

  TString query_check_sh = Form("select PID_sh_eff, PID_sh_err  from %sanalysis where run_number = %d;", coda.experiment.Data(), runnum);
  TSQLResult* result_sh  = Server->Query(query_check_sh.Data());


  if(result_sh->GetFieldCount()!=0)
  {
    TSQLRow *row = result_sh->Next();
    if(row->GetField(0)==nullptr)PID_sh_eff_sql=1;
    else{PID_sh_eff_sql = atof(row->GetField(0));}
    if(row->GetField(0)==nullptr)PID_sh_err_sql=1;
    else{PID_sh_err_sql = atof(row->GetField(0));}
  }



  TString query_check_cer = Form("select PID_cer_eff, PID_cer_err from %sanalysis where run_number = %d;", coda.experiment.Data(), runnum);
  TSQLResult* result_cer  = Server->Query(query_check_cer.Data());
  if(result_cer->GetFieldCount()!=0)
  {
    TSQLRow *row = result_cer->Next();
    if(row->GetField(0)==nullptr)PID_cer_eff_sql=1;
    else{PID_cer_eff_sql = atof(row->GetField(0));}
    if(row->GetField(0)==nullptr)PID_cer_err_sql=1;
    else{PID_cer_err_sql = atof(row->GetField(0));}
  }

	cout << Ainfo.kin <<"  " <<Ainfo.target<<endl;
  string kin = Form("%s",Ainfo.kin.Data());
  string target = Form("%s",Ainfo.target.Data());
  string tgt = shortTgtName(target);

  if(debug) cout << kin <<"  "<< tgt<<endl;
  vector<double> PID_vec= PID_read_csv(kin,tgt);

  vector<double> sh_eff; sh_eff.push_back(PID_vec[2]);
  sh_eff.push_back(PID_vec[3]);
  vector<double> ps_eff; ps_eff.push_back(PID_vec[0]);
  ps_eff.push_back(PID_vec[1]);
  vector<double> Cer_eff; Cer_eff.push_back(PID_vec[4]);
  Cer_eff.push_back(PID_vec[5]);
  vector<double> NE_eff; NE_eff.push_back(PID_vec[6]);
  NE_eff.push_back(PID_vec[7]);


  if(debug)
  {
    cout << "Do you want to add to SQL" <<endl;
    int check=0;
    cin >>check;
    if(check!=1){ Server->Close(); return;}
  }

  if(sh_eff[0]!=PID_sh_eff_sql || sh_eff[1]!=PID_sh_err_sql)
  {
    TString update = Form("UPDATE %sanalysis SET PID_sh_eff = %f, PID_sh_err= %f where run_number = %d;", coda.experiment.Data(),sh_eff[0],sh_eff[1], runnum);
    Server->Query(update.Data());

  }

  if(ps_eff[0]!=PID_ps_eff_sql || ps_eff[1]!=PID_ps_err_sql )
  {
    TString update = Form("UPDATE %sanalysis SET PID_ps_eff = %f, PID_ps_err= %f where run_number = %d;", coda.experiment.Data(),ps_eff[0],ps_eff[1], runnum);
    Server->Query(update.Data());

  }

  if(Cer_eff[0]!=PID_cer_eff_sql||Cer_eff[1]!=PID_cer_err_sql)
  {
    TString update = Form("UPDATE %sanalysis SET PID_cer_eff = %f, PID_cer_err= %f where run_number = %d;", coda.experiment.Data(),Cer_eff[0],Cer_eff[1], runnum);
    Server->Query(update.Data());

  }
  TString update = Form("UPDATE %sanalysis SET PID_NE_eff = %f, PID_NE_err= %f where run_number = %d;", coda.experiment.Data(),NE_eff[0],NE_eff[1], runnum);
  Server->Query(update.Data());



  //Close the server
  Server->Close();
}
