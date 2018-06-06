#include "./headers/inc1.h"
#include "./headers/rootalias1.h"
#include <sys/types.h> 
#include <dirent.h> 
void search_runlist(string Arm ="L",int debug=1){

	int first_run=0, last_run=0;
        string ARM="",arm="";
	if(Arm=="L"){
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
	vector <vector<string>> found;	
	std::ofstream file("total_runlist.dat");
	
	vector <string> tmp_v;
	int did_f=0;
	for(int run=first_run; run <= last_run; run++)
	{
		
		string runs = to_string(run);
		runs=","+runs;
		for(int i =0; i<Kins.size();i++)
		{
			int ff=-1; ff = Kins[i].run_string.find(runs);
			int lf=-1; lf = Kins[i].run_string.length();
			if(ff>-1)
			{
				string kin = Kins[i].target+"_"+Kins[i].kin_num;
				did_f++;
			cout << "Found :" <<endl;	
			cout << Kins[i].run_string <<endl<<endl;
			cout << ff<<"  "<< lf<<endl;
		//		if(debug) cout << ff<<" "<< lf <<endl;		
				tmp_v.push_back(runs);
				tmp_v.push_back(kin);
		//		if(debug) cout << runs << "\t" << kin <<endl;
				file << run<< "\t" << kin <<endl;
				found.push_back(tmp_v);
			
				tmp_v.clear();
			}
		}
		cout << did_f<<endl;
		did_f=0;	
	}	
	file.close();


}//end of program	



