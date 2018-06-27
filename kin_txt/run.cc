

void run(TString filename ="", int ow = 1, int debug =0)
{
	gROOT->LoadMacro("Corrections_table_C.so");
	gROOT->ProcessLine(Form("Corrections_table(\"%s\",%d,%d)",filename.Data(),ow,debug));

}
