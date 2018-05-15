void batch_pid_scan(TString filename ="")
{
        gROOT->ProcessLine(Form("PID_eff_scan(\"%s\")",filename.Data()));

}

