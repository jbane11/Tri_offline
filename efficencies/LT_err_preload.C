void LT_err_preload(int run,int debug=1)
{
  gROOT->LoadMacro("livetime_err_sql_C.so");
  gROOT->ProcessLine(Form("livetime_err_sql(%d,%d)", run,debug));
}
