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

int mergeTrees(const char * inputDir="/volatile/clas12/users/gmat/clas12analysis.sidis.data/rga/nSidis/testsmall",
	       const char * rootname="run*binned",
	       const char * treeName="EVERYTHING"
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
  Merger *merger=new Merger(inputDir,outFileName);

  //---------------------------
  // Add directories for TChain
  //---------------------------
  for(unsigned int i = 0 ; i < dirpaths.size() ; i++){
    merger->addDirPath(dirpaths[i].c_str());
  }
  //---------------------------
  // Add TTree for TChain
  //---------------------------
  merger->addTree(treeName);
  //---------------------------
  // Merge the TTrees
  //---------------------------
  merger->mergeTrees();
  
  // Special return statement for slurm
  // If tree_MC, return 1
  // If tree_reco, return 2
  // If tree_postprocess, return 3
  // Else, return 0
  std::string str = treeName;
  if(str.compare("tree_MC")==0)
    return 1;
  else if(str.compare("tree_reco")==0)
    return 2;
  else if(str.compare("tree_postprocess")==0)
    return 3;

  return 0;
}
