//////////////////////////////////////
// This script will be used to run 
// the ped scan script in the batch system.
// This script will run the already compiled
// version of ped scan. This script will need
// the run number!!
/////////////////////////////////////////


void batch_ped_scan(int run, int debug=0){

	gROOT->LoadMacro("ped_scan_C.so");
	gROOT->ProcessLine(Form("ped_scan(%d,%d)",run,debug));
}
