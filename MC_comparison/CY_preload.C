void CY_preload(string tgt,string kin,string suf,int bins,int clean, int model, int debug){


        gROOT->LoadMacro("MCyield_test_C.so");
        gROOT->ProcessLine(Form("MCyield_test(\"%s\",\"%s\",\"%s\",%d,%d,%d,%d)",tgt.c_str(),kin.c_str(),suf.c_str(),bins,clean,model,debug));



}
