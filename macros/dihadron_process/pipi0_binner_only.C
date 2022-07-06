#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
// If anybody can figure out how to have root automically search in the include
// directory for this file, it would be GREATLY appreciated! 
#include "/work/clas12/users/gmat/CLAS12Analysis/include/clas12ana/Binner.h"
R__LOAD_LIBRARY(libclas12ana.so)
#endif

using namespace std;

int pipi0_binner_only(
			   const char * inputFile = "/work/clas12/users/gmat/CLAS12Analysis/macros/dihadron_process/test.root",
			   const double beamE = 10.6
)
{
  //---------------
  // Load libraries
  //---------------
  gSystem->Load("libclas12ana.so"); 
  
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
  // (true) --> split "+" and "-" helicities
  // (false) --> combine "+" and "-" helicities
  //------------------------------
  bb->create1DBinnings("x",xbins,true);
  bb->create1DBinnings("z",zbins,true);
  bb->create1DBinnings("Mdihadron",Mhbins,true);

  bb->create1DBinnings("x",xbins,false);
  bb->create1DBinnings("z",zbins,false);
  bb->create1DBinnings("Mdihadron",Mhbins,false);  
  //----------------------------------------
  // Process Bins, saving result into TFile
  //----------------------------------------
  bb->processBins();

  return 0;
}
