void preload_cal_scan(string kin,string tgt,string ver, int delta){


	gROOT->LoadMacro("Eff_scan_cal_C.so");
	gROOT->ProcessLine( Form("Eff_scan_cal(\"%s\",\"%s\",\"%s\",%d)", kin.c_str(), tgt.c_str(), ver.c_str(),delta));



}
