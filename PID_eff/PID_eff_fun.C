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


vector<double> Cer_cut_eff(TChain *T,int debug=1)
{
  vector<double> cer_cut_eff;
  double cut=0;
  //get info from tree
  THaRun* RunData = GetRunData(T);
  int run = RunData->GetNumber();
  CODASetting coda = GetCODASetting(run);

  TCut cer_cut = "";
  TCut ele_sample="";
  if(coda.arm=="L")
  {
    cut=cer_min_L;
    ele_sample = dp_cut_L+ th_cut_L+ ph_cut_L+ aperture_L+ beta_cut_L;
    ele_sample += Form( "(L.prl1.e+L.prl2.e) >HacL_D1_P0rb*1000*%g" ,0.95);
    cer_cut = Form("%s.cer.asum_c>=%f",coda.arm.Data(),cut);
  }
  else
  {
    cut=cer_min_R;
    ele_sample = dp_cut_R+ th_cut_R+ ph_cut_R+ beta_cut_R;
    ele_sample += Form ("(R.ps.e+R.sh.e)> HacR_D1_P0rb*1000*%g", 0.95);
    cer_cut = Form("%s.cer.asum_c>=%f",coda.arm.Data(),cut);
  }
  if(debug)
  {
    cout << "Sample cut: "<< ele_sample<<endl;
    cout << "Electron cut: " << cer_cut << "&&" << ele_sample <<endl;
  }

  double electrons_pre_cut = T->GetEntries(ele_sample);
  double electrons_post_cut= T->GetEntries( cer_cut&& ele_sample);
  if(debug)
  {
    cout<< "Sample size : " << electrons_pre_cut;
    cout<< "   Good es  :"  << electrons_post_cut <<endl;
  }

  double eff = electrons_post_cut/electrons_pre_cut;
  double err = Bino_Err(eff,electrons_pre_cut);

  cer_cut_eff.push_back(eff);
  cer_cut_eff.push_back(err);

  return cer_cut_eff;
}

vector<double> Cal_cut_eff(TChain *T, int debug=1)
{
  vector<double> cal_cut_eff;
  double cut=0;
  //get info from tree
  THaRun* RunData = GetRunData(T);
  int run = RunData->GetNumber();
  CODASetting coda = GetCODASetting(run);

  TCut cal_cut = "";
  TCut ele_sample="";
  if(coda.arm=="L")
  {
    cut=sh_min_L;
    ele_sample = dp_cut_L+ th_cut_L+ ph_cut_L+ beta_cut_L+ track_L;
    cal_cut = Form( "(L.prl1.e+L.prl2.e) >L.gold.p*1000*%g" ,cut);
    ele_sample += Form("%s.cer.asum_c>=%f&&%s.cer.asum_c<=%f",coda.arm.Data(),3500.0, coda.arm.Data(),7000.0);
  }
  else
  {
    cut=sh_min_R;
    ele_sample = dp_cut_R+ th_cut_R+ ph_cut_R+ beta_cut_R+ track_R;
    cal_cut= Form ("(R.ps.e+R.sh.e)> R.gold.p*1000*%g", cut);
    ele_sample += Form("%s.cer.asum_c>=%f&&%s.cer.asum_c<=%f",coda.arm.Data(),2700.0,coda.arm.Data(),8700.0);
  }
  if(debug)
  {
    cout << "Sample cut: "<< ele_sample<<endl;
    cout << "Electron cut: + " << cal_cut<<endl;
  }

  double electrons_pre_cut = T->GetEntries(ele_sample);
  double electrons_post_cut= T->GetEntries( cal_cut&& ele_sample);
  if(debug)
  {
    cout<< "Sample size : " << electrons_pre_cut;
    cout<< "   Good es  :"  << electrons_post_cut <<endl;
  }

  double eff = electrons_post_cut/electrons_pre_cut;
  double err = Bino_Err(eff,electrons_pre_cut);

  cal_cut_eff.push_back(eff);
  cal_cut_eff.push_back(err);
  return cal_cut_eff;;
}


vector<double> PID_eff_tree(TChain *T, int debug=1)
{
  vector<double> PID_eff;

  return PID_eff;
}
//Calculate the pid eff for the Caloremters using run number
vector<double> PID_eff_run(int run, int debug=1)
{
  vector<double> PID_eff;
  TChain *T = LoadRun(run);
  PID_eff = PID_eff_tree(T,debug);
  delete T;

  return PID_eff;
}

//vector<vector<double>> PID_eff_cal(double cal_cut=0.7)




vector<double> PID_layer_T(TChain *T, int layer=1)
{
  vector<double>pid_eff;
  double prl1_ele_sample_min=1.5;
  double prl1_ele_sample_max=2.5;
  double prl1_non_ele_sample_min=0.0;
  double prl1_non_ele_sample_max=0.1;

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
  TCut prl2_ele_non_sample_cut =Form("((L.prl2.e)/1000. <0.3)");

  TCut tote_ele_sample_cut= Form("((L.prl2.e+L.prl1.e)/1000.>2.5)");
  TCut tote_ele_non_sample_cut= Form("(L.prl2.e/1000.0 <0.3  && L.prl1.e/1000.<.1)");


  TCut basic_c = dp_cut_L+ ph_cut_L+th_cut_L + beta_cut_L+L_mara_trig;

  double prl1_sample_ele     = T->GetEntries(basic_c+ prl2_ele_sample_cut+ cer_ele_sample_cut);
  double prl1_sample_nele = T->GetEntries(basic_c+ prl2_ele_non_sample_cut+ cer_non_ele_sample_cut);


  double cer_sample_ele     = T->GetEntries(basic_c+ tote_ele_sample_cut);
  double cer_sample_nele = T->GetEntries(basic_c+ tote_ele_non_sample_cut);

  double prl2_sample_ele     = T->GetEntries(basic_c+ prl1_ele_sample_cut+ cer_ele_sample_cut);
  double prl2_sample_nele = T->GetEntries(basic_c+ prl1_ele_non_sample_cut+ cer_non_ele_sample_cut);


  double prl1_ele     = T->GetEntries(basic_c+ prl2_ele_sample_cut+ cer_ele_sample_cut+l1_cut_L);
  double prl1_nele = T->GetEntries(basic_c+ prl2_ele_non_sample_cut+ cer_non_ele_sample_cut+l1_cut_L);


  double cer_ele     = T->GetEntries(basic_c+ tote_ele_sample_cut+cer_cut_L);
  double cer_nele = T->GetEntries(basic_c+ tote_ele_non_sample_cut+ cer_cut_L);

  double prl2_ele     = T->GetEntries(basic_c+ prl1_ele_sample_cut+ cer_ele_sample_cut+l2_cut_L);
  double prl2_nele = T->GetEntries(basic_c+ prl1_ele_non_sample_cut+ cer_non_ele_sample_cut+l2_cut_L);


  double ele_eff_l1 = prl1_ele/prl1_sample_ele;
  double ele_eff_l2 = prl2_ele/prl2_sample_ele;
  double ele_eff_l3 = cer_ele/cer_sample_ele;

  double nele_eff_l1 =1 - prl1_nele/prl1_sample_nele;
  double nele_eff_l2 =1 - prl2_nele/prl2_sample_nele;
  double nele_eff_l3 =1 -  cer_nele/cer_sample_nele;


  double ele_err_l1 = Bino_Err(ele_eff_l1,prl1_sample_ele);
  double ele_err_l2 = Bino_Err(ele_eff_l2,prl2_sample_ele);
  double ele_err_l3 = Bino_Err(ele_eff_l3,cer_sample_ele);
  double nele_err_l1 = Bino_Err((1-nele_eff_l1),prl1_sample_nele);
  double nele_err_l2 = Bino_Err((1-nele_eff_l2),prl2_sample_nele);
  double nele_err_l3 = Bino_Err((1-nele_eff_l3),cer_sample_nele);

cout <<"1 ne"<< prl1_sample_nele <<"  2 ne  " <<prl2_sample_nele << " cer ne " << cer_sample_nele <<endl;

  switch (layer) {
    case 1:
      pid_eff.push_back(ele_eff_l1);
      pid_eff.push_back(ele_err_l1);
      pid_eff.push_back(nele_eff_l1);
      pid_eff.push_back(nele_err_l1);
      break;
    case 2:
      pid_eff.push_back(ele_eff_l2);
      pid_eff.push_back(ele_err_l2);
      pid_eff.push_back(nele_eff_l2);
      pid_eff.push_back(nele_err_l2);
      break;
    case 3:
      pid_eff.push_back(ele_eff_l3);
      pid_eff.push_back(ele_err_l3);
      pid_eff.push_back(nele_eff_l3);
      pid_eff.push_back(nele_err_l3);
      break;
    default:
      pid_eff.push_back(ele_eff_l1);
      pid_eff.push_back(ele_err_l1);
      pid_eff.push_back(nele_eff_l1);
      pid_eff.push_back(nele_err_l1);
      pid_eff.push_back(ele_eff_l2);
      pid_eff.push_back(ele_err_l2);
      pid_eff.push_back(nele_eff_l2);
      pid_eff.push_back(nele_err_l2);
      pid_eff.push_back(ele_eff_l3);
      pid_eff.push_back(ele_err_l3);
      pid_eff.push_back(nele_eff_l3);
      pid_eff.push_back(nele_err_l3);
      break;
    }

  return pid_eff;

}
vector<double> PID_layer(string kin="",string tgt="", int layer=1)
{
  vector <double> blank = {0.0,0.0,0.0,0.0};
  if (layer >3) blank.resize(12,0.0);
  vector<int> runlist = SQL_Kin_Target(kin,tgt);
  if(runlist.size()==0)return blank ;
  TChain *T = new TChain("T");
  for(unsigned int i =0; i<runlist.size();i++)
  {
    T->Add(LoadRun(runlist[i]));
    int total_ent = T->GetEntries();
    if(total_ent>=5000000)break;
    if(i>=10){break;}
  }


  vector<double> PID_eff = PID_layer_T(T,layer);
  T=nullptr;
  delete T;
  return PID_eff;
}



void PID_csv(string tgt="H3")
{
  ofstream output;
  output.open(Form("PID_allkin_%s.csv",tgt.c_str()));
  output<<"kin"<<","<<"Prl1_eff"<<","<<"Prl1_err"<<","<<"Prl1_ne_eff"<<","<<"Prl1_ne_err"<<","<<"Prl2_eff"<<","<<"Prl2_err"<<","<<"Prl2_ne_eff"<<","<<"Prl2_ne_err"  <<","<<"Cer_eff"<<","<<"Cer_err"<<","<<"Cer_ne_eff"<<","<<"Cer_ne_eff"<<","<<"Tot_eff"<<","<<"Tot_err"<<","<<"Tot_ne_eff"<<","<<"Tot_ne_err" <<"\n";
  vector<double> pid1;
  vector<double> pid;


  for(unsigned int i=1;i<16;i++)
  {

    pid=PID_layer(Form("%d",i),tgt,4);
    pid.insert(pid.begin(),i);

/*
    pid1=PID_layer(Form("%d",i),tgt,1);
    pid.push_back(pid1[0]); //1
    pid.push_back(pid1[1]);
    pid.push_back(pid1[2]);
    pid.push_back(pid1[3]); //4
    pid1.clear();
    pid1=PID_layer(Form("%d",i),tgt,2);
    pid.push_back(pid1[0]); //5
    pid.push_back(pid1[1]);
    pid.push_back(pid1[2]);
    pid.push_back(pid1[3]); //8
    pid1.clear();
    pid1=PID_layer(Form("%d",i),tgt,3);
    pid.push_back(pid1[0]); //9
    pid.push_back(pid1[1]);
    pid.push_back(pid1[2]);
    pid.push_back(pid1[3]); //12
    pid1.clear();
*/




    double tot_e_eff = pid[1]*pid[5]*pid[9];
    double tot_e_err = sqrt( pow(pid[2],2)+pow(pid[6],2)+pow(pid[10],2));
    if ( tot_e_err != tot_e_err) tot_e_err = 0.0;
    pid.push_back(tot_e_eff);
    pid.push_back(tot_e_err);

    double tot_ne_eff = (1-(1-pid[3])*(1-pid[7])*(1-pid[11]));
    double tot_ne_err = sqrt( pow(pid[4],2) + pow(pid[8],2) + pow(pid[12],2) );
    if ( tot_ne_err != tot_ne_err) tot_ne_err = 0.0;
    pid.push_back(tot_ne_eff);
    pid.push_back(tot_ne_err);


    for(unsigned int j=0;j<pid.size();j++)
    {
      output<<pid[j];
      cout << Form("%5f", pid[j]) <<"\t";
      if(j!=pid.size()-1)output<<",";
    }
    cout <<"\n\n";
    output<<"\n";
    pid.clear();

  }


  output.close();
}



vector <double> PID_read_csv(string kin="", string tgt="")
{
  vector <double> tot_pid={0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0};
  string line;
  ifstream input; input.open(Form("./PID_allkin_%s.csv",tgt.c_str()));
  if(!input.is_open()){cout << "File issues " <<endl;return tot_pid;}
  vector <double> invec;
  getline(input,line);

  while(input.good())
  {
    getline(input,line);
    //cout <<line <<endl;
    invec = parse_csv_d(line,',');
    if( invec[0] == stod(kin)) break;
    invec.clear();
  }
  int A[8]={1,2,5,6,9,10,15,16};
  //cout << invec[0] <<"  "<< invec[1] <<endl;
//cout << invec[0] << "  " << invec[9] << "  " << invec[10]<<"  " << invec[14] << "  " << invec[15]<<endl;
  for(int i=0; i<8;i++)
    {
      if(invec[A[i]]!=invec[A[i]]){
        if(i%2 == 1){tot_pid[i] = 0.0;}
        if(i%2 == 0){tot_pid[i] = 1.0;}
      }
      else{tot_pid[i] = invec[A[i]]; }
    }

    return tot_pid;
}




/*
string l_sh_c = "(L.prl1.e+L.plr2.e)/(L.gold.p*1000)";
string r_sh_c = "(R.sh.e+R.ps.e)/(R.gold.p*1000)";
string sh_cut= l_sh_c;
if(ARM=="R")sh_cut=r_sh_c;
string ele_sample_cut;

for(int i =0;i<20;i++)
{
  ele_sample_cut=Form("%s>%f&&%s<%f",sh_cut.c_str(),ele_min_lim,sh_cut.c_str(),ele_max_lim);
*/

/*
vector<int> runlist = SQL_Kin_Target(kin,tgt);
if(runlist.size()==0)exit(1);
TChain *T = new TChain("T");
for(unsigned int i =0; i<runlist.size();i++)
{
  T->Add(LoadRun(runlist[i]));
  int total_ent = T->GetEntries();
  if(total_ent>=1000000)break;
  if(i>=10){break;}
}*/
