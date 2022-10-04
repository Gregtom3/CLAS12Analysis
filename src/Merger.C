#include "Merger.h"
#include <iostream>

using namespace std;

Merger::Merger(const char* inputDir, const char* outFileName){
  _inputDir = inputDir;
  std::string temp = outFileName;
  std::replace(temp.begin(),temp.end(),'*','_');
  _outFileName=temp;
}

void Merger::addDirPath(const char* dirpath){
  _dirpaths.push_back(std::string(dirpath));
}

void Merger::addTree(std::string treeName){
  // If the user wanted every TTree, give it to them!
  if(treeName=="EVERYTHING"){
    TChain *chain = new TChain(treeName.c_str());
    for(unsigned int i = 0; i < _dirpaths.size(); i++){
      chain->Add(_dirpaths.at(i).c_str());
    }
    // Assuming all the files in the chain will have the same TTree, get TTrees in first file
    TString firstFile = chain->GetListOfFiles()->At(0)->GetTitle();
    TFile *ftemp = new TFile(firstFile,"READ");
    for(int i = 0 ; i < ftemp->GetListOfKeys()->GetEntries(); i++){
      _treeNames.push_back(ftemp->GetListOfKeys()->At(i)->GetName());
    }
    delete chain;
  }
  else
    _treeNames.push_back(treeName);
}

int Merger::mergeTrees(){
  // For loop over all TTrees
  int i = 1;
  for(std::string treeName : _treeNames){
    // Create TChain for linking TTree files
    TChain *chain = new TChain(treeName.c_str());
    // Open merged output file
    _fOut=new TFile(_outFileName,"UPDATE");
    // Loop over all directories which contain files we want to link
    for(unsigned int i = 0; i < _dirpaths.size(); i++){
      chain->Add(_dirpaths.at(i).c_str());
    }
    cout << "Merging TTree (" << i++ << "/" << _treeNames.size() << ") "  << treeName << " with " << chain->GetEntries() << " entries" << endl;
    // cd into merged output file
    _fOut->cd();
    // Before we even consider Merging, check if the first .root file contains the TTree
    TString firstFile = chain->GetListOfFiles()->At(0)->GetTitle();
    TFile *ftemp = new TFile(firstFile,"READ");
    if(!ftemp->Get(treeName.c_str()))
      {
	cout << "WARNING: TTree " << treeName.c_str() << " not found in file " << firstFile.Data() << "...Skipping..." << endl;
	continue;
      }
    ftemp->Close();
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
