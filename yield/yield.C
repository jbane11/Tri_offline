#include "../headers/inc1.h"
#include "../headers/rootalias1.h"
#include "../header/TRI_Main.h"
#include "../headers/TRI_Tools.h"


void yield (string kin_name="", int debug=1);

/////////////////////////////////////////////////////////////////////////////////////////
	string kin_dir = "/work/halla/triton/Runlist/"
	//Input the kin file incase was not given in argument
	if(kin_name==""){
		cout << "Please enter the kinematic file, that you want to use!"<<endl;
		cout << "example (\"He3_kin7\")"<<endl;
		cin >> kin_name;
	} 
	kin_name = kin_dir+kin_name+".dat";
	///Open the kin file using the function kin_file
	kin_file KFile;
	KFile.set_file(kin_name,debug);
	//This vector contains all of the runs for this target at this kin.
	vector<int> run_vector = KFile.run_vec; 
/////////////////////////////////////////////////////////////////////////////////////////

	corrections C_T;
	C_T.Read_Table(KFile.target,KFile.kin_num,1);
	
	cout<< C_T.CT[2][2] <<endl;




