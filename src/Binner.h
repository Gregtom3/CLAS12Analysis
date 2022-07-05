#ifndef Binner_h
#define Binner_h

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <ROOT/RDataFrame.hxx>
#include <TString.h>
#include <TDirectory.h>

using namespace std;

// Type template below store the necessary cuts to define the bins.
// The type is a map because the user may want to define more than one bin
// Example : 0.35 < z < 0.41 && 0 < Mh < 1.5
typedef std::vector<std::string> type_bin;

class Binner{
 public:
  
  Binner(const char *, const char *, int);
 
  // Creates a single 1d bin
  int create1DBinning(const char *, float, float, bool); 

  // Creates multiple 1D bins in one 'go'
  // Example: z = {0,0.1,0.2,0.3,0.4,0.5} would make 5, 1D bins
  int create1DBinnings(const char *, std::vector<float>, bool);

  // Creates a single 2d bin
  int create2DBinning(const char *, float, float,
		      const char *, float, float, bool);

  // Creates multiple 2D bins, where the first cut consists of an array of binnings,
  // and the second cut is just a single bin
  // Example: z = {0,0.1,0.2,0.3,0.4,0.5} && 0 < Mh < 1.5 would make 5, 2D bins
  int create2DBinnings(const char *, std::vector<float>,
		       const char *, float, float, bool);
  
  // For each binning, pass through the main TTree and store dihadron angles
  int processBins();
  
 protected:
  const char * _fName;
  const char * _tName;
  TFile *_fIn;
  ROOT::RDataFrame _dfIn = ROOT::RDataFrame(0);

  type_bin _bins;
  std::vector<std::string> _binCuts;  
  std::vector<std::string> _treeNames;
  std::vector<int> _helicity_flag;

  // Floats for dihadron angles, mass params, and helicity from output TTree
  float _phih;
  float _phiR;
  float _theta;
  float _Mdiphoton;
  float _Mdihadron;
  float _helicity;

  int _verbosity = 0;
};

#endif
