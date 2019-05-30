#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"




void SQL_PSupdate(int run)
{




	TString mysql_connection = "mysql://halladb/triton-work";
        TString mysql_user       = "triton-user";
        TString mysql_password   = "3He3Hdata";


        TSQLServer* Server = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());

	int SQLps2;
	int SQLps5;
	TChain *T = LoadRun(run);
	if(T==nullptr) exit(1);
	int Rps2 = GetPS(T,2);
	int Rps5 = GetPS(T,5);

	TString query = Form("select prescale_T1 ,prescale_T2 ,prescale_T3, prescale_T4, prescale_T5,prescale_T6, prescale_T7, prescale_T8 from MARATHONrunlist where run_number = %d",run);

	TSQLResult*    result   = Server->Query(query.Data());
	if(result->GetRowCount()==0) {cout << "Run " << run<< "  not found!!"<<"\n"; exit(1);}

	TSQLRow *row = result->Next();
	if(row->GetField(1)!=nullptr)SQLps2=atoi(row->GetField(1));
	else SQLps2=-1; 
	if(row->GetField(4)!=nullptr)SQLps5=atoi(row->GetField(4));
	else SQLps5=-1; 

	if(SQLps2 == -1 || SQLps5 == -1 || SQLps2!=Rps2 || SQLps5!=Rps5){
		cout << "Updateing PS, PS2= " <<Rps2<<"   PS5= "<< Rps5<<endl;	
		TString update = Form("Update MARATHONrunlist SET prescale_T2 = %d,  prescale_T5=%d where run_number=%d",Rps2,Rps5,run);
		Server->Query(update.Data());
	}
	else cout<<"Good\n\n";
        Server->Close();
}
