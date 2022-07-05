#include "Binner.h"

Binner::Binner(const char * tName, const char * fName, int v=0)
{
  _fIn = new TFile(fName,"UPDATE");
  // Create basic ROOT::RDataFrame
  _dfIn = ROOT::RDataFrame(tName,fName);

  _verbosity = v;
  if(_verbosity > 0)
    cout << " \n Beginning Binner script \n" << endl;

}

int Binner::create1DBinning(const char * binVar , float varMin, float varMax){
  // Generate cut statement
  TString binCut = Form("%s > %f && %s < %f",binVar,varMin,binVar,varMax);
  // Append binning to list
  _binCuts.push_back(binCut.Data());
  // Generate unique output tree name
  TString treeName = Form("%s_%f_%f",binVar,varMin,varMax); 
  _treeNames.push_back(treeName.Data());
  return 0;
}

int Binner::create1DBinnings(const char * binVar, std::vector<float> vars){
  for(unsigned int idx = 0; idx < vars.size()-1;idx++){
    float varMin = vars.at(idx);
    float varMax = vars.at(idx+1);
    create1DBinning(binVar,varMin,varMax);
  }
  return 0;
}

int Binner::create2DBinning(const char * binxVar , float varxMin, float varxMax,
		    const char * binyVar , float varyMin, float varyMax){
  // Generate cut statements
  TString binCut = Form("%s > %f && %s < %f && %s > %f && %s < %f",binxVar,varxMin,binxVar,varxMax,binyVar,varyMin,binyVar,varyMax);
  // Append binning to list
  _binCuts.push_back(binCut.Data());
  // Generate unique output tree name
  TString treeName = Form("%s_%f_%f_%s_%f_%f",binxVar,varxMin,varxMax,
			                      binyVar,varyMin, varyMax); 
  _treeNames.push_back(treeName.Data());
  return 0;
}

int Binner::create2DBinnings(const char * binxVar , std::vector<float> varsx,
		    const char * binyVar , float varyMin, float varyMax){
 for(unsigned int idx = 0; idx < varsx.size()-1;idx++){
    float varxMin = varsx.at(idx);
    float varxMax = varsx.at(idx+1);
    create2DBinning(binxVar,varxMin,varxMax,
		    binyVar,varyMin,varyMax);
 }
 return 0;
}

int Binner::processBins()
{
  // Open the TFile
  _fIn->cd();
  // Get number of bins
  const int nBins = _treeNames.size();
  
  // For each binning...
  for(int i = 0 ; i < nBins ; i++){
    // Delete TTree if it is recognized
    if(_fIn->Get(_treeNames.at(i).c_str()))
      gDirectory->Delete(_treeNames.at(i).c_str());

    // Create TTree pointer for bin
    TTree * outTree = new TTree(_treeNames.at(i).c_str(),"");
    outTree->Branch("phih",&_phih);
    outTree->Branch("phiR",&_phiR);
    outTree->Branch("theta",&_theta);
 
    // Fill dihadron angles into binned trees
    if(_verbosity > 0)
      cout << " Filling Bin --> " << _binCuts.at(i) << endl;
   _dfIn.Filter(_binCuts.at(i)).Foreach([&](double phih, double phiR, double theta)
					 { _phih = phih;
					   _phiR = phiR;
					   _theta = theta;
					   outTree->Fill();},
					 {"phi_h","phi_R0","th"});
   
   // Write TTree
   outTree->Write();
  }
  _fIn->Close();
  return 0;
}


