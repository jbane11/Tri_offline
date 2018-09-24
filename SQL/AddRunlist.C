#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"



void AddRunlist(int run, int debug=1){

	int DC = SQLRunlistStatus(run);
	if(DC==1){cout << run <<" is already in the runlist, would you like to update it? (y=, n=0)\n";
		int check=0;
		cin >> check;
		if(check==0){exit(1);
	}

	CODASetting   coda     = GetCODASetting(runnum);
	TChain *T = LoadRun(run);
	
	
	




 
  TSQLServer*   Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
  Server->Close();
