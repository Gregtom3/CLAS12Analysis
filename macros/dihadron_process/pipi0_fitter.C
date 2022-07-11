#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
// If anybody can figure out how to have root automically search in the include
// directory for this file, it would be GREATLY appreciated! 
R__LOAD_LIBRARY(libbrufit.so)
R__LOAD_LIBRARY(libclas12ana.so)
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/Fitter.h"
#endif

using namespace std;

int pipi0_fitter(
		 const char * inputDir = "/volatile/clas12/users/gmat/clas12analysis.sidis.data/fall2018-torus-1-small-batch/july8",
		 const char * inputFile = "merged_july8.root",
		 const char * wildcard = "z_0.7000"
)
{
  //---------------
  // Load libraries
  //---------------
  gSystem->Load("libclas12ana.so"); 
  
  //-----------------------
  // Create Fitter Object
  //-----------------------
  Fitter *fitter = new Fitter(Form("%s/%s",inputDir,inputFile));
  fitter->setWildcard(wildcard);
  fitter->printKeys();

  //-----------------------
  // Set sPlot Object
  //-----------------------
  sPlotObject splot;
  splot.outDir = Form("%s/splot_%s",inputDir,wildcard);
  splot.variable = "Mdiphoton[0.08,0.22]";
  splot.idbranch = "fidmerge";
  splot.signalFactoryPDF = "Gaussian::Signal(Mdiphoton, mu[0.13,0.127,0.135], sigma[0.1,0.001,0.3])";
  splot.bgFactoryPDF = "Chebychev::BG(Mdiphoton, {a0[0.1,-1,1], a1[0.1,-1,1], a2[0.1,-1,1]})";

  fitter->extractSWeights(splot);
  return 0;
}
