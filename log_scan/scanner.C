#include "../headers/inc1.h"
#include "../headers/rootalias1.h"
#include "../headers/TRI_Main.h"
#include "../headers/TRI_Tools.h"



void scanner(int start_run=920, int end_run=930){


	TString log_loc = "/adaqfs/home/a-onl/tritium/replay/t2root/Rootfiles/log";
	std::ofstream output;
	output.open("log_scan.log", std::ofstream::out);

	//Keywords for skipping - more of theses, speeds up code;
	vector<string> skip_words = {"replay:","**","  * ", "-----","<OpenFile()>","rootlogon","typo"};
	//Keywords for IDing issues
	vector<string> concerns = {"warning","Warning","WARNING","ERROR","error","Error"};
	vector<string> errors;
	vector< vector<string>> err_vec;
	//Keywords for the type of issues: also will tilt the output for that type of statement
	vector<string> error_type= {"Other","Scaler","overflow",
		"Output","out of bounds","Resolution lock",
		"Decoder","CODA"};
	vector<string> input_vec;

	std::ofstream diff_e[10];
	for(int d_e=0;d_e<error_type.size();d_e++){
		diff_e[d_e].open(Form("output/%s.log",error_type[d_e].c_str()),std::ofstream::out);}


	string oldline;
	int old_counter;
	for(int i = start_run;i<end_run;i++){

		vector<int> err_typ_count;
		for(int j=0;j<error_type.size();j++){err_typ_count.push_back(0);}	
		old_counter=0;
		TString log_name = Form("%s/%d.log",log_loc.Data(),i);
		std::ifstream log_file;
		log_file.open(Form("%s",log_name.Data()),std::ifstream::in);
		string input_line;
		
		std::getline(log_file,input_line);
		int no_run = -1;
		no_run=input_line.find("skipped");
		if(no_run>=0){continue;}
		if(log_file.good()){
			output<<"_________________"<<endl;
			output<<log_name<<endl;
		}
		int skip=0;
		int s_found=-1;
		cout << i <<endl;
		int line_counter=0;
		while(log_file.good() ){
			vector<string> er; 
			for(int j=0;j<error_type.size();j++){er.push_back("");}	
			skip=0; s_found=-1;
			std::getline(log_file,input_line);
			input_vec.push_back(input_line);
			line_counter++;
			if(is_number(input_line)){continue;}
			for(int i_s=0;i_s<skip_words.size();i_s++){
				s_found=input_line.find(skip_words[i_s]);
				if(s_found>=0){skip=1;break;}
			}
			if(skip==1){continue;}
			
			int c_found=-1;
			int print=0;
			
			for(int c_i=0;c_i<concerns.size();c_i++){
				c_found=input_line.find(concerns[c_i]);
				if(c_found>=0){print=1;
					errors.push_back(input_line);	
					break;}
			}
			
			if(print==1){
			//	cout << input_line<<endl;
				output << input_line<<endl;
				int e_type=-1; int e_slec=0;
				for(int d_e=0;d_e<error_type.size();d_e++){
					e_type=input_line.find(error_type[d_e]);
					if(e_type>=0){e_slec=d_e;
					err_typ_count[e_slec]++;
					er[e_slec]=input_line;}				
					if(d_e==error_type.size()){
						e_slec=0;
						err_typ_count[0]++;
						er[0]=input_line;}	
				}
				
				if(err_typ_count[e_slec]<=1){
					diff_e[e_slec]<<log_name<<endl;}
				diff_e[e_slec]<<"\t"<<input_line<<endl;

			} //end of print
		int eof =-1;
	//	eof=input_line.find("update fullreplay");
		if(eof>=0)break;	
		if(oldline==input_line){old_counter++;}
		if(old_counter>=5){break;}
		oldline=input_line;
		er.clear();
		}//End of one file
	
		err_typ_count.clear();
		log_file.close();
		input_vec.clear();	
	}
	
	for(int d_e=0;d_e<error_type.size();d_e++){
		diff_e[d_e].close();}
	output.close();
	

}	
