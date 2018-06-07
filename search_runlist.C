#include "./headers/inc1.h"
#include "./headers/rootalias1.h"
#include "./headers/TRI_Main.h"
#include "./headers/TRI_Tools.h"
#include <sys/types.h> 
#include <dirent.h> 
void search_runlist(string Arm ="L",int debug=0){

	int first_run=1000, last_run=3000;
        string ARM="",arm="";
	/*if(Arm=="L"){
                ARM="Left";
                arm="l";
                first_run =1000;
                last_run  =5000;
                }
        else {
                ARM="Right";
                arm="r";
                first_run =90000;
                last_run  =99000;
                }
*/
	vector<kin_file> Kins;
	DIR *dir = opendir("./Runlist");
	if(dir)
	{	int counter=0;
		struct dirent *ent;
		while((ent = readdir(dir)) != NULL)
		{
			string file_name = ent->d_name;
			if(file_name.find("~")<=(file_name.length()+2)) continue;
			if(file_name.find("SS")<=(file_name.length()+2)) continue;
			if(file_name.find(".bck")<=(file_name.length()+2)) continue;
			if(file_name.find("ww")<=(file_name.length()+2)) continue;
			if(file_name.find("TEST")<=(file_name.length()+2)) continue;
			if(file_name.find(".dat")>=(file_name.length()+2)) continue;
			if(debug)cout <<" Searching : "<< file_name<< "  ";
			kin_file tmp;

			file_name = "./Runlist/"+file_name;
			if(debug)cout << file_name<<endl;
			tmp.set_file(file_name,debug);
			if(tmp.run_file_status==1){
				Kins.push_back(tmp);}
			counter++;

		}
	
	}
	closedir(dir);

	std::ofstream file("total_runlist.dat",ios::out);
//	FILE * file =fopen("total_runlist.dat","w");	
//	 setvbuf ( file , NULL , _IOFBF ,56 );
	vector<<int>> in_runs;
	for(int i =0; i<Kins.size();i++)
	{
		vector<int> tmp_runs=gGet_RunNoChain(Kins[i].run_string);
		in_runs.push_back(tmp_runs);
		tmp_runs.clear();
	}
	

	std::vector<int>::iterator f_pos;
//	char *buffer = new char[256];
//	int buff=0;
	for(int run=first_run; run <= last_run; run++)
	{
		if(run/100==run/100.0){cout <<"  : " <<endl;cin>>arm;if(arm=="~")break;}		
		string runs = to_string(run);
		runs=runs;
		
		for(int i =0; i<Kins.size();i++)
		{
			//in_runs.clear();
			//in_runs=gGet_RunNoChain(Kins[i].run_string);
//			cout <<in_runs.size()<<"  ";		
			f_pos = std::find(in_runs[i].begin(), in_runs[i].end(), run);
		
			if(f_pos != in_runs[i].end()){
				string kin1 = Kins[i].target+"_"+Kins[i].kin_num;
			cout<<run <<" " << f_pos - in_runs[i].begin()<<" " <<kin1 <<endl;
			file <<run<<"\t"<<kin1<<endl;
			file.flush();
			
		

			//	fprintf(file,"%d\t%s",run,kin1.c_str());
			
			break;
			}	
		}//cout <<endl<<endl;
	}
	file.close();
//	fclose(file);

}//end of program	



