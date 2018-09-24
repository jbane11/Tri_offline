#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"



void PS_SQL(int run, int debug=1){

	CODASetting   coda     = GetCODASetting(run);
	if(coda.experiment=="unknown")exit(1);
	RunInfo RI = GetRunInfo(run);
	if(RI.runnum==0)exit(1);
	TChain *T = LoadRun(run);
	if(T==nullptr){exit(1);}
	int ps_main =GetPS(T,coda.bit);
	if( RI.PS_main == ps_main){
		cout << "Prescales match! Done!\n"; exit(1);
		}
	cout<< "SQL main PS = "<< RI.PS_main <<"   root file PS_main = " <<ps_main<<"\n";
	TSQLServer*   Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
	TString   Query = Form("UPDATE %srunlist SET prescale_T%d = %d where run_number = %d;", coda.experiment.Data(),coda.bit,ps_main,run);
	Server->Query(Query.Data());
	
	Server->Close();
}
