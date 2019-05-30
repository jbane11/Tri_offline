#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"


vector<double> delta_ray_cf(string kin="",string tgt="H3", int debug=1, int draw=1, int savedraw=1)
{
  //Call function to set plotting styles
  SetStyles();
  //Find the runs in this kin!!
  vector<int> runlist = SQL_Kin_Target(kin,tgt);
  vector<double> blank = {0.0,0.0,0.0};
  if(runlist.size()==0) return blank;
  TChain *T = new TChain("T");

  //Load up enought runs to get a total 500000 total events
  for(unsigned int i =0; i<runlist.size();i++)
  {
    T->Add(LoadRun(runlist[i]));
    int total_ent = T->GetEntries(L_mara_trig);
    if(total_ent>=500000)break;
    //if(i>=10){break;}
  }

  //Get info from the first run in the list
  CODASetting CS = GetCODASetting(runlist[0]);
  string ARM=Form("%s",CS.arm.Data());

  //Make the arm depent detector strings and values
  string l_sh = "(L.prl1.e+L.prl2.e)/(1000)";
  string r_sh = "(R.sh.e+R.ps.e)/(1000)";
  string tot_sh=l_sh;
  double cer_min= cer_min_L;
  string cer_sum = Form("%s.cer.asum_c",ARM.c_str());
  if(ARM=="R")
  {
    tot_sh=r_sh;
    cer_min= cer_min_R;
  }

  TCut delta_r_cut ="";// Form("%s <=0.5 && %s >=0.1", tot_sh.c_str(), tot_sh.c_str() );

  TCut cut=acc_cut_L+L_mara_trig+beta_cut_L;
  if(tgt=="DM" || tgt =="EM") {cut = dp_cut_L+ph_cut_L+th_cut_L;}


  int Electrons = T->GetEntries(cut+electron_cut_L);


  int H_pions   = T->GetEntries(cut+!cer_cut_L +sh_cut_L);

  int pions     = T->GetEntries(cut+!cer_cut_L+!sh_cut_L+delta_r_cut);

  int deltas    = T->GetEntries(cut+ cer_cut_L+!sh_cut_L+delta_r_cut);

  if(draw||savedraw)
  {
    TLatex *tex_e = new TLatex(2.5,8000,Form("#color[2]{Electrons %d}",Electrons) );
    TLatex *tex_Hp = new TLatex(2.5,1000,Form("#color[2]{Pions/ineff cer\n %d}",H_pions) );
    TLatex *tex_p = new TLatex(0.5,1000,Form("#color[2]{Pions %d}",pions) );
    TLatex *tex_d = new TLatex(0.5,8000,Form("#color[2]{Delta Rays %d}", deltas) );
    TCanvas *C = new TCanvas( Form("%s_%s",tgt.c_str(),kin.c_str()), Form("%s_%s",tgt.c_str(),kin.c_str()),700,500) ;
    TH2F *Hpid = new TH2F(Form("Histpid_%s_%s",tgt.c_str(),kin.c_str()), Form(" Cer:Cal for kin %s on %s",kin.c_str(),tgt.c_str()), 100, -0.25, 4, 500, -100, 10000 );
    T->Draw(Form("%s:%s>>Histpid_%s_%s", cer_sum.c_str(), tot_sh.c_str(), tgt.c_str(), kin.c_str()), cut, "colz");
    Hpid->GetXaxis()->SetTitle("Total Cal energy(GeV)");
    Hpid->GetYaxis()->SetTitle("Cer sum(adc channels)");

    double cal_max = 4;//Max value to help make cut lines
    TLine *cer_cut_Line = new TLine(0,cer_min,cal_max,cer_min);
    cer_cut_Line->SetLineColor(2);
    cer_cut_Line->SetLineWidth(2);
    cer_cut_Line->Draw("same");

    TLine *cal_cut_Line = new TLine(sh_min_L*3,0,sh_min_L*3,10000);
    cal_cut_Line->SetLineColor(2);
    cal_cut_Line->SetLineWidth(2);
    cal_cut_Line->Draw("same");

    tex_e->Draw("same"); tex_p->Draw("same");
    tex_Hp->Draw("same");tex_d->Draw("same");
    if(savedraw) C->SaveAs(Form("./images/deltas_%s_%s.eps" ,tgt.c_str(), kin.c_str()), "eps");
    if(!draw)delete C;
    T=nullptr;
    delete T;
  }

  vector<double> delta_cor = {deltas/(Electrons*1.0), Bino_Err(deltas/(Electrons*1.0),Electrons), deltas*1.0/(pions*1.0)};


return delta_cor;
}



vector<vector<double>> delta_graph(string tgt="H3")
{
  TVectorD d_p;
  TVectorD kinnum;
  TVectorD d_e;
  TVectorD d_e_err;
  TVectorD x_err;

  vector < vector<double>> delta2D;
  vector<double> deltav;
  vector<double> tmp;
  int cnt=0;

  ofstream output;
  output.open(Form("Deltas_%s.csv",tgt.c_str()));
  for(int i=1;i<16;i++)
  {
    deltav = delta_ray_cf(Form("%d",i),tgt,1,0,1);
    //if(deltav[0]==0) continue;
    d_p.ResizeTo(cnt+1);
    d_e.ResizeTo(cnt+1);
    d_e_err.ResizeTo(cnt+1);
    x_err.ResizeTo(cnt+1);
    kinnum.ResizeTo(cnt+1);

    d_e[cnt]=deltav[0];
    d_p[cnt]=deltav[2];
    d_e_err[cnt]=deltav[1];
    x_err[cnt] = 0.0;
    kinnum[cnt]=i;
    tmp.push_back(d_e[cnt]);
    tmp.push_back(kinnum[cnt]);
    tmp.push_back(d_e_err[cnt]);
    tmp.push_back(d_p[cnt]);
    delta2D.push_back(tmp);
    tmp.clear();
    deltav.clear();
    cnt++;
    output<<d_e[cnt]<<","<<kinnum[cnt]<<","<<d_e_err[cnt]<<","<<d_p[cnt]<<"\n";
  }
  output.close();
  TCanvas *C = new TCanvas(Form("C_%s",tgt.c_str()), Form("Delta cor for %s",tgt.c_str()));

  TMultiGraph *MG = new TMultiGraph();
  TGraph *G_dp = new TGraph(kinnum,d_p);
  TGraphErrors *G_de_e = new TGraphErrors(kinnum,d_e,x_err,d_e_err);

  TGraph *G_de = new TGraph(kinnum,d_e);
  G_dp->SetLineColor(2);
  G_dp->SetMarkerColor(2);
  G_dp->SetMarkerStyle(44);
  G_dp->SetMarkerSize(2);

  G_de_e->SetLineColor(2);
  G_de_e->SetMarkerColor(2);
  G_de_e->SetMarkerStyle(44);
  G_de_e->SetMarkerSize(2);
  G_de_e->Draw("ap");
  G_de->SetMarkerColor(4);
  G_de->SetMarkerStyle(41);
  G_de->SetMarkerSize(2);

  MG->Add(G_dp);
  MG->Add(G_de);
  //MG->Draw("ap");
  C->Update();
  return delta2D;
}



void Deltas_csv()
{
  vector< vector< vector<double>>> A;
  vector< vector<double>> DeltaH3 =delta_graph("H3");
  vector< vector<double>> DeltaHe3=delta_graph("He3");
  vector< vector<double>> DeltaD2 =delta_graph("D2");
  vector< vector<double>> DeltaDM =delta_graph("DM");
  vector< vector<double>> DeltaEM =delta_graph("EM");
  vector< vector<double>> DeltaC  =delta_graph("C");
  A.push_back(DeltaH3);
  A.push_back(DeltaHe3);
  A.push_back(DeltaD2);
  A.push_back(DeltaDM);
  A.push_back(DeltaEM);
  A.push_back(DeltaC);

  ofstream output; output.open("./images/Deltas.csv");
  for(unsigned int i=0;i<DeltaH3.size();i++)
  {
    for(unsigned int j=0;j<A.size();j++)
    {
      for(unsigned int k=0;k<A[j][i].size();k++)
      {
        output<<A[j][i][k];
        if(j!=A.size()-1&&k!=A[j][i].size()-1)output<<",";
      }//end of tgt
    }//end of line
    output<<"\n";
  }

  output.close();
}
