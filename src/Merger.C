#include "Merger.h"

Merger::Merger(const char* outFileName,
	       const char* inTreeName,
	       const char* outTreeName){
  _outFileName = outFileName;
  _inTreeName = inTreeName;
  _outTreeName = outTreeName;
}

void Merger::addDirPath(const char* dirpath){
  _dirpaths.push_back(std::string(dirpath));
}

int Merger::mergeTrees(){

  // Open TFile 
  _fOut = new TFile(_outFileName,"RECREATE");

  // Create TChain for linking TTree files
  TChain *chain = new TChain(_inTreeName);

  // Loop over all directories which contain files we want to link
  for(unsigned int i = 0; i < _dirpaths.size(); i++){
    chain->Add(_dirpaths.at(i).c_str());
  }

  // Save tree to Root File
  chain->Merge(_outFileName);

  // Close file
  _fOut->Close();

  // Reopen file in update mode
  _fOut=new TFile(_outFileName,"UPDATE");

  // Open TTree
  _tOut = (TTree*)_fOut->Get(_inTreeName);
  
  // Create new branch containing id for each entry
  int row = 0;
  TBranch *fidmerge = _tOut->Branch("fidmerge",&row,"fidmerge/I");
  for(auto i = 0; i < _tOut->GetEntries(); i++){
    _tOut->GetEntry(i);
    row = i;
    fidmerge->Fill();
  }
  _tOut->Write();

  // Close file
  _fOut->Close();
  
  // Return
  return 0;
}
