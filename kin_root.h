#include "./inc.h"
#include "./TRI_Main.h"
#include "./TRI_Tools.h"

void kin_root(TString filename=""){

	//input the name opf the kin file and parse it
	if(filename==""){ cout<<"Input file name:   "; cin>>filename;}
	filename = "./Runlist/" + filename;
    ifstream file(filename.Data());
    if(!file.good()){cout << filename.Data() << " does not exist! "<<endl; exit(1);}
    TString content;
    TString Target,Kin,Run_String;
    const char* kin;
    for (int ii=0; content.ReadLine(file) && ii<3;ii++ ) {  
    	cout<<"!!!:  "<<content<<endl;
        if(ii==0)Target = content;
        if(ii==1){
           kin = content.Data();
           Kin = Form("Kin%s", kin);
         }
        if(ii==2)Run_String = content;         
      }
      file.close();
   ///////////////////////////////////////////////////
   
    const vector<Int_t> RunNoChain=gGet_RunNoChain(Run_String);
    TChain *T = gGetTree(RunNoChain,"T");
    
    
}
