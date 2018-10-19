void CY_preload(string tgt,string kin,int wt, int debug){
	

	gROOT->LoadMacro("CalcYield_test_C.so");
	gROOT->ProcessLine(Form("CalcYield_test(\"%s\",\"%s\",%d,%d)",tgt.c_str(),kin.c_str(),wt,debug));



}
