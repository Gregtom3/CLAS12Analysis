#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
// If anybody can figure out how to have root automically search in the include
// directory for this file, it would be GREATLY appreciated! 
R__LOAD_LIBRARY(libbrufit.so)
R__LOAD_LIBRARY(libclas12ana.so)
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/Fitter.h"
#endif

using namespace std;

int pipi0_fitter_test(
		 const char * inputDir = "/volatile/clas12/users/gmat/clas12analysis.sidis.data/fall2018-torus-1-small-batch/july8",
		 const char * rootName = "merged_july8",
		 const char * treeName = "tree_postprocess"
)
{
  //---------------
  // Load libraries
  //---------------
  gSystem->Load("libclas12ana.so"); 
  
  //-----------------------
  // Create Fitter Object
  //-----------------------
  Fitter *fitter = new Fitter(inputDir,rootName,treeName);

  //-----------------------
  // Create Sideband Object
  //-----------------------
  // First, create objects for fitting Mdiphoton regions
  fitObject signal;
  signal.fitOptions = "SR Q0";
  signal.binnedFit1D = new TF1("f_diphoton_sig","gaus(0)",0.108,0.160);
  signal.binnedFit1D->SetParameters(100,0.13,0.1);
  fitObject sideband;
  sideband.fitOptions = "SR Q0";
  sideband.binnedFit1D = new TF1("f_diphoton_bg","pol2",0.17,0.4);
  sideband.binnedFit1D->SetParameters(10,10,10);
  // Second, create 2D histogram for Mdiphoton binning
  histObject hist_Mgg;
  hist_Mgg.paramX = "Mdiphoton";
  hist_Mgg.h1 = new TH1F("h1","",100,0,0.5);
  // Third, create asymmetry fit object
  fitObject asym;
  asym.fitOptions = "SR Q0";
  asym.binnedFit2D = new TF2("f_2D","[0]*sin(2*x-2*y)+[1]*sin(x-y)+[2]*sin(-x+2*y)+[3]*sin(y)+[4]*sin(x)+[5]*sin(2*x-y)+[6]*sin(3*x-2*y)",-3.141592,3.141592,-3.141592,3.141592);
  // Fourth, create 2D histogram for phih phiR binning
  histObject hist_asym;
  hist_asym.paramX = "phi_h";
  hist_asym.paramY = "phi_R0";
  hist_asym.h2 = new TH2F("h2","",8,-3.141592,3.141592,8,-3.141592,3.141592);
  // Finally, put these pieces together to form the sideband object
  sidebandObject sbObj;
  sbObj.signal = signal;
  sbObj.sideband = sideband;
  sbObj.hist = hist_Mgg;
  sbObj.asym = asym;
  sbObj.hist_asym = hist_asym;
  
  fitter->setSideband(sbObj);
  
  fitter->executeSideband();
  //-----------------------
  // Set sPlot Object
  //-----------------------
  /*  sPlotObject splot;
  splot.outDir = Form("%s/splot_%s",inputDir,wildcard);
  splot.variable = "Mdiphoton[0.08,0.22]";
  splot.idbranch = "fidmerge";
  splot.signalFactoryPDF = "Gaussian::Signal(Mdiphoton, mu[0.13,0.127,0.135], sigma[0.1,0.001,0.3])";
  splot.bgFactoryPDF = "Chebychev::BG(Mdiphoton, {a0[0.1,-1,1], a1[0.1,-1,1], a2[0.1,-1,1]})";

  fitter->extractSWeights(splot);*/
  return 0;
}
