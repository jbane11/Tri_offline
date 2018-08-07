#include "THaRun.h"
#include "THaRunParameters.h"
// The paths to store ROOT files
// adapted from Ou,Longwu's GMp code
// -shujie 03,2018

struct target
{
  Double_t pos;
  TString name;
};


const char* PATHS[] = {
	
      	"/volatile/halla/triton/Marathon_Rootfiles/pass1/",
	"/chafs1/work1/tritium/pass1/",
	"/chafs1/work1/tritium/pass1_fix/",
	"/chafs1/work1/tritium/Rootfiles/",
   	"/chafs1/work1/tritium/Tritium_Rootfiles/",
  	"/volatile/halla/triton/Tritium_Rootfiles/",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin1",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin2",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin3",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin4",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin5",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin6",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin7",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin8",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin9",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin10",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin11",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin12",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin13",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin14",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin15",
	"/lustre/expphy/cache/halla/triton/prod/marathon/pass1/kin16",
  //    "/volatile/halla/triton/Marathon_Rootfiles/pass1/",
  "./",
  0
};

const char* PATHS_2[] = {
	"./rootfiles",
	"/volatile/halla/triton/Marathon_Rootfiles/pass1_calibration/",
	"/cache/halla/triton/prod/pass1_calibration/kin1",
	"/cache/halla/triton/prod/pass1_calibration/kin2",
	"/cache/halla/triton/prod/pass1_calibration/kin3",
	"/cache/halla/triton/prod/pass1_calibration/kin4",
	"/cache/halla/triton/prod/pass1_calibration/kin5",
	"/cache/halla/triton/prod/pass1_calibration/kin6",
	"/cache/halla/triton/prod/pass1_calibration/kin7",
	"/cache/halla/triton/prod/pass1_calibration/kin8",
	"/cache/halla/triton/prod/pass1_calibration/kin9",
	"/cache/halla/triton/prod/pass1_calibration/kin10",
	"/cache/halla/triton/prod/pass1_calibration/kin11",
	"/cache/halla/triton/prod/pass1_calibration/kin12",
	"/cache/halla/triton/prod/pass1_calibration/kin13",
	"/cache/halla/triton/prod/pass1_calibration/kin14",
	"/cache/halla/triton/prod/pass1_calibration/kin15",
	"/cache/halla/triton/prod/pass1_calibration/kin16",
	0
};

//-------------------------------------------
//get target info from epics encoder position
// check if target moved during the run
// works for tritium snice 2018.1
//----------------------------------------- 
TString GetTarget(TChain *tree2){
  TString targname;
  target t2={33106235,"H3"};
  target d2={29367355,"D2"};
  target h={25610043,"H2"};
  target he3={21875520,"He3"};
  target empty={18119995,"EM"};
  target dummy={15175217,"DM"};
  target optics={14394929,"Multifoils"};
  target hole={13108119,"Carbon Hole"};
  target raster={12444209,"Raster Target"};
  target al={11728945,"Thick Aluminum"};
  target single={11013681,"Single Carbon Foil"};
  target ti={10298417,"Titanium"};
  target beo={9583153,"BeO"};

  Double_t pos,pos1;
  tree2->SetBranchAddress("haBDSPOS",&pos);
  Int_t nn=tree2->GetEntries(); 
  tree2->GetEntry(Int_t(1+nn/100)); 
  pos1=pos;
  tree2->GetEntry(Int_t(nn-1)); 
  if (pos1!=pos) targname="target moved during the run"; 
  else if(abs(pos)<= 0.000000001 ) {targname =" Unkown - Please double check logbook ";}
  else if(abs(pos)<50) targname="HOME";
  else if(abs(pos-t2.pos)<20000)     targname=t2.name;
  else if(abs(pos-d2.pos)<20000)     targname=d2.name;
  else if(abs(pos-h.pos)<20000)      targname=h.name;
  else if(abs(pos-he3.pos)<20000)    targname=he3.name;
  else if(abs(pos-empty.pos)<20000)  targname=empty.name;
  else if(abs(pos-dummy.pos)<50)  targname=dummy.name;
  else if(abs(pos-optics.pos)<50) targname=optics.name;
  else if(abs(pos-hole.pos)<50)   targname=hole.name;
  else if(abs(pos-raster.pos)<50) targname=raster.name;
  else if(abs(pos-al.pos)<50)     targname=al.name;
  else if(abs(pos-single.pos)<50) targname=single.name;
  else if(abs(pos-ti.pos)<50)     targname=ti.name;
  else if(abs(pos-beo.pos)<50)    targname=beo.name;

 // cout<<pos<<"  "<<targname<<endl;
  return targname;
}

int GetKin(TChain *T)
{	
	TChain *Tree=T;
	int KIN=0;
	double kin_table[20][2] ={	{0  ,12.589},
					{1  ,17.577},
					{2  ,19.115},
					{3  ,20.578},
					{4  ,0.0},
					{5  ,23.213},
					{6  ,0.0},
					{7  ,25.594},	
					{8  ,0.0},
					{9  ,27.778},
					{10 ,0.0},
					{11 ,29.917},
					{12 ,0.0},
					{13 ,31.732},	
					{14 ,0.0},
					{15 ,33.562},
					{16 ,36.12},
					{17 ,0.0},
					{18 ,0.0},	
					{19 ,0.0},
						};

	double angle_sep=0;
	Double_t int_angle=0;
	TString Arm="L";
 //way to check which Arm, if the tree is already open:
 	TBranch *ArmCheck = T->FindBranch(Form("%s.tr.n",Arm.Data()));
        if(ArmCheck==nullptr){
        	if(Arm=="L")Arm="R";
		}                              
 //

	Tree->SetBranchAddress(Form("Hac%s_alignAGL",Arm.Data()), &int_angle);
	Tree->GetEntry(1);
//	cout << "Debug: angle  : "<< int_angle<< " arm " << Arm <<endl;
	for(int i =0; i<20;i++){
		angle_sep = abs(int_angle-kin_table[i][1]); 
		if(angle_sep<=0.09){KIN=kin_table[i][0];}
		}

	return KIN;
}




TChain* LoadRun(Int_t run, const char* path, const char* tree, Int_t debug)
{
    TChain* tt = new TChain(tree);

    TString basename = Form("tritium_%d",run);
    TString rootfile = basename + ".root";
    
    TString dir = path;
    if (!dir.EndsWith("/")) dir.Append("/");

    rootfile.Prepend(dir.Data());

    Long_t split = 0;

    while ( !gSystem->AccessPathName(rootfile.Data()) ) {
	tt->Add(rootfile.Data());
if(!debug)cout << "ROOT file " << rootfile << " added to " << tree<<" tree"<<endl;
	split++;
	rootfile = basename + "_" + split + ".root";
	rootfile.Prepend(dir.Data());

    }

    if (split<=0) {
//	if (debug>0) cerr << "Can not find ROOT file for run " << run << endl;
	delete tt;
	tt = 0;
    }

    return tt;
}

// Load TTree "tree" form all the ROOT files generated from a specific run
// The ROOT files are searched for from the given "PATHS" array given at the beginning of the file
//      run  --- run number
//      tree --- The name of the tree to be loaded
TChain* LoadRun(Int_t run, const char* tree = "T")
{
    Int_t i=0;

    TChain* tt = 0;
    while (PATHS[i]) {
	tt = LoadRun(run,PATHS[i++],tree,1);

	if (tt) break;
    }
   
    if (!tt)
	cerr << "Can not find ROOT file for run " << run << endl;

    return tt;
}
TChain* LoadCalib(Int_t run, const char* tree = "T")
{
    Int_t i=0;

    TChain* tt = 0;
    while (PATHS_2[i]) {
        tt = LoadRun(run,PATHS_2[i++],tree,1);

        if (tt) break;
    }

    if (!tt)
        cerr << "Can not find ROOT file for run " << run << endl;

    return tt;
}


// Return PS of a given run
TArrayI GetPS(TTree* tt)
{
  //  TDatime run_time("2018-01-01 00:00:00");
 
    THaRun* run = 0;
    if (!tt->GetDirectory()) tt->LoadTree(0); // Necessary if T is a TChain
    TArrayI ps;
    TDirectory* fDir = tt->GetDirectory();
    if (fDir) fDir->GetObject("Run_Data",run);
    if (run) {
      ps = run->GetParameters()->GetPrescales();
      delete run;
    }

    return ps;
}

Int_t GetPS(TTree* tt,Int_t trigger)
{
  //  TDatime run_time("2018-01-01 00:00:00");
    if(trigger<=0){return 0;}
    THaRun* run = 0;
    Int_t ps;
    if (!tt->GetDirectory()) tt->LoadTree(0); // Necessary if T is a TChain
    TDirectory* fDir = tt->GetDirectory();
    if (fDir) fDir->GetObject("Run_Data",run);
    if (run) {
       ps = run->GetParameters()->GetPrescales()[trigger-1];
      delete run;
    }
    return ps;
}

double GetBeamE(TTree* beamt)
{
	beamt->ResetBranchAddresses();
	//which arm
	string Arm = "L";
	if(beamt->FindBranch("L.tr.tg_dp") == nullptr ) Arm ="R";
	double CE;
	beamt->SetBranchAddress("HALLA_p",&CE);
	beamt->GetEntry(2);
	return CE;
}

double GetSetTheta(TTree *thet)
{
	thet->ResetBranchAddresses();
	//which arm
	string Arm = "L";
	if(thet->FindBranch("L.tr.tg_dp") == nullptr ) Arm ="R";
	double angle;
	thet->SetBranchAddress(Form("Hac%s_alignAGL",Arm.c_str()),&angle);
	thet->GetEntry(2);
	return angle;
}
	
double GetSetMo(TTree *mot)
{
	mot->ResetBranchAddresses();
	//which arm
	string Arm = "L";
	if(mot->FindBranch("L.tr.tg_dp") == nullptr ) Arm ="R";
	double Mo;
	mot->SetBranchAddress(Form("Hac%s_D1_P0rb",Arm.c_str()),&Mo);
	mot->GetEntry(2);
	return Mo;
}

TString GetTarget(Int_t run)
{
  TChain* etree=LoadRun(run,"E");
  TString targname=GetTarget(etree);
  return targname;
  
}


// get rootfile path

TString GetPath(Int_t run)
{
    Int_t i=0;
    TString rootpath="unknown";

    TChain* T = 0;
    while (PATHS[i]) {
	T = LoadRun(run,PATHS[i++],"T",0);
	if (T) {
	 rootpath=PATHS[i-1];
	  return rootpath;
	}
    }

    if (!T)
	cerr << "Can not find ROOT file for run " << run << endl;

    return rootpath;
}
vector<TString> Tokens1(TString aline,TString aDelim=",")
{
	Int_t i;
	TObjArray* InObjArray;
	TObjString* os;
	TString s;
	vector<TString> OutStringVec;
	OutStringVec.clear();

	InObjArray=aline.Tokenize(aDelim);
	for ( i=0; i<InObjArray->GetEntriesFast(); i++ )
	{
		os=(TObjString*)InObjArray->At(i);
		s=os->GetString();
		OutStringVec.push_back(s);
	}
	return OutStringVec;
}

//----------------------------
//// Load online replay
////--------------------------


TChain* LoadOnline(Int_t run, const char* path, const char* tree,Int_t debug)
{
    TChain* tt = new TChain(tree);

    TString  basename = Form("tritium_online_%d",run);
    TString rootfile = basename + ".root";
    
    TString dir = path;
    if (!dir.EndsWith("/")) dir.Append("/");

    rootfile.Prepend(dir.Data());

    Long_t split = 0;

    while ( !gSystem->AccessPathName(rootfile.Data()) ) {
	tt->Add(rootfile.Data());
	cout << "ROOT file " << rootfile << " added to " << tree<<" tree"<<endl;
	split++;
	rootfile = basename + "_" + split + ".root";
	rootfile.Prepend(dir.Data());

    }

    if (split<=0) {
	if (debug>0) cerr << "Can not find online replay file for run " << run << endl;
	delete tt;
	tt = 0;
    }

    return tt;
}

TChain* LoadOnline(Int_t run, const char* tree = "T")
{
    Int_t i=0;

    TChain* tt = 0;
    while (PATHS[i]) {
	tt = LoadOnline(run,PATHS[i++],tree,1);

	if (tt) break;
    }
   
    if (!tt)
	cerr << "Can not find online replay file for run " << run << endl;

    return tt;
}


vector<Int_t> gGet_RunNoChain1(const TString& aString)
{
	vector<Int_t> output;
	vector<TString> input=Tokens1(aString,",");
	unsigned int i;
	for ( i=0; i<input.size(); i++ )
	{
		if ( isdigit(*input[i].Data()) || input[i].BeginsWith("-") )
		{
			vector<TString> tmp=Tokens1(input[i],'-');
			if ( tmp.size()<=2 )
			{
				// Int_t FirstRunNo;
				Int_t LastRunNo;
				Int_t thisrunno;
				size_t index=input[i].First('-');
				if ( index<input[i].Length() ){
					thisrunno=tmp[0].Atoi();
					LastRunNo=tmp[1].Atoi();
				}else{
					thisrunno=tmp[0].Atoi();
					LastRunNo=thisrunno;
				}
				if ( thisrunno>LastRunNo ){
					thisrunno ^= LastRunNo;
					LastRunNo ^= thisrunno;
					thisrunno ^= LastRunNo;
				}
				while ( thisrunno<=LastRunNo ){
                                    output.push_back(thisrunno);
                                    thisrunno++;
				}
			}
		}
	}
	for ( i=0; i<output.size(); i++ )
	{
		if ( i==0 && output.size()>1 ){
			cerr<<"      RunNoChain is "<<output[i];
		}
		else if ( i==0 && i==(output.size()-1) ){
			cerr<<"      RunNoChain is "<<output[i]<<endl;
		}
		else if ( i==(output.size()-1) ){
			cerr<<" "<<output[i]<<endl;
		}
		else{
			cerr<<" "<<output[i];
		}
	}
	return output;
}


// Load TTree trees from all the runs in a kin.dat file
TChain* LoadKin(TString filename, const char* tree = "T")
{
	filename = "/work/halla/triton/Runlist/" + filename;
    ifstream file(filename.Data());
    TChain *null;
    if(!file.good()){cout << filename.Data() << " does not exist! "<<endl;return(null);}
    TString content;
    TString Target,Kin,Run_String;
    const char* kin;
    for (int ii=0; content.ReadLine(file) && ii<3;ii++ ) {  
    	//cout<<"!!!:  "<<content<<endl;
        if(ii==0)Target = content;
        if(ii==1){
           kin = content.Data();
           Kin = Form("Kin%s", kin);
         }
        if(ii==2)Run_String = content;         
      }
      file.close();
    const vector<Int_t> RunNoChain=gGet_RunNoChain1(Run_String);
	TChain* tt=new TChain("tt","Main Tree");
	for ( unsigned int iii=0; iii<RunNoChain.size(); iii++ )
	{
			Int_t aRunNo=RunNoChain[iii];
			tt->Add(LoadRun(aRunNo,tree));
	}
			
	return tt;
}



