#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"


void run_status(int runnum, int debug=1){
	int junk=0; //Set to not junk!
	CODASetting    coda     = GetCODASetting(runnum);
	TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
	AnalysisInfo   Ainfo    = GetAnalysisInfo(runnum);
	
      	TString Searchrunlist = Form("select run_number, run_type, time_mins, presacle_T%d from %srunlist where run_numbers= %d ;",coda.bit,coda.experiment.Data(),runnum);
	TSQLResult *result = Server->Query(Searchrunlist.Data());
	if(result->GetRowCount()==0){
		cout <<"Not in runlist\n";
		exit(0);
		}


	
	if(Ainfo.status==){cout <<"Not in analysis DB\n"; exit(0);}

	if(junk){
		cout <<"Removing Run " << runnum << Form(" from %sanalysis\n ", coda.experiment.Data());
		TString remove = Form(Form"DELETE FROM %sanalysis where run_number =%d;"coda.experiment.Data(),runnum);
		Server->Query(remove.Data();
	}

  	Server->Close();
}
