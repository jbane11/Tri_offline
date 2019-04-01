void CY_preload(string tgt,string kin,int bins,int clean, int model,int OR, int tarid, int debug){


        gROOT->LoadMacro("MCyield_test_C.so");
        gROOT->ProcessLine(Form("MCyield_test(\"%s\",\"%s\",%d,%d,%d,%d,%d,%d)",tgt.c_str(),kin.c_str(),bins,clean,model,OR,tarid,debug));



}
