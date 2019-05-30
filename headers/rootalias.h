// For Tritium:
// extracting info from rootfiles and other useful functions
// adapted from Ou,Longwu's GMp code
// -shujie 03,2018

#ifndef ROOTALIAS_H
#define ROOTALIAS_H


#include <string>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>
#include <locale>
#include <algorithm>
#include <array>
#include <vector>
#include "TSQLServer.h"
#include "TSQLResult.h"
#include "TSQLRow.h"
#include "TTree.h"
#include "TROOT.h"
#include "TStyle.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "TH1.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TLatex.h"
#include "TLeaf.h"
#include "TGraphErrors.h"
#include "TSystem.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TH2.h"
#include "TLine.h"
#include "TEllipse.h"
#include "TCut.h"
#include "TProfile.h"
#include "TFile.h"
#include "TEventList.h"
#include "TSpectrum.h"
#include "TArray.h"
#include "THaRun.h"
#include "THaRunParameters.h"
#include "TVector.h"


const char* ROOTPATHS[] = {
  "/storage/MARATHONRootfiles/pass2/kin0/",
  "/storage/MARATHONRootfiles/pass2/kin1/",
  "/storage/MARATHONRootfiles/pass2/kin2/",
  "/storage/MARATHONRootfiles/pass2/kin3/",
  "/storage/MARATHONRootfiles/pass2/kin4/",
  "/storage/MARATHONRootfiles/pass2/kin5/",
  "/storage/MARATHONRootfiles/pass2/kin7/",
  "/storage/MARATHONRootfiles/pass2/kin9/",
  "/storage/MARATHONRootfiles/pass2/kin11/",
  "/storage/MARATHONRootfiles/pass2/kin13/",
  "/storage/MARATHONRootfiles/pass2/kin15/",
  "/storage/MARATHONRootfiles/pass2/kin16/",
  "/storage/MARATHONRootfiles/pass2/kin7_1st/",
  "/storage/MARATHONRootfiles/pass2/kin9_1st/",
  "/storage/MARATHONRootfiles/pass2/kin11_1st/",
  "/storage/MARATHONRootfiles/pass2/kin13_1st/",
  "/storage/MARATHONRootfiles/pass2/kin15_1st/",
  "/storage/MARATHONRootfiles/pass2/kin16_1st/",
  "/storage/MARATHONRootfiles/pass2/kin7_2nd/",
  "/storage/MARATHONRootfiles/pass2/kin9_2nd/",
  "/storage/MARATHONRootfiles/pass2/kin11_2nd/",
  "/storage/MARATHONRootfiles/pass2/kin13_2nd/",
  "/storage/MARATHONRootfiles/pass2/kin15_2nd/",
  "/storage/MARATHONRootfiles/pass2/kin15_3rd/",
  "/storage/MARATHONRootfiles/pass2/kin16_2nd/",
  "/storage/MARATHONRootfiles/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin1/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin4/",
  "/run/media/jbane/Slim/MARATHONRoots/",
  "/storage/MARATHONRootfiles/",
  "/storage/MARATHONRootfiles/pass1/kin0/",
  "/storage/MARATHONRootfiles/pass1/kin1/",
  "/storage/MARATHONRootfiles/pass1/kin2/",
  "/storage/MARATHONRootfiles/pass1/kin3/",
  "/storage/MARATHONRootfiles/pass1/kin4/",
  "/storage/MARATHONRootfiles/pass1/kin5/",
  "/storage/MARATHONRootfiles/pass1/kin7/",
  "/storage/MARATHONRootfiles/pass1/kin9/",
  "/storage/MARATHONRootfiles/pass1/kin11/",
  "/storage/MARATHONRootfiles/pass1/kin13/",
  "/storage/MARATHONRootfiles/pass1/kin15/",
  "/storage/MARATHONRootfiles/pass1/kin16/",
  "/storage/MARATHONRootfiles/pass1/optics/",
  "/storage/MARATHONRootfiles/pass1/positron/",
  "/volatile/halla/triton/Marathon_Rootfiles/optics_trial/",
  "./tmproot/",
  "/home/shujie/jlab/MyTritium/Rootfiles/",
  "/volatile/halla/triton/nathaly/Rootfiles/",
  "/volatile/halla/triton/eep_Rootfiles/pass1/",
  "/volatile/halla/triton/Marathon_Rootfiles/pass1_calibration/",
  "/volatile/halla/triton/Marathon_Rootfiles/pass1/",
  "/volatile/halla/triton/Marathon_Rootfiles/pass1_fix/",
  "/chafs1/work1/tritium/Tritium_Rootfiles/",
  "/chafs1/work1/tritium/Rootfiles/",
  "/volatile/halla/triton/shujie/replay/Rootfiles/",
  "/volatile/halla/triton/Tritium_Rootfiles/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin1/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin2/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin3/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin4/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin5/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin7/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin9/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin11/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin13/"
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin15/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2/kin16/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin1/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin2/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin3/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin4/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin5/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin7/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin9/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin11/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin13/"
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin15/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1/kin16/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin0/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin1/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin2/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin3/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin4/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin5/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin7/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin9/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin11/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin13/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin15/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin16/",
  "./",
  0
};
const char* CALPATH[] ={
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin0/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin1/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin2/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin3/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin4/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin5/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin7/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin9/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin11/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin13/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin15/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass2_calibration/kin16/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin0/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin1/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin2/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin3/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin4/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin5/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin7/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin9/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin11/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin13/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin15/",
  "/v/lustre2/expphy/cache/halla/triton/prod/marathon/pass1_calibration/kin16/",
  "./",
  0
};

const char* MCPATHS[] = {
	"/run/media/jbane/Slim/model0/",
  "/run/media/jbane/Slim/model1/",
	"./mcroot/",
	0
};

int    G_debug=1;
const double pi=3.1415926535897932;
const double rad=pi/180.0;
const double wsqr=2.5;
//-----------LHRS-----------------------------------------

//quickdraw string
string QDS_cal_cer_L= "L.cer.asum_c:(L.prl1.e+L.prl2.e)/(HacL_D1_P0rb*1000) >>H(100,0,1.5,1000,0,10000)";

string delta_ray_ep_cut ="";



//Cut file name
string cuttype = "tightcut/";

const double beta_min_L=0.5;
const double sh_min_L=0.7;
const double prl1_min=1.0;
const double prl2_min=0.6;

const double cer_min_L=1800;
const double dnew_thres = 0.1;

TLine *Line_cer_cut_cal_cer = new TLine(0,cer_min_L,4,cer_min_L);

TCut L_dnew = Form("LeftBCMev.current_dnew>=%f",dnew_thres);
TCut R_dnew = Form("RightBCMev.current_dnew>=%f",dnew_thres);
TCut sh_cut_L       = Form("(L.prl1.e+L.prl2.e)>HacL_D1_P0rb*1000*%g",sh_min_L);
TCut cer_cut_L      = Form("L.cer.asum_c>%g",cer_min_L);
TCut beta_cut_L     = Form("L.tr.beta>%g",beta_min_L);
TCut electron_cut_L = cer_cut_L+sh_cut_L+beta_cut_L;

TCut l1_cut_L =Form("(L.prl1.e/1000.0)>%g",prl1_min);
TCut l2_cut_L =Form("(L.prl2.e/1000.0)>%g",prl2_min);

TCut layers_electron_cut_L =cer_cut_L+beta_cut_L+l1_cut_L+l2_cut_L;

TCut inv_m_L	    = Form("EKLx.W2>=%g",wsqr);

TCut track_L        = "L.tr.n==1";
TCut aperture_L     = "((L.tr.tg_y+L.tr.tg_ph*1.5)^2/0.64+(L.tr.tg_th*1.5)^2/0.49)<0.01";
TCut mc_ap_L = "((ytar*100+yptar*1.5)^2/0.64 + (xptar*1.5)^2/0.49) < 0.01";

double tg_dp_L=0.04;
double tg_th_L=0.04;
double tg_ph_L=0.025;//0.03 ; //
double tg_vz_L=0.09;
double tg_dp_L_min=-0.035;// -0.04;//
double tg_th_L_min=-0.04;
double tg_ph_L_min=-0.025; // -0.03 ;//
double tg_vz_L_min=-0.07;
TCut dp_cut_L_max=Form("(L.tr.tg_dp)<%g",tg_dp_L);
TCut th_cut_L_max=Form("(L.tr.tg_th)<%g",tg_th_L);
TCut ph_cut_L_max=Form("(L.tr.tg_ph)<%g",tg_ph_L);
TCut dp_cut_L_min=Form("(L.tr.tg_dp)>%g",tg_dp_L_min);
TCut th_cut_L_min=Form("(L.tr.tg_th)>%g",tg_th_L_min);
TCut ph_cut_L_min=Form("(L.tr.tg_ph)>%g",tg_ph_L_min);

TCut dp_cut_L = dp_cut_L_max&&dp_cut_L_min;
TCut th_cut_L = th_cut_L_max&&th_cut_L_min;
TCut ph_cut_L = ph_cut_L_max&&ph_cut_L_min;

string dp_cut_L_s = Form ("fabs(L.tr.tg_dp)<%g" , tg_dp_L);
string th_cut_L_s = Form ("fabs(L.tr.tg_th)<%g" , tg_th_L);
string ph_cut_L_s = Form("fabs(L.tr.tg_ph)<%g" , tg_ph_L);
TCut spec_L  =Form("%s&&%s&&%s", dp_cut_L_s.c_str(), th_cut_L_s.c_str(), ph_cut_L_s.c_str());
TCut z_cut_L =Form("(L.tr.vz)<%g && (L.tr.vz)>%g",tg_vz_L,tg_vz_L_min);
TCut acc_cut_L=dp_cut_L+th_cut_L+ph_cut_L+z_cut_L;//+aperture_L;
const double tg_dp_L_e=0.035;
const double tg_th_L_e=0.035;
const double tg_ph_L_e=0.02;
TCut dp_cut_L_e  = Form("fabs(L.tr.tg_dp)<%g",tg_dp_L_e);
TCut th_cut_L_e  = Form("fabs(L.tr.tg_th)<%g",tg_th_L_e);
TCut ph_cut_L_e  = Form("fabs(L.tr.tg_ph)<%g",tg_ph_L_e);
TCut acc_cut_L_e = dp_cut_L_e+th_cut_L_e+ph_cut_L_e+z_cut_L+aperture_L;
TCut L_mara_trig = "DL.evtypebits&(1<<2)";

TCut L_total_cut =acc_cut_L+electron_cut_L+inv_m_L;



string mc_yptar_cut = Form("yptar>=%.3f && yptar <=%.3f",tg_ph_L_min,tg_ph_L);
string mc_xptar_cut = Form("xptar>=%.3f && xptar <=%.3f",tg_th_L_min,tg_th_L);
string mc_dp_cut = Form("delta>=%.3f && delta <=%.3f", tg_dp_L_min*100,tg_dp_L*100);
string mc_vz_cut = Form("ztar>=%.3f && ztar<=%.3f",tg_vz_L_min*100,tg_vz_L*100);
TCut total_mc_cut = Form("%s && %s && %s && %s",mc_yptar_cut.c_str(), mc_xptar_cut.c_str(), mc_dp_cut.c_str(), mc_vz_cut.c_str());
//",mc_vz_cut.c_str());//

//--------------RHRS-------------------------------------
const double beta_min_R=0.6;
const double sh_min_R=0.8;
const double cer_min_R=2000;

TCut sh_cut_R       = Form("(R.ps.e+R.sh.e)>HacR_D1_P0rb*1000*%g",sh_min_R);
TCut cer_cut_R      = Form("R.cer.asum_c>%g",cer_min_R);
TCut beta_cut_R     = Form("R.tr.beta>%g",beta_min_R);
TCut electron_cut_R = cer_cut_R+sh_cut_R+beta_cut_R;
TCut track_R        = "R.tr.n==1";

const double tg_dp_R=0.05;
const double tg_th_R=0.07;
const double tg_ph_R=0.05;
const double tg_vz_R=0.07;
TCut dp_cut_R  = Form("fabs(R.tr.tg_dp)<%g",tg_dp_R);
TCut th_cut_R  = Form("fabs(R.tr.tg_th)<%g",tg_th_R);
TCut ph_cut_R  = Form("fabs(R.tr.tg_ph)<%g",tg_ph_R);
TCut z_cut_R   = Form("fabs(R.tr.vz)<%g",tg_vz_R);
TCut acc_cut_R = dp_cut_R+th_cut_R+ph_cut_R+z_cut_R;//+aperture_L;
const double tg_dp_R_e=0.035;
const double tg_th_R_e=0.035;
const double tg_ph_R_e=0.02;
TCut dp_cut_R_e  = Form("fabs(R.tr.tg_dp)<%g",tg_dp_R_e);
TCut th_cut_R_e  = Form("fabs(R.tr.tg_th)<%g",tg_th_R_e);
TCut ph_cut_R_e  = Form("fabs(R.tr.tg_ph)<%g",tg_ph_R_e);
TCut acc_cut_R_e = dp_cut_R_e+th_cut_R_e+ph_cut_R_e+z_cut_R;//+aperture_L;

TCut inv_m_R	    = Form("EKRx.W2>=%g",wsqr);



//--------------------------------
// Chain rootfiles given run number, taken from Longwu Ou's GMP code
//--------------------------------

TChain* LoadRun(Int_t run, const char* path, const char* tree, Int_t debug)
{
  TChain* tt       = new TChain(tree);
  TString basename = Form("tritium_%d",run);
  TString rootfile = basename + ".root";
  TString dir      = path;
  Long_t  split    = 0;

  if (!dir.EndsWith("/")) dir.Append("/");
  rootfile.Prepend(dir.Data());

  while ( !gSystem->AccessPathName(rootfile.Data()) ) {
    tt->Add(rootfile.Data());
    if(debug)   cout << "ROOT file " << rootfile << " added to " << tree<<" tree"<<endl;
    split++;
    rootfile = basename + "_" + split + ".root";
    rootfile.Prepend(dir.Data());
  }

  if (split<=0) {
   if (debug>1) cout << "Can not find ROOT file for run " << run << " at " <<path << endl;
   delete tt;
   tt = 0;
  }
	else{ if(G_debug && debug)cout << "Using " <<run <<" located at " << path<<endl;}
  return tt;
}

// Load TTree "tree" form all the ROOT files generated from a specific run
// The ROOT files are searched for from the given "ROOTPATHS" array given at the beginning of the file
//      run  --- run number
//      tree --- The name of the tree to be loaded
TChain* LoadRun(Int_t run, const char* tree = "T",int debug=0)
{
  Int_t   i  = 0;
  TChain* tt = 0;
  while (ROOTPATHS[i]) {
   tt = LoadRun(run,ROOTPATHS[i++],tree,debug);
   if (tt) break;
  }
//    if (!tt)
//  cerr << "Can not find ROOT file for run " << run << endl;
  return tt;
}


TChain* LoadP1(Int_t run, const char* tree = "T")
{
  Int_t   i  = 0;
  TChain* tt = 0;
  while (ROOTPATHS[i]) {
    string pp = Form("%s",ROOTPATHS[i]);

    if( pp.find("pass1")!=pp.npos){
        tt = LoadRun(run,ROOTPATHS[i],tree,0);
    }
    i++;
    if (tt) break;

  }
//    if (!tt)
//  cerr << "Can not find ROOT file for run " << run << endl;
  return tt;
}



//----------------------------
// Load online replay
//----------------------------


TChain* LoadOnline(Int_t run, const char* path, const char* tree,Int_t debug)
{
    TChain* tt = new TChain(tree);

    TString  basename = Form("tritium_online_%d",run);
    TString  rootfile = basename + ".root";

    TString dir = path;
    if (!dir.EndsWith("/")) dir.Append("/");

    rootfile.Prepend(dir.Data());

    Long_t split = 0;

    while ( !gSystem->AccessPathName(rootfile.Data()) ) {
  tt->Add(rootfile.Data());
 if(debug){ cout << "ROOT file " << rootfile << " added to " << tree<<" tree"<<endl;}
  split++;
  rootfile = basename + "_" + split + ".root";
  rootfile.Prepend(dir.Data());

    }

    if (split<=0) {
  if (debug>0) cout << "Can not find online replay file for run " << run << endl;
  delete tt;
  tt = 0;
    }

    return tt;
}

TChain* LoadOnline(Int_t run, const char* tree = "T")
{
    Int_t i=0;

    TChain* tt = 0;
    while (ROOTPATHS[i]) {
      tt = LoadOnline(run,ROOTPATHS[i++],tree,0);
      if (tt) break;
    }

 //    if (!tt)
  // cerr << "Can not find online replay file for run " << run << endl;

    return tt;
}

TChain* LoadCalib(Int_t run, const char* path, const char* tree,Int_t debug)
{
    TChain* tt = new TChain(tree);

    TString  basename = Form("tritium_%d",run);
    TString  rootfile = basename + ".root";

    TString dir = path;
    if (!dir.EndsWith("/")) dir.Append("/");

    rootfile.Prepend(dir.Data());

    Long_t split = 0;

    while ( !gSystem->AccessPathName(rootfile.Data()) ) {
  tt->Add(rootfile.Data());
 if(debug){ cout << "ROOT file " << rootfile << " added to " << tree<<" tree"<<endl;}
  split++;
  rootfile = basename + "_" + split + ".root";
  rootfile.Prepend(dir.Data());

    }

    if (split<=0) {
  if (debug>0) cout << "Can not find online replay file for run " << run << " at "<< path<< endl;
  delete tt;
  tt = 0;
    }

    return tt;
}

TChain* LoadCalib(Int_t run, const char* tree = "T")
{
    Int_t i=0;

    TChain* tt = 0;
    while (CALPATH[i]) {
  tt = LoadCalib(run,CALPATH[i++],tree,1);

  if (tt) break;
    }

 //    if (!tt)
  // cerr << "Can not find online replay file for run " << run << endl;

    return tt;
}



TChain* LoadMC(Int_t run, int model=0,int tarid=0, const char* tree = "h9040")
{
	TString tgt="";

  string mcpth = MCPATHS[model];

	if(tarid>0){ tgt=Form("_%d",tarid);}
	TChain *tt = new TChain(tree);
	tt->Add(Form("%smc%d%s.root",mcpth.c_str(),run,tgt.Data()));
	if(tt->GetEntries()==0){return nullptr;}
	else{cout << "adding " <<Form("%smc%d%s.root",mcpth.c_str(),run,tgt.Data())<<"\n";}
	return tt;
}
// get rootfile path

TString GetPath(Int_t run)
{
    Int_t i=0;
    TString rootpath="unknown";

    TChain* T = 0;
    while (ROOTPATHS[i]) {
  T = LoadRun(run,ROOTPATHS[i++],"T",0);
  if (T) {
   rootpath=ROOTPATHS[i-1];
    return rootpath;
  }
    }

    if (!T)
  cout << "Can not find ROOT file for run " << run << endl;

    return rootpath;
}

//-----------------------
// Get the THaRun object from Ttree;
//-----------------------
THaRun* GetRunData(TTree* tt)
{
    THaRun* run = 0;
    if (!tt->GetDirectory()) tt->LoadTree(0); // Necessary if T is a TChain
    TDirectory* fDir = tt->GetDirectory();
    if (fDir) fDir->GetObject("Run_Data",run);
    if (run) {
      // cout<<"========Run_Data from rootfile=========\n";
      // run->Print();
      // cout<<"_______________________________________\n";
      return run;
    }

    return 0;
}


// ---------------------------
// Return PS for a given Ttree
// ---------------------------
TArrayI GetPS(TTree* tt)
{
    THaRun* run = GetRunData(tt);
    if (run) {
      TArrayI ps = run->GetParameters()->GetPrescales();
      delete run;
      return ps;
    }
    return 0;

}

Int_t GetPS(TTree* tt,Int_t trigger)
{
    THaRun* run = GetRunData(tt);
    Int_t ps=0;
    if (run) {
       ps = run->GetParameters()->GetPrescales()[trigger-1];
      delete run;
    }
    return ps;
}

// ---------------------------
// Return rawdata taken time (as unix /epoch time) for a given Ttree
// t2day = 1: return days since the Tritium cell filled
//
// ---------------------------
Int_t GetTimeStamp(TTree* tt, Int_t decay_days=0)
{

    THaRun* run = GetRunData(tt);
    TDatime datetime = run->GetDate();

    // cout<<datetime.AsString()<<endl;
    Int_t timestamp = datetime.Convert();    // convert to epoch time

    if (decay_days==1){
        vector<Int_t> filldate={1508774400};// the first tritium cell filled on 2017.10.23. please add newer date in ascending order
        Int_t days = 0;
        for (Int_t dd: filldate){
          if(timestamp > dd){
            days = (timestamp - dd)/24/3600;
          }
        }
        return days;

    }
    return timestamp;
}



// called by sort function to do descending sort
// bool wayToSort(int i, int j) { return i > j; }
// bool wayToSort(double i, double j) { return i > j; }
bool isGreater(double i, double j)
{
    return i > j;
}



// poisson function for cherenkov PMT # of photons
Double_t pois_fit(Double_t *x, Double_t *par)
{
   Double_t arg = 0;
  // par[1]-->normalization (y-axis), par[2]-->Lambda, par[0]=scale (x-axis)
   arg = x[0]*1.0/par[0];
   // Poisson=Power(par[1],x)/TMath::Gamma(x+1)/TMath::Exp(par[1])
   //        =prob. of occuring x times if the averg. event rate is par[2]
   // x[0]-->k
   Double_t fitval = par[1]*TMath::Poisson(arg,par[2]);
   //   cout<<fitval<<endl;
   return fitval;
}


vector<string> cut_branches_L ={"L.tr*", "L.cer.asum_c", "L.prl*.e", "Hac*_D1_P0rb", "D*.evtypebits", "EKLx*" };


#endif
