#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
// If anybody can figure out how to have root automically search in the include
// directory for this file, it would be GREATLY appreciated! 
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/SIDISKinematicsReco.h"
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/Settings.h"
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/Binner.h"
//R__LOAD_LIBRARY(libclas12ana.so)
#endif

using namespace std;

int pipi0_process(
		  //const char * hipoFile = "/cache/clas12/rg-a/production/montecarlo/clasdis/fall2018/torus-1/v1/bkg45nA_10604MeV/45nA_job_3301_3.hipo",
		  //const char * hipoFile = "/cache/clas12/rg-a/production/montecarlo/clasdis/fall2018/torus-1/v0/nobkg_10604MeV/DIS_pass1_1003_1008.hipo",
		  const char * hipoFile = "/w/hallb-scshelf2102/clas12/users/gmat/CLAS12Analysis/data/smallhipo/Out_DIS_pass1_915_920.hipo_skim23.hipo",
		  //const char * hipoFile = "/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v1/dst/train/nSidis/nSidis_005032.hipo",
		  //const char * outputFile = "/work/clas12/users/gmat/CLAS12Analysis/data/fall2018-torus+1-v1-nSidis/june5_287.root",
		  const char * outputFile = "test.root",
		  const double beamE = 10.6
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
  settings._doMC = true;
  settings._doReco = true;
  settings._doQADB = true;
  settings._doRCDB = false;
  settings._eventRecoMethod = Settings::eventRecoMethod::useLargestPinFD;
  settings._connectMC2Reco = true; // Connect pindex of REC::Particle to pindex of MC::Lund

  settings._Q2min = 1; settings._Q2max = 100;
  settings._Wmin = 2; settings._Wmax = 100;
  settings._ymin = 0; settings._ymax = 0.8;
  
  settings.addFinalState(11,1,true);  // Exactly 1 electron
  settings.addFinalState(211,1,false); // Exactly 1 pi+
  settings.addFinalState(22,2,false); // 2 or more gammas
  settings._ignoreOtherRecoParticles = true; // Do not save to tree particle info of uninterested PIDs

  settings.addPIDforEmin(22,0.6);     // Gammas must have minimum energy of 0.6 GeV
  settings.addPIDforPmin(211,1.25);   // Pi+ must have minimum momentum of 1.25 GeV
  settings.addPIDforVzrange(11,-8,3); // e- must have vertex 'z' between [-13,12] cm
  settings.addPIDforBetarange(22,0.9,1.1); // Beta range for photon
  settings.addPIDforChi2max(211,3);        // Pi+ must have abs(chi2pid) < 3 
  settings._chargedPionChi2cut = Settings::chargedPionChi2cut::standard; // See RGA analysis note

  settings._doFiducialCuts = true;   // Perform fiducial cuts
  settings._doPostProcess = true;    // Apply further cuts
  settings._postProcessMethod = "pipluspi0_MC"; // Perform pipluspi0 default processing
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
  // Perform post-processing
  //-----------------------------------
  ana->PostProcessReco();
  //-----------------------------------
  // End processing
  //-----------------------------------
  ana->End();
  // ----------------------------------------------------------------------------
  // -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  // ----------------------------------------------------------------------------  
  return 0;
}
