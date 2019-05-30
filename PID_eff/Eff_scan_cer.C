#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"

void Eff_scan_cer(string kin="", string tgt="H3", int debug=1, int draw=0)
{
  if(debug>3) draw=1;
  int num_of_cuts=50;
  double cer_cut =100;
  double cer_step=(4000-cer_cut)/num_of_cuts;

  vector<int> runlist = SQL_Kin_Target(kin,tgt);

  TChain *T = new TChain("T");

  for(unsigned int i =0; i<runlist.size();i++)
  {
    T->Add(LoadRun(runlist[i]));
    int total_ent = T->GetEntries();
    if(total_ent>=100000)break;
    if(i>=10){break;}
  }
  CODASetting CS = GetCODASetting(runlist[0]);
  string ARM=Form("%s",CS.arm.Data());

  TVector  num_passed_ele;
  TVector  num_passed_pions;
  TVectorD  passed_ele_eff;
  TVectorD  passed_pion_eff;
  TVectorD  passed_ele_eff_err;
  TVectorD  passed_pion_eff_err;
  TVectorD  cut_vec;
  TVectorD  cut_vec_error;

  num_passed_ele.ResizeTo(num_of_cuts+1);
  num_passed_pions.ResizeTo(num_of_cuts+1);
  passed_pion_eff.ResizeTo(num_of_cuts+1);
  passed_ele_eff.ResizeTo(num_of_cuts+1);
  passed_ele_eff_err.ResizeTo(num_of_cuts+1);
  passed_pion_eff_err.ResizeTo(num_of_cuts+1);
  cut_vec.ResizeTo(num_of_cuts+1);
  cut_vec_error.ResizeTo(num_of_cuts+1);

  double sample_ele;
  double sample_pions;

  TCut acc_cuts = dp_cut_L+th_cut_L+ph_cut_L+track_L;
  if(ARM=="R") acc_cuts=dp_cut_R+th_cut_R+ph_cut_R+track_R;

  TCut ele_sample_cut;
  TCut pion_sample_cut;

  TCanvas *C1 = new TCanvas("c1","c1");

  T->Draw("L.prl1.e:L.prl2.e>>h(200,0,3500,200,0,3500)",acc_cuts);
  C1->SetLogy();
  C1->Update();

  cout << "FLush the buff"<<endl;

  double ele_min_lim =4500;
  double ele_max_lim =5500;
  double pion_min_lim=0;
  double pion_max_lim=1000;

//delete C1;

cout<<endl << "FLush the buff"<<endl;
  string l_sh_c = "(L.prl1.e+L.prl2.e)/(L.gold.p*1000)";
  string r_sh_c = "(R.sh.e+R.ps.e)/(R.gold.p*1000)";

  string sh_cut= l_sh_c;
  string cer_cut="L.cer.asum_c";
  if(ARM=="R")
  {
    sh_cut=r_sh_c;
    cer_cut="R.cer.asum_c";
  }
  string e_l1 = Form("";
  string e_l2 = "";
  string scan_cut;
  ele_sample_cut=Form("%s>%f&&%s<%f",cer_cut.c_str(),ele_min_lim,cer_cut.c_str(),ele_max_lim);
  pion_sample_cut=Form("%s>%f&&%s<%f",cer_cut.c_str(),pion_min_lim,cer_cut.c_str(),pion_max_lim);
  sample_ele   = T->GetEntries(ele_sample_cut+acc_cuts);
  sample_pions = T->GetEntries(pion_sample_cut+acc_cuts);
  cout << sample_ele <<" "<< sample_pions<<endl;

  for(int i =0;i<num_of_cuts;i++)
  {
    EP_cut+=(EP_step);
    scan_cut=Form("&&%s>%f",sh_cut.c_str(),EP_cut);

    double num_passed_ele_1 = T->GetEntries( ele_sample_cut+ acc_cuts+ scan_cut);
    double num_passed_pions_1 = T->GetEntries( ele_sample_cut+ acc_cuts+ scan_cut);

    double e_eff_1 = num_passed_ele_1/sample_ele;
    double p_eff_1 = ( sample_pions - num_passed_pions_1)/sample_pions;
    double p_err = Bino_Err(p_eff_1,sample_pions);
    double e_err = Bino_Err(e_eff_1,sample_ele);

    num_passed_ele[i]=(num_passed_ele_1);
    num_passed_pions[i]=(num_passed_pions_1);
    passed_ele_eff[i]=(e_eff_1);
    passed_pion_eff[i]=(p_eff_1);
    passed_ele_eff_err[i]=(e_err);
    passed_pion_eff_err[i]=(p_err);
    cut_vec[i]=(EP_cut);
    cut_vec_error[i]=(0.5*EP_step);

    cout << EP_cut <<" "<< e_eff_1<<"  "<<p_eff_1<<endl;

  }





  TGraphErrors *ele_graph = new TGraphErrors( cut_vec, passed_ele_eff, cut_vec_error, passed_ele_eff_err);
  TGraphErrors *pion_graph = new TGraphErrors( cut_vec, passed_pion_eff, cut_vec_error, passed_pion_eff_err);
  TMultiGraph *mg = new TMultiGraph();


  TCanvas *C = new TCanvas("C","C");

  ele_graph->SetMarkerStyle(44);
  ele_graph->SetMarkerSize(2);

  pion_graph->SetMarkerStyle(33);
  pion_graph->SetMarkerColor(2);
  pion_graph->SetMarkerSize(2);

  mg->Add(ele_graph);
  mg->Add(pion_graph);

  mg->Draw("AP");











}
