#include "Asymmetry.h"

using namespace std;

Asymmetry::Asymmetry(const char * inputDir){
  _inputDir = inputDir;
  void *dir = gSystem->OpenDirectory(_inputDir);
  char * subdir;
  while((subdir = const_cast<char *>(gSystem->GetDirEntry(dir)))){
    // Save full path of subdirectory
    char * subdirpath = Form("%s/%s",inputDir,subdir);
    // Split subdirectory by keyword "_"
    strVect svect = splitString(subdir,"_");
    
    // Test if subdirectory contains keyword "bin"
    if(strVectorContains(svect, _keyword)==false)
      continue;
    
    // Create new asymmetry bin
    // 1. Set subdirpath (starting from root)
    // 2. Find method type from prefix
    // 3. Set bin specific variables (binVar, min, max)
    asymBin ab;
    ab.dirpath = std::string(subdirpath);
    if(strVectorContains(svect,"sideband")==true)
      {
	ab.method = sideband;
      }
    else if(strVectorContains(svect,"splot")==true)
      {
	ab.method = splot;
      }
    else if(strVectorContains(svect,"cows")==true)
      {
	ab.method = cows;
      }
    else
      continue;

    if(setBin(svect,ab)==-1)
      continue;
    else
      _asymBins.push_back(ab);
  }
  
  _fOut = new TFile(Form("%s/asymmetry.root",inputDir),"RECREATE");
}

int Asymmetry::process(){
  // Get unique binnings
  std::vector<strVect> unique_binnings = getUniqueBinnings();
  // Prepare parameter & error vectors from asymmetry fits
  std::vector<float> *sig_params=0;
  std::vector<float> *sig_errors=0;
  std::vector<float> *sigbg_params=0;
  std::vector<float> *sigbg_errors=0;
  std::vector<float> *bg_params=0;
  std::vector<float> *bg_errors=0;
  // For each unique binning, construct the appropriate TGraphErrors object
  // THIS IS GOING TO BE DIFFICULT TO GENERALIZE FOR ARBITRARY BINNING DIMENSIONS
  // BUT ANYWAY, I WILL PROCEED ASSUMING 1D ONLY

  // Loop over each unique binning
  for(strVect sv: unique_binnings){
    // Get subset of asymmetry bins with exact same binning
    std::vector<asymBin> ab_subset = getAsymBinSubset(sv);
    // Create vector of TGraphErrors object
    // Vector b/c one binning can have multiple asymmetry fit parameters
    std::vector<TGraphErrors *> tgeVect_sig_sideband;
    std::vector<TGraphErrors *> tgeVect_sigbg_sideband;
    std::vector<TGraphErrors *> tgeVect_bg_sideband;
    std::vector<TGraphErrors *> tgeVect_sig_splot;
    std::vector<TGraphErrors *> tgeVect_sigbg_splot;
    std::vector<TGraphErrors *> tgeVect_bg_splot;
    std::vector<TGraphErrors *> tgeVect_sig_cows;
    std::vector<TGraphErrors *> tgeVect_sigbg_cows;
    std::vector<TGraphErrors *> tgeVect_bg_cows;
    // Loop over each bin with identical bin scheme
    bool first = true;
    int idx_sideband = 0;
    int idx_splot = 0;
    //    int idx_cows = 0;
    for(asymBin ab : ab_subset){
      std::string rootname;
      std::string treeName;
      if(ab.method == sideband){
	rootname = "sidebandMethod.root";
	treeName = "tree";
      }
      else if(ab.method == splot){
	rootname = "DataWeightedTree.root";
	treeName = "tree";
      }
      else if(ab.method == cows){
	rootname = "cowsMethod.root";
	treeName = "tree";
      }
      else{
	cout << "ERROR in Asymmetry::process --- Unknown asymBin method. Return -1" << endl;
	return -1;
      }
      
      // Load in subdirectory containing binned fit data
      TFile *fIn = new TFile(Form("%s/%s",ab.dirpath.c_str(),rootname.c_str()),"READ");
      TTree *tIn = (TTree*)fIn->Get(treeName.c_str());
      
      // Fill TGraphErrors depending on method

      // SIDEBAND METHOD
      // ----------------------------------------------------
      if(ab.method == sideband){
	// Default to u3 for sideband for now
	tIn->SetBranchAddress("A_sig_u3",&sig_params);
	tIn->SetBranchAddress("A_sig_errors_u3",&sig_errors);
	tIn->SetBranchAddress("A_sigbg_u3",&sigbg_params);
	tIn->SetBranchAddress("A_sigbg_errors_u3",&sigbg_errors);
	tIn->SetBranchAddress("A_bg_u3",&bg_params);
	tIn->SetBranchAddress("A_bg_errors_u3",&bg_errors);
	tIn->GetEntry(0);
	for(unsigned int k = 0 ; k < sig_params->size() ; k++){
	  if(first){
	    first = false;
	    for(unsigned int l = 0 ; l < sig_params->size() ; l++){
	      tgeVect_sig_sideband.push_back(new TGraphErrors((int)ab_subset.size()));	 
	      tgeVect_sigbg_sideband.push_back(new TGraphErrors((int)ab_subset.size()));	 
	      tgeVect_bg_sideband.push_back(new TGraphErrors((int)ab_subset.size()));	 
	      tgeVect_sig_sideband.at(l)->SetName(Form("sideband_%s_sig_%d",ab.graphName.c_str(),(int)l));
	      tgeVect_sigbg_sideband.at(l)->SetName(Form("sideband_%s_sigbg_%d",ab.graphName.c_str(),(int)l));
	      tgeVect_bg_sideband.at(l)->SetName(Form("sideband_%s_bg_%d",ab.graphName.c_str(),(int)l));
	    }
	  }
	  // Bin center code will clearly change if we upgrade to 2D binnings
	  float bincenter = (ab.binMin.at(0)+ab.binMax.at(0))/2;
	  tgeVect_sig_sideband.at(k)->SetPoint(idx_sideband,bincenter,sig_params->at(k));
	  tgeVect_sig_sideband.at(k)->SetPointError(idx_sideband,0,sig_errors->at(k));
	  tgeVect_sigbg_sideband.at(k)->SetPoint(idx_sideband,bincenter,sigbg_params->at(k));
	  tgeVect_sigbg_sideband.at(k)->SetPointError(idx_sideband,0,sigbg_errors->at(k));
	  tgeVect_bg_sideband.at(k)->SetPoint(idx_sideband,bincenter,bg_params->at(k));
	  tgeVect_bg_sideband.at(k)->SetPointError(idx_sideband,0,bg_errors->at(k));
	} 
	idx_sideband++;
      }//if(ab.method == sideband)
      
      // SPLOT METHOD
      // ----------------------------------------------------
      if(ab.method == splot){
	// Default to u3 for sideband for now
	tIn->SetBranchAddress("A_sig",&sig_params);
	tIn->SetBranchAddress("A_sig_errors",&sig_errors);
	tIn->SetBranchAddress("A_sigbg",&sigbg_params);
	tIn->SetBranchAddress("A_sigbg_errors",&sigbg_errors);
	tIn->SetBranchAddress("A_bg",&bg_params);
	tIn->SetBranchAddress("A_bg_errors",&bg_errors);
	tIn->GetEntry(0);
	for(unsigned int k = 0 ; k < sig_params->size() ; k++){
	  if(first){
	    first = false;
	    for(unsigned int l = 0 ; l < sig_params->size() ; l++){
	      tgeVect_sig_splot.push_back(new TGraphErrors((int)ab_subset.size()));	 
	      tgeVect_sigbg_splot.push_back(new TGraphErrors((int)ab_subset.size()));	 
	      tgeVect_bg_splot.push_back(new TGraphErrors((int)ab_subset.size()));	 
	      tgeVect_sig_splot.at(l)->SetName(Form("splot_%s_sig_%d",ab.graphName.c_str(),(int)l));
	      tgeVect_sigbg_splot.at(l)->SetName(Form("splot_%s_sigbg_%d",ab.graphName.c_str(),(int)l));
	      tgeVect_bg_splot.at(l)->SetName(Form("splot_%s_bg_%d",ab.graphName.c_str(),(int)l));
	    }
	  }
	  // Bin center code will clearly change if we upgrade to 2D binnings
	  float bincenter = (ab.binMin.at(0)+ab.binMax.at(0))/2;
	  tgeVect_sig_splot.at(k)->SetPoint(idx_splot,bincenter,sig_params->at(k));
	  tgeVect_sig_splot.at(k)->SetPointError(idx_splot,0,sig_errors->at(k));
	  tgeVect_sigbg_splot.at(k)->SetPoint(idx_splot,bincenter,sigbg_params->at(k));
	  tgeVect_sigbg_splot.at(k)->SetPointError(idx_splot,0,sigbg_errors->at(k));
	  tgeVect_bg_splot.at(k)->SetPoint(idx_splot,bincenter,bg_params->at(k));
	  tgeVect_bg_splot.at(k)->SetPointError(idx_splot,0,bg_errors->at(k));
	} 
	idx_splot++;
      }//if(ab.method == splot)
    }//for(asymBin ab : ab_subset)


    // Finished creating all the TGraphErrors for the unique binning
    // Write TGraphErrors
    _fOut->cd();
    for(unsigned int l = 0 ; l < sig_params->size() ; l++){
      tgeVect_sig_sideband.at(l)->Write();
      tgeVect_sigbg_sideband.at(l)->Write();
      tgeVect_bg_sideband.at(l)->Write();

      tgeVect_sig_splot.at(l)->Write();
      tgeVect_sigbg_splot.at(l)->Write();
      tgeVect_bg_splot.at(l)->Write();
    }
  }//for(strVect sv: unique_binnings)

  // Finished processing all unique binnings
  // Close TFile
  _fOut->Close();

  return 0;
}
void Asymmetry::print(){
  // Print all the unique binnings
  for(strVect sv : getUniqueBinnings()){
    cout << sv.size() << "-dimensional binning in ";
    for(unsigned int j = 0 ; j < sv.size() ; j++){
      cout << sv.at(j);
      if(j+1<sv.size())
	cout << " and ";
      else
	cout << ".\n";   
    }
  }
}
// Splits string/char based on delimeter
// Example:
// splitString("how_are_you","_") would return
// std::vector<char *> = {"how","are","you"}
strVect Asymmetry::splitString(std::string string, const char * delimiter){
  strVect splits;
  std::istringstream iss(string);
  std::string token;
  while (std::getline(iss, token, (Char_t)delimiter[0]))
    {
      splits.push_back(token.c_str());
    }
  return splits;
}

// Checks if char * vector contains keyword
bool Asymmetry::strVectorContains(strVect svect, const char* keyword){
  for(std::string s: svect){
    int x = std::string(s).compare(std::string(keyword));
    if(x==0)
      return true; // Match!
  }
  return false; // No matches found
}

// Returns std::string exactly "skip" places after the keyword
// Example
// strVectorAfter({"alpha","beta","gamma","omega"} , "beta" , 2)
// returns "omega"
std::string Asymmetry::strVectorFind(strVect svect, const char* keyword, int skip){
  int i = 0;
  for(std::string s : svect){
    int x = std::string(s).compare(std::string(keyword));
    if(x==0){
      if(i+skip>(int)svect.size()-1 || i+skip < 0){
	cout << "ERROR in Asymmetry::strVectorFind --- Out of bounds exception with respect to skip variable. The keyword index PLUS skip variable must be within [0," << svect.size()-1 << "], however index+skip = " << i << "+" << skip << " = " << i+skip << ". Returning empty std::string" << endl;
	return std::string("");
      }
      return std::string(svect.at(i+skip));
    }
    else
      i++;
  }
  cout << "WARNING in Asymmetry::strVectorFind --- No keyword found. Returning empty string" << endl;
  return std::string("");
}

// Returns index of keyword in strVect
int Asymmetry::strVectorIndex(strVect svect, const char* keyword){
  int i = 0;
  for(std::string s : svect){
    int x = std::string(s).compare(std::string(keyword));
    if(x==0)
      return i;
    else
      i++;
  }
  cout << "WARNING in Asymmetry::strVectorIndex --- Cannot find keyword. Return -1" << endl;
  return -1;
}

// Set the bin related variables based on svect info
int Asymmetry::setBin(strVect svect, asymBin & ab){
  // Get index of keyword in char array
  int idx = strVectorIndex(svect,_keyword);
  // Get size of char array
  int L = (int)svect.size();
  // Extract number of binnings
  if((L-idx-1)%3!=0){
    cout << "ERROR in Asymmetry::setBin --- After the key word, there must be a 'N' distinct terms separate by '_' , where 'N' is divisible by 3. This is not the case, as 'N="<<L-idx-1<<"'. The required format from the Binning code should be 'method_bin_binVar1_min1_max1_binVar2_min2_max2... Returning -1" << endl;
    return -1;
  }
  int nBins = (L-idx-1)/3;
  // Set bin names, bin mins, bin maxs
  for(int b = 0 ; b < nBins; b++){
    ab.binVar.push_back(strVectorFind(svect,_keyword,b*3+1));
    ab.binMin.push_back(std::stof(strVectorFind(svect,_keyword,b*3+2)));
    ab.binMax.push_back(std::stof(strVectorFind(svect,_keyword,b*3+3)));
  }
  
  // Set graph name based on binning
  std::string grName = "";
  for(std::string s : ab.binVar){
    grName+=(s+"_");
  }
  ab.graphName = grName.c_str();
  
  return 0;
}

// Compare two strVectors
// Returns true if identical
// Returns false if not identical
bool Asymmetry::strVectorCompare(strVect sv1, strVect sv2){
  if(sv1.size()!=sv2.size())
    return false;
  
  for(unsigned int i = 0 ; i < sv1.size() ; i++){
    std::string s1 = sv1.at(i);
    std::string s2 = sv2.at(i);
    if(s1.compare(s2)!=0)
      return false;
  }
  
  return true;
}

std::vector<strVect> Asymmetry::getUniqueBinnings(){
  std::vector<strVect> unique_binnings;
  cout << " Consolidating the following binnings " << endl;
  for(asymBin ab: _asymBins){
    if(unique_binnings.size()==0)
      unique_binnings.push_back(ab.binVar);
    else{
      bool seen_before = false;
      for(strVect sv : unique_binnings){
	if(strVectorCompare(sv,ab.binVar)==true)
	  seen_before = true;
      }
      if(!seen_before)
	unique_binnings.push_back(ab.binVar);	
    }
  }
  return unique_binnings;
}

// Returns subset of AsymBins defined by their unique binning (binVar) 
// This code essentially returns a vector of asymmetry binnings
// of a specifically defined bin type (say, for example, x & z 2D binning)
std::vector<asymBin> Asymmetry::getAsymBinSubset(strVect sv){
  std::vector<asymBin> asymBinSubset;
  for(asymBin ab : _asymBins){
    if(strVectorCompare(ab.binVar,sv)==true)
      asymBinSubset.push_back(ab);
  }
  return asymBinSubset;
}

