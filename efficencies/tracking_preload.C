void tracking_preload(int run,int debug=1)
{
  gROOT->LoadMacro("Tracking_eff_C.so");
  gROOT->ProcessLine(Form("tracking_sql(%d,%d)",run,debug));


}
