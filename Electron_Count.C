#include "TRI_Tools.h"

void Electron_Count(TString filename= ""){
/*    TString Target = ""; cout<<"--- Target (H2,D2, H3, He3, Empty, Dummy) : "; cin >> Target;
    int kin = 0; cout<<"--- Kin (1 or 2 or 3 ...) : "; cin >> kin;
    TString Kin = Form("Kin%d", kin); 
    TString Run_String = ""; cout<<"--- List of Runs (1234,2345,3456,...): "; cin >> Run_String;
*/

	if(filename==""){
     cout<<"!!! If you don't know how to use it, read README "<<endl;
     cout<<"Input file name:   "; cin>>filename;
	}
     filename = "./Runlist/" + filename;
     ifstream file(filename.Data());
     TString content;
     TString Target,Kin,Run_String;
     const char* kin;
     for (int ii=0; content.ReadLine(file) && ii<3;ii++ )
      {  cout<<"!!!:  "<<content<<endl;
         if(ii==0)Target = content;
         if(ii==1){
            kin = content.Data();
            Kin = Form("Kin%s", kin);
          }
         if(ii==2)Run_String = content;         
      }
      file.close();


    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
    
    TString Arm = ""; //cout<<"--- L or R (HRS-L or HRS-R ) : "; cin >> Arm;
    TString arm = "";
    if(RunNoChain[0]<=50000)
	   { Arm = "L"; arm = "l";}
    else		    
	   { Arm = "R"; arm = "r";}

    TChain* T_Tree =(TChain*) gGetTree(RunNoChain, "T");

    TString cut_L = "DL.evtypebits>>2&1 && L.tr.n==1 ";
    TString cut_e_L = "DL.evtypebits>>2&1 && L.tr.n==1 && L.cer.asum_c>2000. && (L.prl1.e+L.prl2.e)/(L.gold.p*1000.)>0.7 && abs(L.tr.vz)<0.1 && L.tr.tg_ph<0.02 && L.tr.tg_ph>-0.03 && L.tr.tg_th<0.055 && L.tr.tg_th>-0.05 && abs(L.tr.tg_dp)<0.04";
    TString cut_R = "DR.evtypebits>>5&1 && R.tr.n==1 ";
    TString cut_e_R = "DR.evtypebits>>5&1 && R.tr.n==1 && R.cer.asum_c>1000. && (R.ps.e+R.sh.e)>2000. && abs(rpr.z)<0.075 && abs(R.tr.tg_ph)<0.025 && abs(R.tr.tg_th)<0.04 && abs(R.tr.tg_dp)<0.04";


	TString cut;
	TString cutE;
	
	//cout;
	
	if(Arm=="L"){cut=cut_L;cutE=cut_e_L;}
		else{cut=cut_R;cutE=cut_e_R;}

    cout<<"--- Good Electron Cut: "<< cutE<<endl;

    int Total_Ele= T_Tree->GetEntries(TCut(cut));
    int Good_Ele= T_Tree->GetEntries(TCut(cutE));
    cout<<endl; 
    cout<<Form("--- For %s at %s on HRS-%s:  ", Target.Data(), Kin.Data(), Arm.Data())<<endl;
    cout<<"--- Total Good Electrons from these runs are = "<<Good_Ele<<endl;


    gStyle->SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    TH1F *h1 = new TH1F("h1","",250, -0.5,0.5);
    h1->SetXTitle("ReactZ (m)");
    T_Tree->Draw(Form("rp%s.z>>h1",arm.Data()),TCut(cut));
    T_Tree->SetLineColor(2);
    T_Tree->Draw(Form("%s.tr.vz",Arm.Data()),TCut(cutE),"same");

    TLatex *t1 = new TLatex();
    t1->SetNDC();
    t1->SetTextFont(32);
    t1->SetTextSize(0.06);
    t1->SetTextColor(1); 
    t1->DrawLatex(0.2, 0.92, Form("Target = %s at %s on HRS-%s ",Target.Data(), Kin.Data(), Arm.Data() ));

    t1->SetTextColor(4); 
    t1->SetTextSize(0.055);
    t1->DrawLatex(0.2, 0.70, Form("Total Good Electrons = %4.1f K (out of %4.1fK) ", (Good_Ele/1000.), (Total_Ele/1000.) ));
//    t1->DrawLatex(0.2, 0.60, Form("Total charge = %f uC", charge));


    t1->SetTextColor(6); 
    t1->SetTextSize(0.04);
    t1->DrawLatex(0.15, 0.32, Form("Run-List = %s",Run_String.Data() ));
    c1->Print(Form("good_ele/%s_%s_%s_%d.png", Target.Data(),Kin.Data(),Arm.Data(), RunNoChain[0]));
    c1->Print(Form("good_ele/%s_%s_%s_%d.pdf", Target.Data(),Kin.Data(),Arm.Data(), RunNoChain[0]));
}
