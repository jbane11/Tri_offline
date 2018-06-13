

int which_kin(int run){

	ifstream input ("../../total_runlist.dat");

	string line;

	vector<string> tmp;
	int in_run=-10;
	int in_kin=0;
	int real_kin=-1;
	while(getline(input,line)){
		int pos;
		
		pos=line.find("\t");
		tmp.push_back(line.substr(0,pos));
		pos=line.find_last_of("_");
		tmp.push_back(line.substr(pos+1,line.size()));
		in_run=stoi(tmp[0]);
		in_kin=stoi(tmp[1]);
			
		tmp.clear();

		if(run==in_run){
			cout<< " kin =" <<in_kin<<endl;
			real_kin=in_kin;
			break;}
		}
		
return real_kin;
}
	
		
		
		
