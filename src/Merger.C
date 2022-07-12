#include "Merger.h"

Merger::Merger(const char* outFileName){
  _outFileName = outFileName;
}

void Merger::addDirPath(const char* dirpath){
  _dirpaths.push_back(std::string(dirpath));
}

void Merger::addTree(std::string treeName){
  _treeNames.push_back(treeName);
}

int Merger::mergeTrees(){

  // Open TFile 
  _fOut = new TFile(_outFileName,"UPDATE");

  // For loop over all TTrees
  for(std::string treeName : _treeNames){
    // Create TChain for linking TTree files
    TChain *chain = new TChain(treeName.c_str());

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
    _tOut = (TTree*)_fOut->Get(treeName.c_str());
  
    // Create new branch containing id for each entry
    int row = 0;
    TBranch *fidmerge = _tOut->Branch("fidmerge",&row,"fidmerge/I");
    for(auto i = 0; i < _tOut->GetEntries(); i++){
      _tOut->GetEntry(i);
      row = i;
      fidmerge->Fill();
    }
    _tOut->Write();
  }
  // Close file
  _fOut->Close();
  
  // Return
  return 0;
}
