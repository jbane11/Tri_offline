#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"



void livetime_err_sql(int runnum,  int debug=1)
{
  ///These will be added as I get more into the DB
  string cols ="livetime, trigger_counts, trigger_events";

  if(!SQLCheck(runnum)) exit(1);
  double correction=1.0;
  double correction_error=0.0;

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
     if(row->GetField(0)!=nullptr){
       correction *= atof(row->GetField(0));
       correction_error = Binomial_Error(correction,atoi(row->GetField(1)));
     }
     else{
       correction =1;
       correction_error = 0.0;
      }
     TString update = Form("UPDATE %sanalysis SET livetime_err= %f where run_number = %d;", coda.experiment.Data(),correction_error, runnum);
     Server->Query(update.Data());

   }
    Server->Close();
}
