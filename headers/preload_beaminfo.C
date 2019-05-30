void preload_beaminfo(int run, int debug){
  gROOT->LoadMacro("beampos_info_C.so");
  gROOT->ProcessLine( Form("beampos_info(%d,%d)",run,debug));
}
