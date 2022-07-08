#ifndef Fitter_h
#define Fitter_h
/* STL includes */
#include <iostream>
/* ROOT includes */
#include <TFile.h>
#include <TTree.h>
#include <TCollection.h>
#include <TObject.h>
#include <TKey.h>
#include <TString.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TF2.h>
#include <TFitResult.h>
/* BRUFIT includes */
#include <sPlot.h>
#include <Process.h>

using namespace std;

struct sPlotObject{
  const char * outDir;
  const char * variable;
  const char * idbranch;
  const char * signalFactoryPDF;
  const char * bgFactoryPDF;
};


struct fitObject{
  std::string fitFunction;
  int npars;
  std::vector<float> initialPars;
  std::vector<std::pair<float,float>> initialParRanges;
  std::string fitOptions;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
};

struct sidebandObject{
  fitObject signal;
  fitObject sideband;
  // Purity Variables
  // See analysis note for details
  std::pair<double,double> u0; // value, error
  std::pair<double,double> u1; // value, error
  std::pair<double,double> u2; // value, error
  std::pair<double,double> u3; // value, error
};

struct histObject{
  std::string name;
  std::string title;
  std::string paramX;
  std::string paramY;
  float xmin;
  float xmax;
  int xbins;
  float ymin;
  float ymax;
  int ybins;
};

class Fitter{
 public:
  // Constructor
  Fitter(std::string);

  int setWildcard(std::string wildcard);
  
  void setSideband(sidebandObject);
  void setSplot(sPlotObject);
  //  int loadCOWs(COWsObject);
  
  int executeSideband(histObject);  
  int executeSplot();  
  int executeCOWs();  

  int extractSWeights(sPlotObject);
  // Print keyNames in TFile
  void printKeys();

  int End();

 protected:
  

  int perform1DBinnedFits(fitObject, histObject);
  
  // Get keyNames within TFile given wildcard string (no * needed)
  std::vector<std::string> getKeyNames(std::string);

  // Check the npars consistency of the fitObject
  int checkFitObj(fitObject);



  std::string _filepath;
  TFile *_fIn;
  std::string _wildcard = "";

  sidebandObject _sideband;
  sPlotObject _splot;
};
#endif
