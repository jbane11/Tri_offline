#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"

void beamcharge(int runnum, int cut=0, int debug=1){

	CODASetting    coda     = GetCODASetting(runnum);
	AnalysisInfo  Ainfo	= GetAnalysisInfo(runnum);
	if(Ainfo.status == -1){cout <<"Not in analysis DB \n"; exit(1);}
	TSQLServer*    Server   = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
	TString        query    = Form("select * from %schargelist where run_number=%d;", coda.experiment.Data(),runnum);
  	TSQLResult*    result   = Server->Query(query.Data());
	TSQLResult* mod;

	string field_name=Form("charge%d",cut);
	string current_name=Form("current%d",cut);
	TSQLRow *row;

	double current;
	double charge = CalcCharge1(runnum,current,cut*1.0);
	if(charge!=charge) charge = CalcChargeold(runnum,current,cut*1.0);
	if(debug) cout<<"Charge : " << charge<<"  Current " << current <<"\n";
	
	int found=0;
	if(result->GetRowCount()>0) 
	{
	//Run is there -> need to modify or add a new column
		if(debug) cout << "Run Found "<<endl;
		row = result->Next();
		for( int i=0; i<result->GetFieldCount();i++)
		{
			if( result->GetFieldName(i) == field_name )
			{//The correct column already exist
				if(debug) cout << "column found " <<endl;
				query = Form("UPDATE %schargelist set %s = %f, %s = %f WHERE run_number=%d;",coda.experiment.Data(),field_name.c_str(),charge,current_name.c_str(),current,runnum);

				mod =Server->Query(query.Data());

				found=1;		
				break;
			}
		}
			
		if(found!=1)
		{//Run is there, but correct column is not make the columns first.
			if(debug) cout <<"column not found "<<endl; 
		query= Form("ALTER TABLE %schargelist ADD %s float(7);",coda.experiment.Data(),field_name.c_str());
			mod =Server->Query(query.Data());
			
		query= Form("ALTER TABLE %schargelist ADD %s float(5);",coda.experiment.Data(),current_name.c_str());
			mod =Server->Query(query.Data());
			
			query = Form("UPDATE %schargelist set %s = %f, %s = %f WHERE run_number=%d;",coda.experiment.Data(),field_name.c_str(),charge,current_name.c_str(),current,runnum);
			mod =Server->Query(query.Data());
			
		}

	}
	else {
	//Run is not there need 
		if(debug) cout << "run not found "<<endl;
		//Need to find if the column exist.
		query= Form("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME ='%schargelist';",coda.experiment.Data());
		cout <<query <<endl;
		TSQLResult* search = Server->Query(query.Data());
		
		//make sure that the search works
		if(search->GetRowCount()>0)
	        {
			for( int i=0; i<search->GetRowCount();i++)
	                {
				row = search->Next();
				cout << row->GetField(0) <<endl;
				if( row->GetField(0) == field_name )
	                        {//The correct column already exist
					if(debug) cout << "column found " <<endl; 
					query = Form("INSERT INTO %schargelist (run_number,%s, %s) VALUES (%d,%f, %f) ;",coda.experiment.Data(),field_name.c_str(),current_name.c_str(),runnum,charge,current);
					mod =Server->Query(query.Data());
					found=1;
					break;
				}						
			}
			if(found!=1)
			{//COlumn was not found
				if(debug) cout << "column not found "<<endl;
				query= Form("ALTER TABLE %schargelist ADD %s float(7);"
					,coda.experiment.Data(),field_name.c_str());
                        	mod =Server->Query(query.Data());
		                query= Form("ALTER TABLE %schargelist ADD %s float(5);"
					,coda.experiment.Data(),current_name.c_str());
                	        mod =Server->Query(query.Data());		
				query=Form("INSERT INTO %schargelist (%s, %s) VALUES (%f, %f) ;",
					coda.experiment.Data(),field_name.c_str(),current_name.c_str(),charge,current);
                                mod =Server->Query(query.Data());
			}
		}
	}	
	if(debug) cout << query<<endl;
	Server->Close();
}
