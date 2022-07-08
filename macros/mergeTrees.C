//-------------------------------------------------
//
// mergeTrees.C
//
//-------------------------------------------------
#include "../src/Merger.h"
R__LOAD_LIBRARY(libclas12ana.so)

using namespace std;

int mergeTrees(const char * inputDir,
	       const char * wildcard
){
  //---------------
  // Load libraries
  //---------------
  gSystem->Load("libclas12ana.so");

  //---------------
  // User params
  //---------------
  std::vector<std::string> dirpaths;
  dirpaths.push_back(Form("%s",inputDir));
  const char* treeName = "tree_postprocess";
  const char* outFileName = Form("%s/merged_%s.root",inputDir,inputFile);
  
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
