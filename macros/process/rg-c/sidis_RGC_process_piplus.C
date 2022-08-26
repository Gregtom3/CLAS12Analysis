#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
// If anybody can figure out how to have root automically search in the include
// directory for this file, it would be GREATLY appreciated! 
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/SIDISKinematicsReco.h"
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/Settings.h"
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/Binner.h"
R__LOAD_LIBRARY(libclas12ana.so)
#endif

using namespace std;

int sidis_RGC_process_piplus(
		  const char * hipoFile = "/volatile/clas12/rg-c/production/ana_data/dst/train/sidisdvcs/sidisdvcs_016338.hipo",
		  const char * outputFile = "rgc-test.root",
		  const double beamE = 10.5
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
  ana->set_printFrequency(10000);

  //-----------------------------------
  // Create Settings for Study
  //-----------------------------------
  Settings settings;
 
  settings._electronBeamEnergy = beamE; 
  //  settings._maxEvents = 10000;
  settings._doMC = false;
  settings._doReco = true;
  settings._doQADB = false; // Turn off QADB for RGC as of now
  settings._eventRecoMethod = Settings::eventRecoMethod::useLargestPinFD;
  settings._connectMC2Reco = false; // Connect pindex of REC::Particle to pindex of MC::Lund

  settings._Q2min = 0; settings._Q2max = 100;
  settings._Wmin = 2; settings._Wmax = 100;
  settings._ymin = 0; settings._ymax = 1;
  
  settings.addFinalState(11,1,true);  // Exactly 1 electron
  settings.addFinalState(211,1,false); // At least 1 pi+
  settings.addFinalState(2212,1,true); // Exactly 1 proton
  //  settings.addOpenInvite(2212); // Allow for protons to be saved if found
  settings._ignoreOtherRecoParticles = true; // Do not save to tree particle info of uninterested PIDs

  //  settings.addPIDforPmin(2212,1);   // Proton minimum 1 GeV
  //  settings.addPIDforVzrange(11,-13,12); // e- must have vertex 'z' between [-13,12] cm

  settings._doFiducialCuts = false;   // Perform fiducial cuts
  settings._doPostProcess = false;    // Apply further cuts

  settings.addHipoFile(hipoFile);
  
  // ----------------------------------------------------------------------------
  // -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  // ----------------------------------------------------------------------------

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
  // ----------------------------------------------------------------------------
  // -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  // ----------------------------------------------------------------------------  
  return 0;
}
