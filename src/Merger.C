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

  // Open merged output file (not temporary)
  _fOut=new TFile(_outFileName,"UPDATE");

  // For loop over all TTrees
  for(std::string treeName : _treeNames){
    // Create TChain for linking TTree files
    TChain *chain = new TChain(treeName.c_str());

    // Loop over all directories which contain files we want to link
    for(unsigned int i = 0; i < _dirpaths.size(); i++){
      chain->Add(_dirpaths.at(i).c_str());
    }

    // Save tree to Temp Root File
    TString tempFile = Form("%s_%s.root",_outFileName,treeName.c_str());
    chain->Merge(tempFile.Data());

    // Open temp TFile with stored chained TTree
    TFile *ftemp = new TFile(tempFile,"UPDATE");
    ftemp->cd();

    // Open TTree
    _tOut = (TTree*)ftemp->Get(treeName.c_str());
  
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
    ftemp->Close();
    // Remove temp file
    gSystem->Exec(Form("rm %s",tempFile.Data()));
  }
  // Close file
  _fOut->Close();
  
  // Return
  return 0;
}
