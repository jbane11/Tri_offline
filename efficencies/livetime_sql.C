#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"



void livetime_sql(int runnum,  int debug=1)
{
  ///These will be added as I get more into the DB
  string cols ="trigger_counts, trigger_events";

  if(!SQLCheck(runnum)) exit(1);
  double correction=1.0;
  correction_error=0.0;

  CODASetting   coda     = GetCODASetting(runnum);
  AnalysisInfo  runinfo;
  TSQLServer*  	Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TString 	query    = Form("select %s from %sanalysis where run_number=%d",cols.c_str(), coda.experiment.Data(),runnum);
  TSQLResult*   result   = Server->Query(query.Data());


  TSQLRow *row = result->Next();
 /////I can Add these in as I get more corrections into the DB
  int count = result->GetFieldCount();

  if(count>=1)
  {
     //Livetime
     correction = atof(row->GetField(1));
     correction /= atof(row->GetField(0));
     cout<< correction<<endl;
     TString update = Form("UPDATE %sanalysis SET livetime= %f where run_number = %d;", coda.experiment.Data(),correction, runnum);
     Server->Query(update.Data());
     update = Form("UPDATE %sanalysis SET livetime_eff= %f where run_number = %d;", coda.experiment.Data(),correction, runnum);
     Server->Query(update.Data());
     Server->Close();
   }
}
