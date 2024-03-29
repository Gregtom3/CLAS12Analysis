// ------------------------------------------------------------------
// 
//
//  tutorial_1_rga_dihadron.C
//
// 
// ------------------------------------------------------------------
/*
  
  In this default tutorial file, we will be analyzing a relatively small hipo4 file. 

  The user makes edits within this file in three locations...
    - Defining the 'const char * hipoFile' 
    - Defining the 'const char * outputFile' 
    - Manipulating the analysis preferences with the Settings object

  This file is executed with the command 'clas12root tutorial_1_rga_dihadron.C'

  By default, the cuts made are...
    - Number reconstructed electrons = 1
    - Number reconstructed photons >=  2
    - Number reconstructed pi+ = 1
    - Energy of all photons > 0.6 GeV
    - Momentum of pi+ > 1.25 GeV
    - abs(Chi2pid) of pi+ < 3 
    - Q2 > 1 GeV^{2}
    - W > 2 GeV
    - y < 0.8

  Additionally, the default pipluspi0 postprocessing module is loaded, and will run after all events from the hipo file are processed. Read the script PostProcess.C for details.

  The output of this tutorial are three TTree's
    - tree_MC --> stored Monte Carlo particle info
    - tree_reco --> stored reconstructed particle info
    - tree_postprocess --> stores additional event-by-event info based on tree_reco

*/

#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
// If anybody can figure out how to have root automically search in the include
// directory for this file, it would be GREATLY appreciated! 
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/SIDISKinematicsReco.h"
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/Settings.h"
R__LOAD_LIBRARY(libclas12ana.so)
#endif

using namespace std;



int tutorial_1_rga_dihadron(
		  const char * hipoFile = "/w/hallb-scshelf2102/clas12/users/gmat/CLAS12Analysis/data/smallhipo/small_MC_sample.hipo",
		  const char * outputFile = "./tutorial_1_output.root"
		  )
{
  
  //---------------
  // Load libraries
  //---------------
  gSystem->Load("libclas12ana.so"); 
 
  //-----------------------------------
  // Create SIDISKinematicsReco Object 
  //-----------------------------------
  SIDISKinematicsReco *ana = new SIDISKinematicsReco(outputFile);
  ana->set_verbosity(1);
  ana->set_printFrequency(100000);
  
  //-----------------------------------
  // **** EDIT BELOW ****
  //
  // 
  // Create Settings for Study
  //-----------------------------------
  Settings settings;
  settings._electronBeamEnergy = 10.6; // Do not change, since this is the beam energy for the corresponding file
 
  // Settings for the analysis
  settings._ignoreOtherRecoParticles = true; // Do not save to reco tree particle info of uninterested PIDs
  settings._doMC = true;             // Analyze MC::Lund
  settings._doReco = true;           // Analyze REC::Particle
  settings._doPostProcess = true;    // Apply further cuts
  settings._connectMC2Reco = true;   // Connect pindex of REC::Particle to pindex of MC::Lund
  settings._postProcessMethod = "pipluspi0_MC"; // Perform monte carlo pipluspi0 processing
  
  // Event cuts
  settings._Q2min = 1; settings._Q2max = 100;
  settings._Wmin = 2; settings._Wmax = 100;
  settings._ymin = 0; settings._ymax = 0.8;

  settings.addFinalState(11,1,true);  // Exactly 1 electron 
  settings.addFinalState(211,1,false); // Exactly 1 pi+
  settings.addFinalState(22,2,false); // 2 or more gammas
  
  // Particle Cuts
  settings.addPIDforEmin(22,0.6);     // Gammas must have minimum energy of 0.6 GeV
  settings.addPIDforPmin(211,1.25);   // Pi+ must have minimum momentum of 1.25 GeV
  settings.addPIDforChi2max(211,3);        // Pi+ must have abs(chi2pid) < 3 
  settings._eventRecoMethod = Settings::eventRecoMethod::useLargestPinFD; // Use the status < 0 trigger for identifying the reconstructed scattered electron  
  
  //-----------------------------------
  // ***No need to edit past here***
  // These functions setup the analysis
  //-----------------------------------
  settings.addHipoFile(hipoFile);
  //-----------------------------------
  // Import Settings into Processing Framework
  //-----------------------------------
  ana->ImportSettings(settings);
  //-----------------------------------
  // Initialize the Analysis Module
  //-----------------------------------
  ana->Init();
  //-----------------------------------
  // Process all events
  //-----------------------------------
  ana->process_events();
  //-----------------------------------
  // Perform post-processing
  //-----------------------------------
  ana->PostProcessReco();
  //-----------------------------------
  // End processing
  //-----------------------------------
  ana->End();
  return 0;
}
