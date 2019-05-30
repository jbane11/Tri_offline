#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/inc1.h"


TCut Basic_C = dp_cut_L+th_cut_L+ph_cut_L+ L_mara_trig+ track_L+ "1";

//used for ps_sh e not e/p
double x1_max = 0.11;
TCut x1 = Form("(L.prl1.e+L.prl2.e)/1000.0 < %f",x1_max);
TCut x1_cer = "L.cer.asum_c>=800";
double x2_min = 0.25;
double x2_max = 0.35;
TCut x2 = Form("(L.prl1.e+L.prl2.e)/1000.0 < %f &&  (L.prl1.e+L.prl2.e)/1000.0 > %f", x2_max ,x2_min);
TCut x2_cer = "L.cer.asum_c<600&&L.cer.asum_c>250";

TCut d_x2 = x2 + Form("L.cer.asum_c>%f",cer_min_L);
TCut es  = electron_cut_L;


//Call function to set plotting styles

void x1_x2_plots2d(string kin ="-h", string tgt="-h",string det = "-h",int bins=100, double fbin =-10, double lbin =10,int bins1=100, double fbin1 =-10, double lbin1 =10)
{

  SetStyles();
  //Find the runs in this kin!!
  vector<int> runlist = SQL_Kin_Target(kin,tgt);
  vector<double> blank = {0.0,0.0,0.0};
  if(runlist.size()==0) exit(1);
  TChain *T = new TChain("T");
  vector<double> clkcnt_vec;



  //Load up enough runs to get a total 500000 total events
  for(unsigned int i =0; i<runlist.size();i++)
  {
    T->Add(LoadRun(runlist[i]));

    clkcnt_vec.push_back(GetClkCount(T));

    //cout << clkcnt_vec[i] <<endl;
    int total_ent = T->GetEntries(L_mara_trig);

    if(total_ent>=100000)break;
    //if(i>=10){break;}
  }
  string det_plt_B = Form("%s>>Histo_B",det.c_str());
  string det_plt_x1 = Form("%s>>Histo_x1",det.c_str());
  string det_plt_x2 = Form("%s>>Histo_x2",det.c_str());
  string det_plt_d  = Form("%s>>Histo_d",det.c_str());
  string det_plt_e  = Form("%s>>Histo_e",det.c_str());

  TH2F *Histo_B = new TH2F("Histo_B","Histo_B" ,bins,fbin,lbin,bins1,fbin1,lbin1);
  TH2F *Histo_x1 = new TH2F("Histo_x1","Histo_x1" ,bins,fbin,lbin,bins1,fbin1,lbin1);
  TH2F *Histo_x2 = new TH2F("Histo_x2","Histo_x2" ,bins,fbin,lbin,bins1,fbin1,lbin1);
  TH2F *Histo_d = new TH2F("Histo_d","Histo_d" ,bins,fbin,lbin,bins1,fbin1,lbin1);
  TH2F *Histo_e = new TH2F("Histo_e","Histo_e" ,bins,fbin,lbin,bins1,fbin1,lbin1);

  Histo_B->SetMarkerStyle(31);
  Histo_B->SetMarkerSize(0.5);

  Histo_x1->SetMarkerColor(2);
  Histo_x1->SetMarkerStyle(29);

  Histo_x2->SetMarkerColor(7);
  Histo_x2->SetMarkerStyle(33);

  Histo_d->SetMarkerColor(9);
  Histo_d->SetMarkerStyle(34);

  Histo_e->SetMarkerColor(4);
  Histo_e->SetMarkerStyle(23);



  TCanvas *C = new TCanvas(Form("C%s_%s",kin.c_str(),det.c_str()), Form("Canvas for %s",det.c_str()) );
  T->Draw(Form("%s",det_plt_B.c_str()),Basic_C,"");
  T->Draw(Form("%s",det_plt_x1.c_str()),Basic_C+x1+x1_cer,"same");
  T->Draw(Form("%s",det_plt_x2.c_str()),Basic_C+x2+x2_cer,"same");
  T->Draw(Form("%s",det_plt_d.c_str()),Basic_C+d_x2,"same");
  T->Draw(Form("%s",det_plt_e.c_str()),Basic_C+es,"same");

  //string xdet = det.substr();




  TLegend *leg = new TLegend(0.6,0.6,0.95,0.91);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.04);
  leg->AddEntry(Histo_B,"Basic cuts","p");
  leg->AddEntry(Histo_x1,"X1","p");
  leg->AddEntry(Histo_x2,"X2","p");
  leg->AddEntry(Histo_d,"Deltas from x2","p");
  leg->AddEntry(Histo_e,"Good electrons","p");
  leg->SetNColumns(1);
  leg->Draw("same");

  Histo_B->SetTitle(Form("Kin %s %s",kin.c_str(),det.c_str()));
  Histo_B->GetYaxis()->SetTitle("Counts");
  Histo_B->GetXaxis()->SetTitle(Form("%s",det.c_str()));
  HistSettings(Histo_B);

  string imgname=det;
  if(imgname.find("/") != std::string::npos)
  {
    auto a = imgname.find("/");
    imgname.replace(a,a+1,"");
  }

  C->SaveAs(Form("./images/%s_%s_%s.eps",kin.c_str(),tgt.c_str(),imgname.c_str()),"eps");



}

void x1_x2_plots(string kin ="-h", string tgt="-h",string det = "-h",int bins=100, double fbin =-10, double lbin =10, int logy=1)
{
  if( kin =="-h" || kin == "help" || kin == "-help" || tgt == "-h" || det == "-h")
  {
      cout <<"############################################################\n\n";
      cout << "Help Statment for x1_x2 plots"<<"\n";
      cout << "You need to provide a kin(str), tgt(str) and detector(str)" <<"\n";
      cout << "Example: \".x x1_x2_plots.C(\"15\",\"H3\",\"L.tr.tg_y\") " <<"\n";
      cout <<"############################################################\n\n";
      exit(1);

  }



  SetStyles();
  //Find the runs in this kin!!
  vector<int> runlist = SQL_Kin_Target(kin,tgt);
  vector<double> blank = {0.0,0.0,0.0};
  if(runlist.size()==0) exit(1);
  TChain *T = new TChain("T");
  vector<double> clkcnt_vec;



  //Load up enough runs to get a total 500000 total events
  for(unsigned int i =0; i<runlist.size();i++)
  {
    T->Add(LoadRun(runlist[i]));

    clkcnt_vec.push_back(GetClkCount(T));

    //cout << clkcnt_vec[i] <<endl;
    int total_ent = T->GetEntries(L_mara_trig);

    if(total_ent>=100000)break;
    //if(i>=10){break;}
  }
  string det_plt_B = Form("%s>>Histo_B",det.c_str());
  string det_plt_x1 = Form("%s>>Histo_x1",det.c_str());
  string det_plt_x2 = Form("%s>>Histo_x2",det.c_str());
  string det_plt_d  = Form("%s>>Histo_d",det.c_str());
  string det_plt_e  = Form("%s>>Histo_e",det.c_str());

  TH1F *Histo_B = new TH1F("Histo_B","Histo_B",bins,fbin,lbin);
  TH1F *Histo_x1 = new TH1F("Histo_x1","Histo_x1",bins,fbin,lbin);
  TH1F *Histo_x2 = new TH1F("Histo_x2","Histo_x2",bins,fbin,lbin);
  TH1F *Histo_d = new TH1F("Histo_d","Histo_d",bins,fbin,lbin);
  TH1F *Histo_e = new TH1F("Histo_e","Histo_e",bins,fbin,lbin);

  TCanvas *C = new TCanvas(Form("C%s_%s",kin.c_str(),det.c_str()), Form("Canvas for %s",det.c_str()) );
  T->Draw(Form("%s",det_plt_B.c_str()),Basic_C);
  T->Draw(Form("%s",det_plt_x1.c_str()),Basic_C+x1+x1_cer,"same");
  T->Draw(Form("%s",det_plt_x2.c_str()),Basic_C+x2+x2_cer,"same");
  T->Draw(Form("%s",det_plt_d.c_str()),Basic_C+d_x2,"same");
  T->Draw(Form("%s",det_plt_e.c_str()),Basic_C+es,"same");

  Histo_B->SetLineWidth(2);
  Histo_x1->SetLineColor(2);
  Histo_x1->SetFillColor(2);
  Histo_x1->SetFillStyle(3003);
  Histo_x2->SetLineColor(7);
  Histo_x2->SetFillColor(7);
  Histo_x2->SetFillStyle(3385);
  Histo_d->SetFillColor(9);
  Histo_d->SetLineColor(9);
  Histo_d->SetFillStyle(3395);
  Histo_e->SetFillColor(4);
  Histo_e->SetLineColor(4);
  Histo_e->SetFillStyle(3375);

  TLegend *leg = new TLegend(0.6,0.6,0.95,0.91);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.04);
  leg->AddEntry(Histo_B,"Basic cuts","l");
  leg->AddEntry(Histo_x1,"X1","l");
  leg->AddEntry(Histo_x2,"pions","l");
  leg->AddEntry(Histo_d,"deltas","l");
  leg->AddEntry(Histo_e,"good electrons","l");
  leg->SetNColumns(1);
  leg->Draw("same");

  Histo_B->SetTitle(Form("Kin %s %s",kin.c_str(),det.c_str()));
  Histo_B->GetYaxis()->SetTitle("Counts");
  Histo_B->GetXaxis()->SetTitle(Form("%s",det.c_str()));
  HistSettings(Histo_B);
  TLatex ltex;
  ltex.SetTextSize(0.025);
  string cuts = Form("%s", Basic_C.GetTitle());
  vector <string> cut_vec = parse_std(cuts,"&&");

  double latex_y_per_mod = 0.05;
  if(logy==1){
      latex_y_per_mod = 0.05;
      C->SetLogy();

  }

  double latexyper=0.8;
  double latexyorg = Histo_B->GetBinContent(Histo_B->GetMaximumBin());
  double latexy;
  double latexx =0.95;//*lbin;

  //cout << cut_vec.size() << endl;
  for( unsigned int i =0; i<cut_vec.size(); i++)
  {
      latexyper -= latex_y_per_mod;
      latexy = latexyper;//*latexyorg;
      /*if(i<cut_vec.size()-1){
        ltex.SetTextAlign(31);
        ltex.DrawLatexNDC(latexx,latexy,Form("%s \t: %s",cut_vec[i].c_str() ,cut_vec[i+1].c_str()) );

        i++;
      }
      else{ltex.DrawLatexNDC(latexx,latexy,Form("%s ",cut_vec[i].c_str())); ltex.SetTextAlign(31);  }*/

  }//end of cut list
  string imgname=det;
  if(imgname.find("/") != std::string::npos)
  {
    auto a = imgname.find("/");
    imgname.replace(a,a+1,"");
  }

  C->SaveAs(Form("./images/%s_%s_%s.eps",kin.c_str(),tgt.c_str(),imgname.c_str()),"eps");

}



vector<double> x1_x2_clkcnt(string kin="",string tgt="")
{
  TCut Basic_C = dp_cut_L+th_cut_L+ph_cut_L+ L_mara_trig+ track_L+ "1";
  //used for ps_sh e not e/p


  //Find the runs in this kin!!
  vector<int> runlist = SQL_Kin_Target(kin,tgt);
  vector<double> blank = {0.0,0.0,0.0};
  if(runlist.size()==0) return blank;
  TChain *T = new TChain("T");
  vector<double> clkcnt_vec;
  //Load up enough runs to get a total 500000 total events
  double s_clkcnt=0;
  double total_clkcnt =0;
  double s_charge=0;
  double total_charge=0;


  for(unsigned int i =0; i<runlist.size();i++)
  {
    T->Add(LoadRun(runlist[i]));
    s_clkcnt=GetClkCount(T);
    clkcnt_vec.push_back(s_clkcnt);
    total_clkcnt+=s_clkcnt;
    s_charge=GetChargeSQL(runlist[i]);
    total_charge+=s_charge;
    int total_ent = T->GetEntries(L_mara_trig);
    if(total_ent>=100000)break;
    //if(i>=10){break;}
    s_clkcnt=0;
    s_charge=0;
  }

  int x1_events = T->GetEntries(Basic_C+x1+x1_cer);
  int x2_events = T->GetEntries(Basic_C+x2+x2_cer);
  int basic_ev  = T->GetEntries(Basic_C);
  int deltas_events= T->GetEntries(Basic_C+d_x2);
  int G_ele= T->GetEntries(Basic_C+es+z_cut_L);

  double x1_time = x1_events/total_clkcnt;
  double x2_time = x2_events/total_clkcnt;
  double basic_time = basic_ev/total_clkcnt;


  vector<double> timenorm;
  timenorm.push_back(x1_time);
  timenorm.push_back(x2_time);
  timenorm.push_back(basic_time);
  timenorm.push_back(x1_events);
  timenorm.push_back(x2_events);
  timenorm.push_back(basic_ev);
  timenorm.push_back(deltas_events);
  timenorm.push_back(G_ele);
  timenorm.push_back(total_clkcnt);
  timenorm.push_back(total_charge);


  return timenorm;
}

vector<vector<double>> x1_x2_allkin(string tgt="H3")
{
  vector< vector<double>> x1_x2_tn;
  std::ofstream output_file;
  output_file.open(Form("./images/x1_x2_%s.txt",tgt.c_str()));
  for(int i=1; i<16;i++)
  {
    x1_x2_tn.push_back(x1_x2_clkcnt(Form("%d",i),tgt));

    for(unsigned int k =0;k<x1_x2_tn[i-1].size();k++)
    {
      cout << x1_x2_tn[i-1][k]<<"\t";
      output_file<<x1_x2_tn[i-1][k];
      if(k!=x1_x2_tn[i-1].size()-1)output_file<<",";
    }
    output_file<<"\n";
  }

  output_file.close();
  return x1_x2_tn;
}
