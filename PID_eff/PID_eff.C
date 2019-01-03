#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"


//Calculate the pid eff for the Caloremters using ttree
vector<double> Cal_eff_tree(TChain *T, double cut=0.0, int debug=1)
{
  //Return vector
  vector<double> Cal_eff;
  //get info from tree
  THaRun* RunData = GetRunData(T);
  int run = RunData->GetNumber();
  //Get arm and a few other bits
  CODASetting coda = GetCODASetting(run);
  //Predefine varibles for L/R
  string det_bdy ="";
  string det_pre =Form("%s",coda.arm.Data());
  string det_suf ="e";
  double pion_max = 750;
  double pion_min = 250;
  double ele_min  = 4500;
  double ele_max  = 7500;

  //cut made in caloremter
  TCut pid_cut ="1";
  double calhit=0.1;
  TCut pion_sel="1";
  TCut ele_sel ="1";
  //Cuts made in acc,tracking, and cherenkov
  TCut Good_Ele = "1";

  if(coda.arm=="L")//Left arm- Set detector name, cut value and GE selection
    {
      Good_Ele =th_cut_L+ph_cut_L+track_L+beta_cut_L;
      pion_sel =Form("L.cer.asum_c<%g && L.cer.asum_c<%g ",pion_max,pion_min);
      ele_sel  =Form("L.cer.asum_c>%g && L.cer.asum_c<%g",ele_min,ele_max);
      det_bdy  ="(L.prl1.e+L.prl2.e)/(HacL_D1_P0rb*1000)";
      pid_cut  =Form("%s>%g",det_bdy.c_str(),calhit);
    }
  else
  {
      Good_Ele =th_cut_R+ph_cut_R+track_R+beta_cut_R;
      pion_sel =Form("R.cer.asum_c<%g && R.cer.asum_c>%g",pion_max,pion_min);
      ele_sel  =Form("R.cer.asum_c>%g && R.cer.asum_c<%g",ele_min, ele_max);
      det_bdy  ="(R.ps.e+R.sh.e)/(HacR_D1_P0rb*1000)";
      pid_cut  =Form("%s>%g",det_bdy.c_str(),calhit);
  }

  double sample_ele=T->GetEntries(Good_Ele+ele_sel);
  double sample_ele_cut=T->GetEntries(Good_Ele+ele_sel+pid_cut);
  double ele_keep_eff = sample_ele_cut/sample_ele;
  double err_ele_keep = Binomial_Error(ele_keep_eff,sample_ele);
  if(debug>=2) cout <<sample_ele<<" "<<sample_ele_cut<<" "<<ele_keep_eff<<endl;

  double sample_pions=T->GetEntries(Good_Ele+pion_sel);
  double sample_pions_cut=T->GetEntries(Good_Ele+pion_sel+pid_cut);
  double pion_rej_eff=sample_pions_cut/sample_pions;
  double err_pion_rej = Binomial_Error(pion_rej_eff,sample_pions);
  if(debug>=2) cout <<sample_pions<<" "<<sample_pions_cut<<" "<<pion_rej_eff<<endl;

  if(debug) cout <<"Ele "<< ele_keep_eff <<" \t err:"<<err_ele_keep<<"  \t pion:   "<<pion_rej_eff<<" \t err: "<<err_pion_rej<<"\n";

  double cal_eff_total = ele_keep_eff;///(1-pion_rej_eff);
  Cal_eff.push_back(cal_eff_total);

  double cal_eff_err = sqrt( pow(err_ele_keep/ele_keep_eff,2));//+pow(err_pion_rej/(1-pion_rej_eff),2));
  Cal_eff.push_back(cal_eff_err);

  return Cal_eff;
}


//Calculate the pid eff for the Caloremters using run number
vector<double>Cal_eff_run(int run, double cut=0.0,int debug=1)
{
  vector<double> Cal_eff;
  TChain *T = LoadRun(run);
  Cal_eff = Cal_eff_tree(T,0.0,debug);
  delete T;
  return Cal_eff;
}
////////////////////////////////////////////////////////

vector<double> Cer_eff_tree(TChain *T, double cut=0.0, int debug=1)
{
  //Return vector
  vector<double> Cer_eff;
  //get info from tree
  THaRun* RunData = GetRunData(T);
  int run = RunData->GetNumber();
  //Get arm and a few other bits
  CODASetting coda = GetCODASetting(run);
  //Predefine varibles for L/R
  string det_bdy ="";
  string det_pre =Form("%s",coda.arm.Data());
  string det_suf ="e";
  string shrt="";
  double pion_max = 0.4;
  double pion_min = 0;
  double ele_min   = 0.95;
  double ele_max   = 1.05;


  //cut made in caloremter
  TCut pid_cut ="1";
  double cerhit=100;
  TCut pion_sel="1";
  TCut ele_sel ="1";
  //Cuts made in acc,tracking, and cherenkov
  TCut Good_Ele = "1";

  if(coda.arm=="L")//Left arm- Set detector name, cut value and GE selection
    {
      Good_Ele =th_cut_L+ph_cut_L+track_L+beta_cut_L;
      shrt     ="(L.prl1.e+L.prl2.e)/(HacL_D1_P0rb*1000)";
      pion_sel =Form("%s<%g && %s>%g ", shrt.c_str(), pion_max, shrt.c_str(), pion_min);
      ele_sel  =Form("%s>%g&&%s<%g",shrt.c_str(),ele_min,shrt.c_str(),ele_max);
      det_bdy  ="L.cer.asum_c";
      pid_cut  =Form("%s>%g",det_bdy.c_str(),cerhit);
    }
  else
  {
    Good_Ele =th_cut_R+ph_cut_R+track_R+beta_cut_R;
    shrt     ="(R.sh.e+T.ps.e)/(HacR_D1_P0rb*1000)";
    pion_sel =Form("%s<%g && %s>%g ", shrt.c_str(), pion_max, shrt.c_str(), pion_min);
    ele_sel  =Form("%s>%g&&%s>%g",shrt.c_str(),ele_min,shrt.c_str(),ele_max);
    det_bdy  ="R.cer.asum_c";
    pid_cut  =Form("%s>%g",det_bdy.c_str(),cerhit);

  }

  double sample_ele=T->GetEntries(Good_Ele+ele_sel);
  double sample_ele_cut=T->GetEntries(Good_Ele+ele_sel+pid_cut);
  double ele_keep_eff = sample_ele_cut/sample_ele;
  double err_ele_keep = Binomial_Error(ele_keep_eff,sample_ele);
  if(debug>=2) cout <<sample_ele<<" "<<sample_ele_cut<<" "<<ele_keep_eff<<endl;

  double sample_pions=T->GetEntries(Good_Ele+pion_sel);
  double sample_pions_cut=T->GetEntries(Good_Ele+pion_sel+pid_cut);
  double pion_rej_eff=sample_pions_cut/sample_pions;
  double err_pion_rej = Binomial_Error(pion_rej_eff,sample_pions);
  if(debug>=2) cout <<sample_pions<<" "<<sample_pions_cut<<" "<<pion_rej_eff<<endl;

  if(debug) cout <<"Ele "<< ele_keep_eff <<" \t err:"<<err_ele_keep<<"  \t pion:   "<<pion_rej_eff<<" \t err: "<<err_pion_rej<<"\n";

  double Cer_eff_total = ele_keep_eff;///(1-pion_rej_eff);
  Cer_eff.push_back(Cer_eff_total);

  double Cer_eff_err = sqrt( pow(err_ele_keep/ele_keep_eff,2));//+pow(err_pion_rej/(1-pion_rej_eff),2));
  Cer_eff.push_back(Cer_eff_err);

  return Cer_eff;
}


//Calculate the pid eff for the Caloremters using run number
vector<double> Cer_eff_run(int run, double cut=0.0,int debug=1)
{
  vector<double> Cer_eff;
  TChain *T = LoadRun(run);
  Cer_eff = Cer_eff_tree(T,0.0,debug);
  delete T;

  return Cer_eff;
}
