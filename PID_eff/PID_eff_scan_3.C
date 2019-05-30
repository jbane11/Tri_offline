#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"




void PID_eff_scan_3(string kin="1",string tgt="H3")
{

  vector<int> runlist = SQL_Kin_Target(kin,tgt);
  if(runlist.size()==0)exit(1);
  TChain *T = new TChain("T");
  for(unsigned int i =0; i<runlist.size();i++)
  {
    T->Add(LoadRun(runlist[i]));
    int total_ent = T->GetEntries();
    if(total_ent>=1000000)break;
    if(i>=10){break;}
  }
  CODASetting CS = GetCODASetting(runlist[0]);
  string ARM=Form("%s",CS.arm.Data());



  int cutnum=25;

  double first_bin_prl1 = 0.0;
  double last_bin_prl1  = 1.5;
  double step_size_prl1 = abs(first_bin_prl1-last_bin_prl1)/cutnum*1.0;

  double first_bin_prl2 = 0.0;
  double last_bin_prl2  = 1.5;
  double step_size_prl2 = abs(first_bin_prl2-last_bin_prl2)/cutnum*1.0;

  double first_bin_cer = 500.0;
  double last_bin_cer  = 4500.0;
  double step_size_cer = abs(first_bin_cer-last_bin_cer)/cutnum*1.0;

  double prl1_ele_sample_min=1.5;
  double prl1_ele_sample_max=2.5;
  double prl1_non_ele_sample_min=0.0;
  double prl1_non_ele_sample_max=0.3;

  double ele_min_lim =5500;
  double ele_max_lim =6500;
  double pion_min_lim=0;
  double pion_max_lim=800;

  TCut prl1_ele_sample_cut = Form("(L.prl1.e/1000.)>=%f&&(L.prl1.e/1000.)<=%f", prl1_ele_sample_min, prl1_ele_sample_max);
  TCut prl1_ele_non_sample_cut = Form("(L.prl1.e/1000.)>=%f&&(L.prl1.e/1000.)<=%f", prl1_non_ele_sample_min, prl1_non_ele_sample_max);

  string cer_cut="L.cer.asum_c";

  TCut cer_ele_sample_cut = Form( "(%s>%f&&%s<%f)", cer_cut.c_str(), ele_min_lim, cer_cut.c_str(),ele_max_lim);
  TCut cer_non_ele_sample_cut = Form("(%s>%f&&%s<%f)", cer_cut.c_str(), pion_min_lim, cer_cut.c_str(), pion_max_lim);

  TCut prl2_ele_sample_cut = Form("(L.prl2.e/1000. >1.0)&&(L.prl2.e/1000.<1.5)" );
  TCut prl2_ele_non_sample_cut =Form("((L.prl2.e)/1000. <0.1)");

  TCut tote_ele_sample_cut= Form("((L.prl2.e+L.prl1.e)/1000.>2.5)");
  TCut tote_ele_non_sample_cut= Form("((L.prl2.e+L.prl1.e)/1000.<1.0)");


  TCut basic_c = acc_cut_L + beta_cut_L+L_mara_trig;

  ofstream outfile;
  outfile.open(Form("./images/cal_layer_%s_%s.csv",tgt.c_str(),kin.c_str()));

  double prl1_sample_ele     = T->GetEntries(basic_c+ prl2_ele_sample_cut+ cer_ele_sample_cut);
  double prl1_sample_non_ele = T->GetEntries(basic_c+ prl2_ele_non_sample_cut+ cer_non_ele_sample_cut);


  double cer_sample_ele     = T->GetEntries(basic_c+ tote_ele_sample_cut);
  double cer_sample_non_ele = T->GetEntries(basic_c+ tote_ele_non_sample_cut);

  double prl2_sample_ele     = T->GetEntries(basic_c+ prl1_ele_sample_cut+ cer_ele_sample_cut);
  double prl2_sample_non_ele = T->GetEntries(basic_c+ prl1_ele_non_sample_cut+ cer_non_ele_sample_cut);



cout << prl1_sample_ele << "  " <<prl1_sample_non_ele <<endl;


  double cut_levelprl1=first_bin_prl1;
  double cut_levelprl2=first_bin_prl2;
  double cut_levelcer =first_bin_cer;

  outfile << "Cut_prl2" <<","<<" prl2_ele_pass" <<","<< "prl2_ele_eff"<<","<< "prl2_ele_eff_err"<<","<< "prl2_nele_eff"<<","<< "prl2_nele_err" <<","<<"Cut_prl1" <<","<< "prl1_ele_pass" <<","<< "prl1_ele_eff" <<","<< "prl1_ele_eff_err"<<","<< "prl1_nele_eff"<<","<< "prl1_nele_err"  <<","<<"Cut_cer" <<","<< "cer_ele_pass" <<","<< "cer_ele_eff" <<","<< "cer_ele_eff_err"<<","<< "cer_nele_eff"<<","<< "cer_nele_err" <<"\n";

  cout << "starting scan"<<endl;
  for(int i=0; i<cutnum;i++)
  {
    TCut prl2_cut = Form("(L.prl2.e/1000.0 >%g)",cut_levelprl1);
    TCut prl1_cut = Form("(L.prl1.e/1000.0 >%g)",cut_levelprl2);
    TCut cer_cut = Form("(L.cer.asum_c >%g)",cut_levelcer);

    double prl2_ele_pass  = T->GetEntries(basic_c+ prl1_ele_sample_cut+ cer_ele_sample_cut+  prl2_cut);
    double prl2_non_ele_pass= T->GetEntries(basic_c+ prl1_ele_non_sample_cut+ cer_non_ele_sample_cut+ prl2_cut );

    double prl1_ele_pass  = T->GetEntries(basic_c+ prl2_ele_sample_cut+ prl1_cut+ cer_ele_sample_cut);
    double prl1_non_ele_pass= T->GetEntries(basic_c+ prl2_ele_non_sample_cut+ prl1_cut+ cer_non_ele_sample_cut);

    double cer_ele_pass  = T->GetEntries(basic_c+ tote_ele_sample_cut+ cer_cut);
    double cer_non_ele_pass= T->GetEntries(basic_c+ tote_ele_non_sample_cut+ cer_cut );

    double prl2_e_eff = prl2_ele_pass/prl2_sample_ele;
    double prl1_e_eff = prl1_ele_pass/prl1_sample_ele;
    double cer_e_eff = cer_ele_pass/cer_sample_ele;

    double prl2_ele_eff_err = Bino_Err(prl2_e_eff,prl2_sample_ele);
    double prl1_ele_eff_err = Bino_Err(prl1_e_eff,prl1_sample_ele);
    double cer_ele_eff_err  = Bino_Err( cer_e_eff,cer_sample_ele);

    double prl2_ne_eff = (prl2_sample_non_ele-prl2_non_ele_pass)/prl2_sample_non_ele;
    double prl1_ne_eff = (prl1_sample_non_ele-prl1_non_ele_pass)/prl1_sample_non_ele;
    double cer_ne_eff = (cer_sample_non_ele-cer_non_ele_pass)/cer_sample_non_ele;

    double prl2_ne_err = Bino_Err(prl2_e_eff,prl2_sample_ele);
    double prl1_ne_err = Bino_Err(prl1_e_eff,prl1_sample_ele);
    double cer_ne_err  = Bino_Err( cer_e_eff,cer_sample_ele);

outfile<<cut_levelprl2<<","<<prl2_ele_pass<<","<<prl2_e_eff<<","<< prl2_ele_eff_err<<","<< prl2_ne_eff<<","<< prl2_ne_err <<","<<cut_levelprl1 <<","<< prl1_ele_pass <<","<< prl1_e_eff <<","<< prl1_ele_eff_err<<","<< prl1_ne_eff<<","<< prl2_ne_err <<","<<cut_levelcer <<","<< cer_ele_pass <<","<< cer_e_eff <<","<< cer_ele_eff_err<<","<< cer_ne_eff<<","<< cer_ne_err <<"\n";
cout << i+1 <<" out of "<<cutnum<<"\n";

        cut_levelprl2+=step_size_prl2;
        cut_levelprl1+=step_size_prl1;
        cut_levelcer+=step_size_cer;

  }

  outfile.close();
}
