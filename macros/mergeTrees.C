//-------------------------------------------------
//
// mergeTrees.C
//
//-------------------------------------------------
#include "../src/Merger.h"
R__LOAD_LIBRARY(libclas12ana.so)

using namespace std;

int mergeTrees(){
  //---------------
  // Load libraries
  //---------------
  gSystem->Load("libclas12ana.so");

  //---------------
  // User params
  //---------------
  std::vector<std::string> dirpaths;
  dirpaths.push_back("/work/clas12/users/gmat/CLAS12Analysis/data/fall2018-torus-1-v1-nSidis/june5_*.root");
  dirpaths.push_back("/work/clas12/users/gmat/CLAS12Analysis/data/fall2018-torus+1-v1-nSidis/june5_*.root");
  dirpaths.push_back("/work/clas12/users/gmat/CLAS12Analysis/data/spring2019-torus-1-v1-nSidis/june5_*.root");
  const char* treeName = "tree_postprocess";
  const char* outFileName = "/work/clas12/users/gmat/CLAS12Analysis/data/rg-a/june8.root";
  
  //-------------------------
  // Create mergeTrees Object
  //-------------------------
  Merger *merger=new Merger(outFileName,treeName,treeName);
  
  //---------------------------
  // Add directories for TChain
  //---------------------------
  for(unsigned int i = 0 ; i < dirpaths.size() ; i++){
    merger->addDirPath(dirpaths[i].c_str());
  }

  //---------------------------
  // Merge the TTrees
  //---------------------------
  merger->mergeTrees();

  return 0;
}
