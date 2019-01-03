#include "./PID_eff.C"

void PID_SQL(int runnum =0,int debug=0)
{
  double PID_cal_eff_sql=1;
  double PID_cal_err_sql=0;
  double PID_cer_eff_sql=1;
  double PID_cer_err_sql=0;

  CODASetting  coda = GetCODASetting(runnum);
  AnalysisInfo Ainfo= GetAnalysisInfo(runnum);

  if(Ainfo.status == -1){cout <<"Not in analysis DB \n"; exit(1);}

  TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(), mysql_user.Data(), mysql_password.Data());

  TString query_check_cal = Form("select cal_eff, cal_eff_err  from %sanalysis where run_number = %d;", coda.experiment.Data(), runnum);
  TSQLResult* result_cal  = Server->Query(query_check_cal.Data());


  if(result_cal->GetFieldCount()!=0)
  {
    TSQLRow *row = result_cal->Next();
    if(row->GetField(0)==nullptr)PID_cal_eff_sql=1;
    else{PID_cal_eff_sql = atof(row->GetField(0));}
    if(row->GetField(0)==nullptr)PID_cal_err_sql=1;
    else{PID_cal_err_sql = atof(row->GetField(0));}
  }

  TString query_check_cer = Form("select cer_eff, cer_eff_err from %sanalysis where run_number = %d;", coda.experiment.Data(), runnum);
  TSQLResult* result_cer  = Server->Query(query_check_cer.Data());
  if(result_cer->GetFieldCount()!=0)
  {
    TSQLRow *row = result_cer->Next();
    if(row->GetField(0)==nullptr)PID_cer_eff_sql=1;
    else{PID_cer_eff_sql = atof(row->GetField(0));}
    if(row->GetField(0)==nullptr)PID_cer_err_sql=1;
    else{PID_cer_err_sql = atof(row->GetField(0));}
  }

  vector<double> Cal_eff = Cal_eff_run(runnum,debug);
  vector<double> Cer_eff = Cer_eff_run(runnum,debug);

  if(debug)
  {
    cout << "Do you want to add to SQL" <<endl;
    int check=0;
    cin >>check;
    if(check!=1){ Server->Close(); return;} 
  }

  if(Cal_eff[0]!=PID_cal_eff_sql)
  {
    TString update = Form("UPDATE %sanalysis SET cal_eff = %f, cal_eff_err= %f where run_number = %d;", coda.experiment.Data(),Cal_eff[0],Cal_eff[1], runnum);
    Server->Query(update.Data());

  }
  if(Cer_eff[0]!=PID_cer_eff_sql)
  {
    TString update = Form("UPDATE %sanalysis SET cer_eff = %f, cer_eff_err= %f where run_number = %d;", coda.experiment.Data(),Cer_eff[0],Cer_eff[1], runnum);
    Server->Query(update.Data());

  }

  //Close the server
  Server->Close();
}
