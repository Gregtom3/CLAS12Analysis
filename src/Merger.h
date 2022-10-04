#ifndef Merger_h
#define Merger_h
/* STL Includes */
#include <utility>
#include <filesystem>
/* ROOT Includes */
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <TBranch.h>

using namespace std;

class Merger{
 public:
  Merger(const char*,const char*);
  
  void addDirPath(const char*);

  void addTree(std::string);
  int mergeTrees();

  TFile *_fOut;
  TTree *_tOut;
  std::vector<std::string> _dirpaths;
  const char* _inputDir;
  TString _outFileName;
  std::vector<std::string> _treeNames;
  const char* _outTreeName;
  
};
#endif
