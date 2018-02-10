/*class TRI_VAR{{{*/
class TRI_VAR
{
	public:
		TRI_VAR()
		{
			SetValue();
		};
		TRI_VAR(TRI_VAR const&){};
//		TRI_VAR& operator=(TRI_VAR const&){};
		TRI_VAR& operator*=(const Double_t& aNum)
		{
			this->Value *= aNum;
			this->Stat_Err *= aNum;
			this->Sys_Err *= aNum;
			return *this;
		};

		Double_t Value;//value of this variable
		Double_t Stat_Err;//statistical error of this variable
		Double_t Sys_Err;//systematic error of this variable

		void SetValue
			(
			 const Double_t& aValue=0,
			 const Double_t& aStat_Err=0,
			 const Double_t& aSys_Err=0
			)
			{
				Value=aValue;
				Stat_Err=aStat_Err;
				Sys_Err=aSys_Err;
			};

		void Print()
		{
			Printf("%-*s%e",20,"Value:",Value);
			Printf("%-*s%e",20,"Stat_Err:",Stat_Err);
			Printf("%-*s%e",20,"Sys_Err:",Sys_Err);
		}
};
/*}}}*/

/*inline vector<TString> Tokens(TString aline,TString aDelim=" "){{{*/
inline vector<TString> Tokens(TString aline,TString aDelim=",")
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
/*}}}*/

/*inline TString gGet_InputFile_Var(const TString& aFile_Name,const Int_t& aLine){{{*/
inline TString gGet_InputFile_Var(const TString& aFile_Name,const Int_t& aLine)
{
	ifstream file(aFile_Name.Data());
	TString content;
	Int_t i=0;
	TString outstring;
	while ( content.ReadLine(file) )
	{
		if ( i==aLine )
		{
			outstring=content;
			break;
		}
		i++;
		if ( !file.good() )
			break;
	}
	file.close();
	if ( outstring=="\"\"" )
		outstring="";
	return outstring;
}
/*}}}*/

/*inline vector<Int_t> gGet_RunNoChain(const TString& aString){{{*/
inline vector<Int_t> gGet_RunNoChain(const TString& aString)
{
	vector<Int_t> output;
	vector<TString> input=Tokens(aString,",");
	unsigned int i;
	for ( i=0; i<input.size(); i++ )
	{
		if ( isdigit(*input[i].Data()) || input[i].BeginsWith("-") )
		{
			vector<TString> tmp=Tokens(input[i],'-');
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
/*}}}*/

/*inline TChain* gAddTree(const Int_t aRunNo,const TString& aArm,TString aTreeName="T"){{{*/
inline TChain* gAddTree(const Int_t aRunNo,TString aTreeName="T")
{
	TChain* aTree=new TChain(aTreeName.Data(),"Main Tree");
	Int_t index=0;
	TString File_Form=Form("%s_%d.root",ROOTFILES_NAME.Data(),aRunNo);
	if ( gSystem->FindFile(ROOTFILES_DIR.Data(),File_Form) )
	{
		aTree->Add(Form("%s/%s_%d.root",ROOTFILES_DIR.Data(),ROOTFILES_NAME.Data(),aRunNo));
	}
	else{  
		cerr<<Form("       *ERROR*, no rootfile founded for Run#%d",aRunNo)<<endl;
		return 0;
	}
	index++;
	File_Form=Form("%s_%d_%d.root",ROOTFILES_NAME.Data(),aRunNo,index);
	while ( gSystem->FindFile(ROOTFILES_DIR.Data(),File_Form) )
	{
		aTree->Add(Form("%s/%s_%d_%d.root",ROOTFILES_DIR.Data(),ROOTFILES_NAME.Data(),aRunNo,index));
		index++;
		File_Form=Form("%s_%d_%d.root",ROOTFILES_NAME.Data(),aRunNo,index);
	}
	aTree->LoadTree(0);

	return aTree;
}
/*}}}*/

/*inline TChain* gGetTree(const vector<Int_t>& aRunNoChain,TString aTreeName="T"){{{*/
inline TChain* gGetTree(const vector<Int_t>& aRunNoChain, TString aTreeName="T")
{
	TChain* Tree=new TChain(aTreeName.Data(),"Main Tree");
	for ( unsigned int i=0; i<aRunNoChain.size(); i++ )
	{
		Int_t aRunNo=aRunNoChain[i];
		Int_t index=0;
		TString File_Form=Form("%s_%d.root",ROOTFILES_NAME.Data(),aRunNo);
		if ( gSystem->FindFile(ROOTFILES_DIR.Data(),File_Form) )
		{
			cerr<<Form("      %s->Add(%s/%s_%d.root)",aTreeName.Data(),ROOTFILES_DIR.Data(),ROOTFILES_NAME.Data(),aRunNo)<<endl;
			Tree->Add(Form("%s/%s_%d.root",ROOTFILES_DIR.Data(),ROOTFILES_NAME.Data(),aRunNo));
		}
		else
			return 0;
		index++;
		File_Form=Form("%s_%d_%d.root",ROOTFILES_NAME.Data(),aRunNo,index);
		while ( gSystem->FindFile(ROOTFILES_DIR.Data(),File_Form) )
		{
			cerr<<Form("         Add %s/%s_%d_%d.root",ROOTFILES_DIR.Data(),ROOTFILES_NAME.Data(),aRunNo,index)<<endl;
			Tree->Add(Form("%s/%s_%d_%d.root",ROOTFILES_DIR.Data(),ROOTFILES_NAME.Data(),aRunNo,index));
			index++;
			File_Form=Form("%s_%d_%d.root",ROOTFILES_NAME.Data(),aRunNo,index);
		}
		Tree->LoadTree(0);
	}
	return Tree;
}
/*}}}*/

/*inline int gGet_TargetInfo(TString aTarget_Name){{{*/
inline int gGet_TargetInfo(const TString &aTarget_Name, int* aA, int* aZ, double* aThickness, double* aThickness_Err)
{
    if(aTarget_Name=="He3"||aTarget_Name=="Helium"||aTarget_Name=="He"){
        aA[0] = 3;
        aZ[0] = 2;
        aThickness[0] = 53.4  * 1e-3;//g/cm^2
        aThickness_Err[0] = 0.6 * 1e-3 ;//g/cm^2
        return 0;
    }
    else if(aTarget_Name=="H3"||aTarget_Name=="Tritium"||aTarget_Name=="T2"){
        aA[0] = 3;
        aZ[0] = 1;
        aThickness[0] = 77 * 1e-3;//g/cm^2
        aThickness_Err[0] = 0.1  * 1e-3;//g/cm^2
        return 0;
    }
    else if(aTarget_Name=="D2"||aTarget_Name=="Deuterium"||aTarget_Name=="H2"){
        aA[0] = 2;
        aZ[0] = 1;
        aThickness[0] = 142.2 * 1e-3;//g/cm^2
        aThickness_Err[0] = 0.8 * 1e-3 ;//g/cm^2
        return 0;
    }
    else if(aTarget_Name=="H1"||aTarget_Name=="Hydrogen"){
        aA[0] = 1;
        aZ[0] = 1;
        aThickness[0] = 70.8  * 1e-3;//g/cm^2
        aThickness_Err[0] = 0.4 * 1e-3 ;//g/cm^2
        return 0;
    }else{
        cerr<<" *****, I don't know this target name = "<<aTarget_Name.Data()<<endl;
      return -1; 
    }

}

