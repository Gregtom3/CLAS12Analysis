#ifndef Asymmetry_h
#define Asymmetry_h

/* STL includes */
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

typedef std::vector<std::string> strVect;

// Enumeration for fitting procedures
enum Method{
  sideband = 0,
  splot = 1,
  cows = 2;
};

// Structure for each unique binning
// std::vector's account for multidimensional binnings
struct asymBin{
  const char * dirpath;
  Method method;
  strVect binVar;
  std::vector<float> binMin;
  std::vector<float> binMax;
};

class Asymmetry{
 public:
  Asymmetry(const char *);
  void print();
 protected:
  strVect splitString(std::string, const char *);
  bool strVectorContains(strVect, const char *);
  std::string strVectorFind(strVect, const char *, int);
  int strVectorIndex(strVect, const char *);
  bool setBin(strVect, asymBin&);
  bool strVectorCompare(strVect, strVect);
  std::vector<strVect> getUniqueBinnings();
  std::vector<asymBin> getAsymBinSubset(strVect);
  

  const char * _inputDir;
  TFile *_fOut;
  strVect _subDirs;
  std::vector<asymBin> _asymBins;
  const char *_keyword = "bin";


};
#endif
