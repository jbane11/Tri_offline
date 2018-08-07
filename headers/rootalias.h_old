// The paths to store ROOT files
// adapted from Ou,Longwu's GMp code
// -shujie 03,2018

struct target
{
  Double_t pos;
  TString name;
};


const char* PATHS[] = {
  "/chafs1/work1/tritium/Rootfiles/",
  "/chafs1/work1/tritium/Tritium_Rootfiles/",
  "/volatile/halla/triton/Tritium_Rootfiles/",
  //     "/volatile/halla/triton/Marathon_Rootfiles/pass1/",
  "./",
  0
};

//-------------------------------------------
//get target info from epics encoder position
// check if target moved during the run
// works for tritium snice 2018.1
//----------------------------------------- 
TString GetTarget(TChain *tree2){
  TString targname;
  target t2={33106235,"Tritium"};
  target d2={29367355,"Deuterium"};
  target h={25610043,"Hydrogen"};
  target he3={21875520,"Helium3"};
  target empty={18119995,"Empty Cell"};
  target dummy={15175217,"Dummy"};
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

  cout<<pos<<"  "<<targname<<endl;
  return targname;
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



// Chain all the ROOT files for a given run number and return the TChain
//      run   --- run number
//      path  --- Directory to look for the ROOT files
//      tree  --- Name of the tree to be loaded from the ROOT files. Can be "T", "E", "LEFT", ...
//      debug --- Debug level
//      rootpath: return rootpath
//
TChain* LoadRun(Int_t run, const char* tree = "T", string &rootpath)
{
    Int_t i=0;

    TChain* tt = 0;
    
    while (PATHS[i]) {
	tt = LoadRun(run,PATHS[i++],tree,0);
	if (tt) {
	  rootpath=PATHS[i-1];
	  break;
	}
    }

    if (!tt)
	cerr << "Can not find ROOT file for run " << run << endl;

    return tt;
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
	cout << "ROOT file " << rootfile << " added to " << tree<<" tree"<<endl;
	split++;
	rootfile = basename + "_" + split + ".root";
	rootfile.Prepend(dir.Data());
    }

    if (split<=0) {
	if (debug>0) cerr << "Can not find ROOT file for run " << run << endl;
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
	tt = LoadRun(run,PATHS[i++],tree,0);
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

    TDirectory* fDir = tt->GetDirectory();
    if (fDir) fDir->GetObject("Run_Data",run);
    if (run) {
      TArrayI ps = run->GetParameters()->GetPrescales();
      delete run;
    }

    return ps;
}

Int_t GetPS(TTree* tt,Int_t trigger)
{
  //  TDatime run_time("2018-01-01 00:00:00");

    THaRun* run = 0;

    if (!tt->GetDirectory()) tt->LoadTree(0); // Necessary if T is a TChain
    TDirectory* fDir = tt->GetDirectory();
    if (fDir) fDir->GetObject("Run_Data",run);
    if (run) {
      Int_t ps = run->GetParameters()->GetPrescales()[trigger-1];
      delete run;
    }
    return ps;
}
