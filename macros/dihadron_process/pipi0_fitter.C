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
  fitObject signal_sbnd;
  signal_sbnd.fitOptions = "SR Q0";
  signal_sbnd.binnedFit1D = new TF1("f_diphoton_sig","gaus(0)",0.108,0.160);
  signal_sbnd.binnedFit1D->SetParameters(100,0.13,0.1);
  fitObject sideband_sbnd;
  sideband_sbnd.fitOptions = "SR Q0";
  sideband_sbnd.binnedFit1D = new TF1("f_diphoton_bg","pol2",0.17,0.4);
  sideband_sbnd.binnedFit1D->SetParameters(10,10,10);
  // Second, create 1D histogram for Mdiphoton binning
  histObject hist_Mgg_sbnd;
  hist_Mgg_sbnd.paramX = "Mdiphoton";
  hist_Mgg_sbnd.h1 = new TH1F("h1_sbnd","",100,0,0.5);
  // Third, create asymmetry fit object
  fitObject asym_sbnd;
  asym_sbnd.fitOptions = "SR Q0";
  asym_sbnd.binnedFit2D = new TF2("f_2D","[0]*sin(2*x-2*y)+[1]*sin(x-y)+[2]*sin(-x+2*y)+[3]*sin(y)+[4]*sin(x)+[5]*sin(2*x-y)+[6]*sin(3*x-2*y)",-3.141592,3.141592,-3.141592,3.141592);
  // Fourth, create 2D histogram for phih phiR binning
  histObject hist_asym_sbnd;
  hist_asym_sbnd.paramX = "phi_h";
  hist_asym_sbnd.paramY = "phi_R0";
  hist_asym_sbnd.h2 = new TH2F("h2_sbnd","",8,-3.141592,3.141592,8,-3.141592,3.141592);
  // Finally, put these pieces together to form the sideband object
  sidebandObject sbndObj;
  sbndObj.signal = signal_sbnd;
  sbndObj.sideband = sideband_sbnd;
  sbndObj.hist = hist_Mgg_sbnd;
  sbndObj.asym = asym_sbnd;
  sbndObj.hist_asym = hist_asym_sbnd;
  
  fitter->setSideband(sbndObj);
  
  fitter->executeSideband();
  //-----------------------
  // Set sPlot Object
  //-----------------------
  // First, create 1D histogram for Mdiphoton binning
  histObject hist_Mgg_splot;
  hist_Mgg_splot.paramX = "Mdiphoton";
  hist_Mgg_splot.h1 = new TH1F("h1_splot","",100,0.08,0.22);
  // Second, create  asymmetry fit object
  fitObject asym_splot;
  asym_splot.fitOptions = "SR Q0";
  asym_splot.binnedFit2D = new TF2("f_2D","[0]*sin(2*x-2*y)+[1]*sin(x-y)+[2]*sin(-x+2*y)+[3]*sin(y)+[4]*sin(x)+[5]*sin(2*x-y)+[6]*sin(3*x-2*y)",-3.141592,3.141592,-3.141592,3.141592);
  // Third, create 2D histogram for phih phiR binning
  histObject hist_asym_splot;
  hist_asym_splot.paramX = "phi_h";
  hist_asym_splot.paramY = "phi_R0";
  hist_asym_splot.h2 = new TH2F("h2_splot","",8,-3.141592,3.141592,8,-3.141592,3.141592);
  // Fourth, load in brufit variables to the sPlot object
  sPlotObject splotObj;
  splotObj.variable = "Mdiphoton[0.08,0.22]";
  splotObj.idbranch = "fidmerge";
  splotObj.signalFactoryPDF = "Gaussian::Signal(Mdiphoton, mu[0.13,0.127,0.135], sigma[0.1,0.001,0.3])";
  splotObj.bgFactoryPDF = "Chebychev::BG(Mdiphoton, {a0[0.1,-1,1], a1[0.1,-1,1], a2[0.1,-1,1]})";
  splotObj.hist = hist_Mgg_splot;
  splotObj.asym = asym_splot;
  splotObj.hist_asym = hist_asym_splot;
  
  fitter->setSplot(splotObj);

  fitter->executeSplot();
  return 0;
}
