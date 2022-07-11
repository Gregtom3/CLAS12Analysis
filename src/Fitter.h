#ifndef Fitter_h
#define Fitter_h
/* STL includes */
#include <iostream>
#include <math.h>
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
#include <TROOT.h>
/* BRUFIT includes */
#include <sPlot.h>
#include <Process.h>

using namespace std;

struct fitObject{
  std::string fitOptions;
  TF1 * binnedFit1D;
  TF2 * binnedFit2D;
};

struct histObject{
  std::string paramX;
  std::string paramY;
  std::string drawCut;
  TH1F * h1;
  TH2F * h2;
};

struct sidebandObject{
  // Signal + Sideband distributions
  fitObject signal;
  fitObject sideband;
  // Histogram across Signal + Sideband region
  histObject hist;
  // Asymmetry distribution
  fitObject asym;
  // Binned Asymmetry histogram
  histObject hist_asym;
};

struct sPlotObject{
  // BRUFIT variables
  const char * variable;
  const char * idbranch;
  const char * signalFactoryPDF;
  const char * bgFactoryPDF;
  // Histogram for 1D fitting region
  histObject hist;
  // Asymmetry distribution
  fitObject asym;
  // Binned Asymmetry histogram
  histObject hist_asym;
};

class Fitter{
 public:
  // Constructor
  Fitter(std::string, std::string, std::string);
  
  void setSideband(sidebandObject);
  void setSplot(sPlotObject);
  //  int loadCOWs(COWsObject);
  
  int executeSideband();  
  int executeSplot();  
  int executeCOWs();  

  int extractSWeights();

  int End();

 protected:
  
  int perform1DBinnedFits(fitObject, histObject);
  int perform2DBinnedFits(fitObject, histObject);

  
  

  std::string _filepath;
  std::string _rootname;
  std::string _treeName;

  TFile *_fIn;
  TTree *_tree;
  
  sidebandObject _sideband;
  sPlotObject _splot;
  histObject _asymHist;
};
#endif
