#ifndef Asymmetry_h
#define Asymmetry_h

/* STL includes */
#include <iostream>
#include <string>
#include <sstream>
/* ROOT includes */
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TGraphErrors.h>
#include <TSystem.h>

using namespace std;

typedef std::vector<std::string> strVect;

// Enumeration for fitting procedures
enum Method{
  sideband = 0,
  splot = 1,
  cows = 2
};

// Structure for each unique binning
// std::vector's account for multidimensional binnings
struct asymBin{
  const char * dirpath;
  Method method;
  strVect binVar;
  std::vector<float> binMin;
  std::vector<float> binMax;
  const char * graphName;
};

class Asymmetry{
 public:
  Asymmetry(const char *);
  int process();
  void print();
 protected:


  bool strVectorContains(strVect, const char *);
  std::string strVectorFind(strVect, const char *, int);
  int strVectorIndex(strVect, const char *);
  int setBin(strVect, asymBin&);
  bool strVectorCompare(strVect, strVect);

  strVect splitString(std::string, const char *);
  std::vector<strVect> getUniqueBinnings();
  std::vector<asymBin> getAsymBinSubset(strVect);

  const char * _inputDir;
  TFile *_fOut;
  strVect _subDirs;
  std::vector<asymBin> _asymBins;
  const char *_keyword = "bin";


};
#endif
