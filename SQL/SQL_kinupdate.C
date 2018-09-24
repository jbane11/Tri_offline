
#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"

//This script will update the kinematic setting column of the SQL marathon analysis  SQL DB

void SQL_kinupdate(string kinfile)
{ 
	
	int pos_flag=0;
	kin_file Kfile;
	if(kinfile.find("/Runlist")<kinfile.length()){
		Kfile.set_file(Form("%s",kinfile.c_str()));
		while(int pos1 =kinfile.find("/")<kinfile.length())
		{
			kinfile.erase(0,pos1+1);
		}
		if(kinfile.find("p")<kinfile.length())pos_flag=1;
	
		kinfile.erase(0,kinfile.find("kin")+3);
		kinfile.erase(kinfile.find(".dat"),kinfile.length()-1);
	}
	else{
		Kfile.set_file(Form("/home/jbane/tritium/Runlist/%s.dat",kinfile.c_str()));
		int pos2 = kinfile.find("kin");
		kinfile.erase(0,pos2+3);
	}
	vector<int>runlist=Kfile.run_vect;
	if(Kfile.run_file_status==0){exit(1);}

	TString kin_name =Form("%s",kinfile.c_str());
	if(pos_flag==1)kin_name+="_pos";
	cout << kin_name<<endl;
	

	TString mysql_connection = "mysql://halladb/triton-work";
	TString mysql_user       = "triton-user";
	TString mysql_password   = "3He3Hdata";


	TSQLServer* Server = TSQLServer::Connect(mysql_connection.Data(),mysql_user.Data(),mysql_password.Data());
	TString query;


	for (int i =0; i<runlist.size();i++)
	{		int run = runlist[i];
			cout <<"Updating run " << run << " with kin " <<kin_name<<" !!"<<"\n";
		query = Form("update MARATHONrunlist set Kinematic='%s' where run_number = %d",kin_name.Data(),run);
		Server->Query(query.Data());
		query = Form("update MARATHONanalysis set Kinematic='%s' where run_number = %d",kin_name.Data(),run);
		Server->Query(query.Data());
	}


	Server->Close();


}
	

