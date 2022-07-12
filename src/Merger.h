#ifndef Merger_h
#define Merger_h
/* STL Includes */
#include <utility>
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
  Merger(const char*);
  
  void addDirPath(const char*);

  void addTree(std::string);
  int mergeTrees();
 protected:
  TFile *_fOut;
  TTree *_tOut;
  std::vector<std::string> _dirpaths;
  const char* _outFileName;
  std::vector<std::string> _treeNames;
  const char* _outTreeName;
  
};
#endif
