//-------------------------------------------------
//
// mergeTrees.C
//
// mergeTrees(flag, inputDir, rootname, treeName)
/*

  (flag)
  -------------------------------------------------------------------------
  The flag variable (0 or 1) indicates what mode mergeTrees in run in
    If flag == 0 , mergeTrees assumes there is only one directory, in which
  multiple  .root files are held.
  If flag == 1 , mergeTrees assumes there are multiple directories, each
  containing are single .root file

  (inputDir)
  -------------------------------------------------------------------------
  Absolute path to where the root files are held (should contain wildcard if
  flag == 1)

  (rootname)
  -------------------------------------------------------------------------
  Prefix of the .root file (should contain wildcard if flag == 0)

  (treeName)
  -------------------------------------------------------------------------
  Root tree name 

*/
// - inputDir 
//-------------------------------------------------
#include "../src/Merger.h"
R__LOAD_LIBRARY(libclas12ana.so)

using namespace std;

int mergeTrees(const char * inputDir,
	       const char * rootname,
	       const char * treeName
){
  //---------------
  // Load libraries
  //---------------
  gSystem->Load("libclas12ana.so");

  //---------------
  // User params
  //---------------
  std::vector<std::string> dirpaths;
  dirpaths.push_back(Form("%s/%s*.root",inputDir,rootname));
  const char* outFileName = Form("%s/merged_%s.root",inputDir,rootname);
  
  //-------------------------
  // Create mergeTrees Object
  //-------------------------
  Merger *merger=new Merger(outFileName);
  merger->addTree(treeName);

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
