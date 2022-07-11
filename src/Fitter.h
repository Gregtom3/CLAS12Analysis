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
  // Asymmetry distribution
  fitObject asym;
  // Binned Asymmetry histogram
  histObject hist_asym;
  // Asymmetry fit parameters & errors
  std::vector<double> asym_params_u0;
  std::vector<double> asym_errors_u0;

  std::vector<double> asym_params_u1;
  std::vector<double> asym_errors_u1;

  std::vector<double> asym_params_u2;
  std::vector<double> asym_errors_u2;

  std::vector<double> asym_params_u3;
  std::vector<double> asym_errors_u3;
};

struct sPlotObject{
  const char * outDir;
  const char * variable;
  const char * idbranch;
  const char * signalFactoryPDF;
  const char * bgFactoryPDF;
};

class Fitter{
 public:
  // Constructor
  Fitter(std::string, std::string, std::string);
  
  void setSideband(sidebandObject);
  void setSplot(sPlotObject);
  //  int loadCOWs(COWsObject);
  
  int executeSideband(histObject);  
  int executeSplot();  
  int executeCOWs();  

  int extractSWeights(sPlotObject);

  int End();

 protected:
  
  int appendSidebandInfo();
  int perform1DBinnedFits(fitObject, histObject);
  int perform2DBinnedFits(fitObject, histObject);
  
  // Check the npars consistency of the fitObject

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
