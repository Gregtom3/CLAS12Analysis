// ------------------------------------------------------------------
// 
//
//  tutorial_2a_rgc_inclusive.C
//
// 
// ------------------------------------------------------------------
/*
  
  <<<>>> To properly execute this tutorial, the user must make appropriate
  edits in the "tutorial_2b_rgc_inclusive.sh" shell script and/or
  "tutorial_2c_rgc_inclusive.sh"
  <<<>>>

  <<<>>> You are required to have a directory with your username in ...
    - /volatile/clas12/users/<USERNAME>
    - /farm_out/<USERNAME>
  <<<>>>

  In this default tutorial file, we will be analyzing... 
    - (2b) 4 NH3 runs for RG-C (16327 , 16328, 16329, 16330)
    - (2c) All RG-C runs in the sidisdvcs train

  The user makes edits within this file in three locations...
    - Manipulating the analysis preferences with the Settings object
    - tutorial_2b_rgc_inclusive.sh
    - tutorial_2c_rgc_inclusive.sh

    
  The role of the shell scripts are to send analysis jobs to slurm, run-by-run
  This way, we can analyze many runs in parallel, cutting down wait-time
  So far, CLAS12Analysis is designed to handle single HIPO files anyway, despite using HipoChain's

  The output of this script are 4 (or many more) separate .root files, each containing a single TTree "tree_reco"

  Run this program (after making edits to tutorial_2[b-c]_rgc_inclusive.sh) by running...
     ./tutorial_2b_rgc_inclusive.sh
     OR
     ./tutorial_2c_rgc_inclusive.sh
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



int tutorial_2_rgc_inclusive(
		  const char * hipoFile = "",
		  const char * outputFile = "",
		  const double beamE = 10.5473)
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
  settings._electronBeamEnergy = beamE; // Do not change, since this is the beam energy for the corresponding file
 
  // Settings for the analysis
  settings._ignoreOtherRecoParticles = true; // Do not save to reco tree particle info of uninterested PIDs
  settings._doReco = true;           // Analyze REC::Particle
  settings._maxEvents = 10000;       // Only analyze 10k events per HIPO file 

  // Event cuts
  settings._Q2min = 1; settings._Q2max = 100;
  settings._Wmin = 2; settings._Wmax = 100;
  settings._ymin = 0; settings._ymax = 0.8;

  settings.addFinalState(11,1,true);  // Exactly 1 electron 
  
  // Particle Cuts
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
  // End processing
  //-----------------------------------
  ana->End();
  return 0;
}
