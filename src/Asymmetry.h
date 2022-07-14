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
<<<<<<< HEAD
  std::string dirpath;
=======
  const char * dirpath;
>>>>>>> e46f75b74d42eaa76b0775520b0eb5e35bd7742f
  Method method;
  strVect binVar;
  std::vector<float> binMin;
  std::vector<float> binMax;
<<<<<<< HEAD
  std::string graphName;
=======
  const char * graphName;
>>>>>>> e46f75b74d42eaa76b0775520b0eb5e35bd7742f
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
