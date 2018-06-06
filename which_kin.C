

void which_kin(int run){

	ifstream input ("total_runlist.dat");

	string line;

	vector<string> tmp;
	int in_run=0;
	int in_kin=0;
	while(input.getline(line);){
		int pos;
		while(pos=line.find("\t") !=string::npos){
			tmp.push_back(line.substr(0,pos));
		}
		cout << tmp[0]<<" " <<tmp[1] <<endl;	
	}
}	
		
		
		
