#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"

void Eff_scan_cal(string kin="",string tgt="H3", string version="",int dd_cut=1,int debug=1,int draw=1)
{
  SetStyles();
  int askforsam=0;
  if(debug>3) draw=1;
  int num_of_cuts=25;
  double EP_cut =0.0;
  double EP_1st=EP_cut;
  double EP_end =1.00;
  if(version=="s")
  {
    EP_cut=0.6;
    EP_1st=EP_cut;
    EP_end=0.9;
    if(version.find("_")==std::string::npos)version="_"+version;
  }

  double EP_step=(EP_end-EP_cut)/num_of_cuts;

  vector<int> runlist = SQL_Kin_Target(kin,tgt);
  if(runlist.size()==0)exit(1);
  TChain *T = new TChain("T");

  for(unsigned int i =0; i<runlist.size();i++)
  {
    T->Add(LoadRun(runlist[i]));
    int total_ent = T->GetEntries();
    if(total_ent>=500000)break;
    if(i>=10){break;}
  }
  CODASetting CS = GetCODASetting(runlist[0]);
  string ARM=Form("%s",CS.arm.Data());

  TVector  num_passed_ele;
  TVectorD  l1_ele_eff;
  TVectorD  l2_ele_eff;
  TVector  num_passed_pions;
  TVectorD  passed_ele_eff;
  TVectorD  passed_pion_eff;
  TVectorD  passed_ele_eff_err;
  TVectorD  passed_pion_eff_err;
  TVectorD  cut_vec;
  TVectorD  cut_vec_error;
  TVectorD  multiply;
  TVectorD  multiply_err;

  num_passed_ele.ResizeTo(num_of_cuts+1);
  multiply.ResizeTo(num_of_cuts+1);
  multiply_err.ResizeTo(num_of_cuts+1);
  num_passed_pions.ResizeTo(num_of_cuts+1);
  passed_pion_eff.ResizeTo(num_of_cuts+1);
  passed_ele_eff.ResizeTo(num_of_cuts+1);
  passed_ele_eff_err.ResizeTo(num_of_cuts+1);
  passed_pion_eff_err.ResizeTo(num_of_cuts+1);
  cut_vec.ResizeTo(num_of_cuts+1);
  cut_vec_error.ResizeTo(num_of_cuts+1);

  l1_ele_eff.ResizeTo(num_of_cuts+1);
  l2_ele_eff.ResizeTo(num_of_cuts+1);

  double sample_ele;
  double sample_pions;

  TCut acc_cuts = dp_cut_L+th_cut_L+ph_cut_L+track_L+aperture_L+L_mara_trig;
  if(ARM=="R") acc_cuts=dp_cut_R+th_cut_R+ph_cut_R+track_R;

  TCut ele_sample_cut;
  TCut pion_sample_cut;
  double ele_min_lim =4500;
  double ele_max_lim =5500;
  double pion_min_lim=0;
  double pion_max_lim=800;

  if(askforsam)
  {
    TCanvas *C1 = new TCanvas("c1","c1");

    T->Draw("L.cer.asum_c>>h(10000,0,20000)",acc_cuts);
    C1->SetLogy();
    C1->Update();

    cout << "FLush the buff"<<endl;
    cout<< "ele_min " << "  ele_max  "<< "  pion_min " <<" pion_max "<<endl;
    cin >> ele_min_lim>> ele_max_lim>> pion_min_lim>> pion_max_lim;
    //delete C1;
  }


  string l_sh_c = "(L.prl1.e+L.prl2.e)/(L.gold.p*1000)";
  string l_l1_c ="(L.prl1.e)/(1000*L.gold.p)";
  string l_l2_c ="(L.prl1.e)/(1000*L.gold.p)";
  string r_sh_c = "(R.sh.e+R.ps.e)/(R.gold.p*1000)";
  string r_l1_c ="R.ps.e/(R.gold.p*1000)";
  string r_l2_c ="R.sh.e/(R.gold.p*1000)";
  double delta_cut_min = 0.3;

  string delta_v="_delta";
  if(dd_cut==0){delta_cut_min=0.0;delta_v="";}

  TCut Delta_c = Form("L.prl2.e/1000.0>%f",delta_cut_min);
  string sh_cut= l_sh_c;
  string cer_cut="L.cer.asum_c";
  if(ARM=="R")
  {
    sh_cut=r_sh_c;
    cer_cut="R.cer.asum_c";
  }
  string scan_cut;
  string scan_l1_cut;
  string scan_l2_cut;
  ele_sample_cut=Form("%s>%f&&%s<%f",cer_cut.c_str(),ele_min_lim,cer_cut.c_str(),ele_max_lim);
  pion_sample_cut=Form("%s>%f&&%s<%f",cer_cut.c_str(),pion_min_lim,cer_cut.c_str(),pion_max_lim);
  sample_ele   = T->GetEntries(ele_sample_cut+acc_cuts+Delta_c);
  sample_pions = T->GetEntries(pion_sample_cut+acc_cuts);
  cout << sample_ele <<" "<< sample_pions<<endl;
  ofstream outf;
  outf.open(Form("./images/Eff_scan_cal_%s_%s%s%s.csv", tgt.c_str(), kin.c_str(), version.c_str(),delta_v.c_str()));
  outf<<"Cut"<<","<<"electrons"<<","<<"Ele_eff"<<","<<"Ele_err"<<","<<"Non_electrons"<<","<<"Non_ele_eff"<<","<<"non_ele_err"<<","<<"Total_eff"<<","<<"Total_err\n";
  for(int i =0;i<num_of_cuts;i++)
  {

    scan_cut=Form("&&%s>%f",sh_cut.c_str(),EP_cut);
    scan_l1_cut=Form("&&%s>%f",l_l1_c.c_str(),EP_cut);
    scan_l2_cut=Form("&&%s>%f",l_l2_c.c_str(),EP_cut);

    double num_passed_ele_1 = T->GetEntries( ele_sample_cut + Delta_c+ acc_cuts+ scan_cut);
    double num_passed_pions_1 = T->GetEntries( pion_sample_cut+ acc_cuts+ scan_cut);
    double l1_p_ele = T->GetEntries( ele_sample_cut + Delta_c+ acc_cuts+ scan_l1_cut);
    double l2_p_ele = T->GetEntries( ele_sample_cut + Delta_c+ acc_cuts+ scan_l2_cut);


    double e_eff_1 = num_passed_ele_1/sample_ele;
    double p_eff_1 = ( sample_pions - num_passed_pions_1)/sample_pions;
    double p_err = Bino_Err(p_eff_1,sample_pions);
    double e_err = Bino_Err(e_eff_1,sample_ele);
    l1_ele_eff[i]=(l1_p_ele/sample_ele);
    l2_ele_eff[i]=(l2_p_ele/sample_ele);
    num_passed_ele[i]=(num_passed_ele_1);
    num_passed_pions[i]=(num_passed_pions_1);
    passed_ele_eff[i]=(e_eff_1);
    passed_pion_eff[i]=(p_eff_1);
    passed_ele_eff_err[i]=(e_err);
    passed_pion_eff_err[i]=(p_err);
    cut_vec[i]=(EP_cut);
    cut_vec_error[i]=(0.5*EP_step);

    multiply[i]=e_eff_1*p_eff_1;
    multiply_err[i]= sqrt( pow(p_err,2) +pow(e_err,2));

    if(debug)cout << EP_cut <<" "<< e_eff_1<<"  "<<p_eff_1<<"   "<< multiply[i]<<endl;
    EP_cut+=(EP_step);
    outf<<EP_cut <<","<<num_passed_ele_1 <<"," << e_eff_1 <<","<< e_err <<","<<num_passed_pions_1<<","<< p_eff_1 <<","<< p_err <<","<< multiply[i] <<","<< multiply_err[i]<<","<<l1_p_ele<<","<<l2_p_ele<<"\n";

  }
  outf.close();
  if(draw)
  {
    TGraphErrors *ele_graph = new TGraphErrors( cut_vec, passed_ele_eff, cut_vec_error, passed_ele_eff_err);
    TGraphErrors *pion_graph = new TGraphErrors( cut_vec, passed_pion_eff, cut_vec_error, passed_pion_eff_err);
    TGraphErrors *multi_graph = new TGraphErrors( cut_vec,multiply, cut_vec_error, multiply_err);
    TGraphErrors *l1_graph = new TGraphErrors( cut_vec,l1_ele_eff, cut_vec_error, passed_ele_eff_err);
TGraphErrors *l2_graph = new TGraphErrors( cut_vec,l2_ele_eff, cut_vec_error, passed_ele_eff_err);


    TMultiGraph *mg = new TMultiGraph();


    TCanvas *C = new TCanvas("C","C");

    ele_graph->SetMarkerStyle(29);
    ele_graph->SetMarkerSize(2);

    pion_graph->SetMarkerStyle(33);
    pion_graph->SetMarkerColor(2);
    pion_graph->SetMarkerSize(2);

    multi_graph->SetMarkerStyle(34);
    multi_graph->SetMarkerColor(4);
    multi_graph->SetMarkerSize(2.5);

    l1_graph->SetMarkerStyle(26);
    l1_graph->SetMarkerColor(26);
    l1_graph->SetMarkerSize(2.5);
    l2_graph->SetMarkerStyle(27);
    l2_graph->SetMarkerColor(30);
    l2_graph->SetMarkerSize(2.5);

    mg->Add(ele_graph);
    mg->Add(pion_graph);
    mg->Add(multi_graph);
    mg->Add(l1_graph);
    mg->Add(l2_graph);


    mg->Draw("ap");
    mg->GetXaxis()->SetRangeUser(EP_1st*0.8,EP_end*1.2);
    mg->GetYaxis()->SetRangeUser(0.8,1.01);
    mg->SetTitle("Electron selection and Pion rejection eff.");
    mg->GetXaxis()->SetTitle("Cut Value");
    mg->GetYaxis()->SetTitle("Eff.");


    TLegend *leg = new TLegend(0.7,0.89,0.9,0.98);
    leg->AddEntry(ele_graph,"Ele eff", "lp");
    leg->AddEntry(pion_graph,"Non electron rejection eff", "lp");
    leg->AddEntry(multi_graph,"Total eff.", "lp");
    leg->AddEntry(l1_graph,"Ele eff L1", "lp");
    leg->AddEntry(l2_graph,"Ele eff L2", "lp");

    leg->Draw("same");
    C->SaveAs(Form("./images/Eff_scan_cal_%s_%s%s%s.eps", tgt.c_str(), kin.c_str(), version.c_str(),delta_v.c_str()),"eps");
  }
}
