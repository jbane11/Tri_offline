#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"


void checktgt(int runnum, int debug=1){

	CODASetting    coda     = GetCODASetting(runnum);
	AnalysisInfo  Ainfo	= GetAnalysisInfo(runnum);
	if(Ainfo.status == -1){cout <<"Not in analysis DB \n"; exit(1);}
	TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
	TString        query    = Form("select target from %srunlist where run_number=%d", coda.experiment.Data(),runnum);
  	TSQLResult*    result   = Server->Query(query.Data());
	if(result->GetRowCount()==0) {cout << "Run " << runnum<< "  not found!!"<<"\n"; exit(1);}	
	TSQLRow *row = result->Next();
	string sql_tgt = row->GetField(0);

	TargetInfo tgt_info = GetTarget(runnum);
	string run_tgt = Form("%s",tgt_info.name.Data());
		
	if(debug){ cout<< "From SQL\trun \n";
		cout <<sql_tgt << "  \t"<< run_tgt <<endl;}

	if(sql_tgt==run_tgt){cout<< "Great: NEXT!!!\t"; Server->Close();exit(1);}
	else{
		/////Update the runlist!!!
		TString update = Form("Update %srunlist SET target = '%s' where run_number = %d;",coda.experiment.Data(),run_tgt.c_str(),runnum);
		Server->Query(update.Data());
	}
	Server->Close();
	





	



}
