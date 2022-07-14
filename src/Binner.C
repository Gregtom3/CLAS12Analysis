#include "Binner.h"

Binner::Binner(const char * tName, const char * fName, int v=0)
{
  _tName = tName;
  _fName = fName;

  _verbosity = v;
  if(_verbosity > 0)
    cout << " \n Beginning Binner script \n" << endl;

}

int Binner::create1DBinning(const char * binVar , float varMin, float varMax, bool helSplit){
  // Generate cut statement
  TString binCut = Form("%s > %f && %s < %f",binVar,varMin,binVar,varMax);
  // Append binning to list
  _binCuts.push_back(binCut.Data());
  // Generate unique output tree name
  if(helSplit==true){
    TString treeName = Form("bin_hel_plus_%s_%f_%f",binVar,varMin,varMax); 
    _treeNames.push_back(treeName.Data());
    _helicity_flag.push_back(1);

    _binCuts.push_back(binCut.Data()); // Append binning again

    treeName = Form("bin_hel_minus_%s_%f_%f",binVar,varMin,varMax); 
    _treeNames.push_back(treeName.Data());
    _helicity_flag.push_back(-1);
  }
  else{
    TString treeName = Form("bin_%s_%f_%f",binVar,varMin,varMax); 
    _treeNames.push_back(treeName.Data());
    _helicity_flag.push_back(0);
  }
  return 0;
}

int Binner::create1DBinnings(const char * binVar, std::vector<float> vars, bool helSplit){
  for(unsigned int idx = 0; idx < vars.size()-1;idx++){
    float varMin = vars.at(idx);
    float varMax = vars.at(idx+1);
    create1DBinning(binVar,varMin,varMax, helSplit);
  }
  return 0;
}

int Binner::create2DBinning(const char * binxVar , float varxMin, float varxMax,
			    const char * binyVar , float varyMin, float varyMax, bool helSplit){
  // Generate cut statements
  TString binCut = Form("%s > %f && %s < %f && %s > %f && %s < %f",binxVar,varxMin,binxVar,varxMax,binyVar,varyMin,binyVar,varyMax);
  // Append binning to list
  _binCuts.push_back(binCut.Data());
  // Generate unique output tree name
  if(helSplit==true){
    TString treeName = Form("bin_hel_plus_%s_%f_%f_%s_%f_%f",binxVar,varxMin,varxMax,
			                      binyVar,varyMin, varyMax); 
    _treeNames.push_back(treeName.Data());
    _helicity_flag.push_back(1);

    _binCuts.push_back(binCut.Data()); // Append binning again
    
    treeName = Form("bin_hel_minus_%s_%f_%f_%s_%f_%f",binxVar,varxMin,varxMax,
			                      binyVar,varyMin, varyMax); 
    _treeNames.push_back(treeName.Data());
    _helicity_flag.push_back(-1);
  }
  else{
    TString treeName = Form("bin_%s_%f_%f_%s_%f_%f",binxVar,varxMin,varxMax,
			                      binyVar,varyMin, varyMax); 
    _treeNames.push_back(treeName.Data());
    _helicity_flag.push_back(0);
  }
  return 0;
}

int Binner::create2DBinnings(const char * binxVar , std::vector<float> varsx,
			     const char * binyVar , float varyMin, float varyMax, bool helSplit){
 for(unsigned int idx = 0; idx < varsx.size()-1;idx++){
    float varxMin = varsx.at(idx);
    float varxMax = varsx.at(idx+1);
    create2DBinning(binxVar,varxMin,varxMax,
		    binyVar,varyMin,varyMax, helSplit);
 }
 return 0;
}

int Binner::processBins()
{
  _fIn = new TFile(_fName,"UPDATE");
  // Create basic ROOT::RDataFrame
  _dfIn = ROOT::RDataFrame(_tName,_fName);

  // Open the TFile
  _fIn->cd();
  // Get number of bins
  const int nBins = _treeNames.size();
  
  // For each binning...
  for(int i = 0 ; i < nBins ; i++){
    // Delete TTree if it is recognized
    if(_fIn->Get(_treeNames.at(i).c_str()))
      gDirectory->Delete(_treeNames.at(i).c_str());

    // Create TTree pointer for bin
    TTree * outTree = new TTree(_treeNames.at(i).c_str(),"");
    outTree->Branch("phih",&_phih);
    outTree->Branch("phiR",&_phiR);
    outTree->Branch("theta",&_theta);
<<<<<<< HEAD
    outTree->Branch("Mdiphoton",&_Mdiphoton);
    outTree->Branch("Mdihadron",&_Mdihadron);
=======
>>>>>>> e46f75b74d42eaa76b0775520b0eb5e35bd7742f
    if(_helicity_flag.at(i)==0){
      outTree->Branch("helicity",&_helicity);
    }
    outTree->Branch("fidmerge",&_fidmerge);
    // Fill dihadron angles into binned trees
    if(_verbosity > 0)
      cout << " Filling Bin --> " << _treeNames.at(i) << endl;
    
    if(_helicity_flag.at(i)==1)
      _binCuts.at(i)+=" && helicity == 1";
    else if(_helicity_flag.at(i)==-1)
      _binCuts.at(i)+=" && helicity == -1";

    _dfIn.Filter(_binCuts.at(i)).Foreach([&](double phih, double phiR, double theta, double Mdiphoton, double Mdihadron,float helicity, int fidmerge)
					 { _phih = phih;
					   _phiR = phiR;
					   _theta = theta;
					   _Mdiphoton = Mdiphoton;
					   _Mdihadron = Mdihadron;
					   _helicity = helicity;
					   _fidmerge = fidmerge;
					   outTree->Fill();},
					 {"phi_h","phi_R0","th","Mdiphoton","Mdihadron","helicity","fidmerge"});
    
    // Write TTree
    outTree->Write();
  }
  _fIn->Close();
  return 0;
}

void Binner::printBinsToFile(const char * c){
  ofstream fOut(Form("%s",c),std::ofstream::out);
  for(std::string binname : _treeNames)
    fOut << binname << "\n";
  fOut.close();
}
