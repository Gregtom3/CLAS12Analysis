#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
// If anybody can figure out how to have root automically search in the include
// directory for this file, it would be GREATLY appreciated! 
R__LOAD_LIBRARY(libbrufit.so)
R__LOAD_LIBRARY(libclas12ana.so)
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/Asymmetry.h"
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/Binner.h"
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/Fitter.h"
#endif

using namespace std;

void binner(const char *);
void fitter(const char *, const char *, const char *);
void asym(const char *);


int pipi0_asymmetry(
		    const char * inputDir = "/volatile/clas12/users/gmat/clas12analysis.sidis.data/fall2018-torus-1-small-batch/july14",
		    const char * rootName = "merged_july14",
		    const char * treeName = "bin_x_0.100000_0.130000",
		    int stage = 2
)
{
  
  if(stage < 1 || stage > 3){
    cout << "ERROR in pipi0_asymmetry() --- There are only three stages to the asymmetry procedure, which are Binning --> Fitting --> Asymmetry Plots. The stage provided ("<<stage<<") must be either 1, 2, or 3. Return -1" << endl;
    return -1;
  }

  //---------------
  // Load libraries
  //---------------
  gSystem->Load("libclas12ana.so"); 

  //--------------------
  // Run specified stage
  //--------------------
  switch(stage){
  case 1:
    binner(Form("%s/%s.root",inputDir,rootName));
    break;
  case 2:
    fitter(inputDir,rootName,treeName);
    break;
  case 3:
    asym(inputDir);
    break;
  }
  
  cout << " Stage " << stage << " of Asymmetry procedure complete " << endl;
  return 0;
}

// void binner(const char *)
// Purpose: After completing the PostProcessing analysis of .hipo files, this user
// editable script opens a input.root file, opens the 'tree_postprocess' TTree, and
// creates+saves new TTrees to input.root which effectively split 'tree_postprocess'
// into 'binned trees'. 

// For example, the user can define x bins [0,0.5] and [0.5,1.0]. 
// This script will then open 'tree_postprocess' and 'copy' it twice.
// Yet, each copy will only contain events with x [0,0.5] and [0.5,1.0] respectively
void binner(const char * inputFile){
  
  //---------------
  // Set verbosity
  //---------------
  int verbosity = 1;

  //-----------------------
  // Create Binner Object
  //-----------------------
  Binner *bb = new Binner("tree_postprocess",inputFile,verbosity);

  //-----------------------
  // Create x, z, Mh bins
  //-----------------------
  std::vector<float> xbins = {0.1,0.13,0.16,0.19,0.235,0.3,0.5};
  std::vector<float> zbins = {0.35,0.43,0.49,0.55,0.62,0.7,0.83};
  std::vector<float> Mhbins;
  for(float mh = 0.3; mh < 1.3; mh+=1.0/7.0)
    Mhbins.push_back(mh);

  //------------------------------
  // Load bins into Binner Object
  // (true) --> split "+" and "-" helicities DEPRECATED: Should be removed
  // (false) --> combine "+" and "-" helicities
  //------------------------------
  bb->create1DBinnings("x",xbins,false);
  bb->create1DBinnings("z",zbins,false);
  bb->create1DBinnings("Mdihadron",Mhbins,false);  
  //----------------------------------------
  // Process Bins, saving result into TFile
  //----------------------------------------
  bb->processBins();
  //----------------------------------------
  // Save bin names to text file
  // Necessary for fitting macro
  //----------------------------------------
  bb->printBinsToFile(Form("%s.txt",inputFile));
  
  return;
}


// void fitter(const char * , const char *, const char *)
// NEED EXPLANATION
void fitter(const char *inputDir, const char *rootName, const char *treeName){
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
  hist_asym_sbnd.paramX = "phih";
  hist_asym_sbnd.paramY = "phiR";
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
  hist_asym_splot.paramX = "phih";
  hist_asym_splot.paramY = "phiR";
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
  return;
}

// void asym(const char *)
// NEED EXPLANATION
void asym(const char * dirPath){
  //-----------------------
  // Create Asymmetry Object
  //-----------------------
  Asymmetry * asymmetry = new Asymmetry(dirPath);
  //-----------------------
  // Print asymmetry bins
  //-----------------------
  asymmetry->print();
  //-----------------------
  // Run asymmetry module
  //-----------------------
  asymmetry->process();
  return 0;
}
