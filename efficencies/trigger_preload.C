void trigger_preload(int run,int debug=0)
{
  gROOT->LoadMacro("trigger_eff_C.so");
  gROOT->ProcessLine(Form("trigger_sql(%d,%d)",run,debug));
}
