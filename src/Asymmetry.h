#ifndef Asymmetry_h
#define Asymmetry_h

/* STL includes */
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

typedef std::vector<char *> charVect;

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
  charVect binVar;
  std::vector<float> binMin;
  std::vector<float> binMax;
};

class Asymmetry{
 public:
  Asymmetry(const char *);

 protected:
  charVect splitString(char *, const char *);
  bool charVectorContains(charVect, const char *);
  std::string charVectorFind(charVect, const char *, int);
  int charVectorIndex(charVect, const char *);
  bool setBin(charVect, asymBin&);

  const char * _inputDir;
  charVect _subDirs;
  std::vector<asymBin> _asymBins;
  const char *_keyword = "bin";
};
#endif
