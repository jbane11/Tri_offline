void CY_preload(string tgt,string kin,int wt,string suf,int debug){
	

	gROOT->LoadMacro("CalcYield_test_C.so");
	gROOT->ProcessLine(Form("CalcYield_test(\"%s\",\"%s\",%d,\"%s\",%d)",tgt.c_str(),kin.c_str(),wt, suf.c_str(),debug));



}
