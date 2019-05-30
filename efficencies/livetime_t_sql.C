#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"



void livetime_t_sql(int runnum,  int debug=1)
{

  CODASetting   coda     = GetCODASetting(runnum);
  TChain *T = LoadRun(runnum);

  double raw = T->GetMaximum("evLeftT2");

  double trig_ev = T->GetEntries(L_mara_trig);

  double LT = trig_ev/raw;

  AnalysisInfo  runinfo;
  TSQLServer*  	Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  TString 	query    = Form("select * from %sanalysis where run_number=%d", coda.experiment.Data(),runnum);
  TSQLResult*   result   = Server->Query(query.Data());
  TSQLRow *row = result->Next();
 /////I can Add these in as I get more corrections into the DB
  int count = result->GetFieldCount();

  if(count>=1)
  {
     //Livetime
     cout << LT <<endl;
     TString update = Form("UPDATE %sanalysis SET livetime= %f where run_number = %d;", coda.experiment.Data(),LT, runnum);
     Server->Query(update.Data());
     Server->Close();
   }
}
