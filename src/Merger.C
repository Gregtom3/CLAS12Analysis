#include "Merger.h"
#include <iostream>

using namespace std;

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
  // For loop over all TTrees
  for(std::string treeName : _treeNames){
    // Create TChain for linking TTree files
    TChain *chain = new TChain(treeName.c_str());
    // Open merged output file
    _fOut=new TFile(_outFileName,"UPDATE");
    // Loop over all directories which contain files we want to link
    for(unsigned int i = 0; i < _dirpaths.size(); i++){
      chain->Add(_dirpaths.at(i).c_str());
    }
    // cd into merged output file
    _fOut->cd();
    // Merge TTree and store in output file
    chain->Merge(_fOut,0);
    // chain->Merge closes _fOut, so we must reopen it
    _fOut=new TFile(_outFileName,"UPDATE");
    // Grab the merged TTree
    _tOut =(TTree*)_fOut->Get(treeName.c_str()); 
    // Create new branch containing id for each entry
    int row = 0;
    TBranch *fidmerge = _tOut->Branch("fidmerge",&row,"fidmerge/I");
    for(auto i = 0; i < _tOut->GetEntries(); i++){
      _tOut->GetEntry(i);
      row = i;
      fidmerge->Fill();
    }
    // Store TTree in outfile
    _fOut->cd();
    _tOut->Write();
    // Delete original TTree
    _fOut->Delete(Form("%s;1",treeName.c_str()));
    // Close file
    _fOut->Close();
  }  
  // Return
  return 0;
}
