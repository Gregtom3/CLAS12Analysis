#include "Fitter.h"

using namespace std;

Fitter::Fitter(std::string filepath){
  _filepath = filepath;
  _fIn = new TFile(filepath.c_str(),"READ");
}

int Fitter::setWildcard(std::string wildcard){
  if(wildcard.find('*')!=std::string::npos){
    cout << "ERROR in Fitter::setWildcard --- Do not use '*' in wildcard string ... return -1" << endl;
    return -1;
  }
  else if(getKeyNames(wildcard).size()==0){
    cout << "ERROR in Fitter::setWildcard --- No TObjects in " << _fIn->GetName() << " contains the keyword " << wildcard << " ... return -1" << endl;
    return -1;
  }
  else
    _wildcard = wildcard;
  return 0;
}

void Fitter::setSideband(sidebandObject obj){
  _sideband = obj;
}

void Fitter::setSplot(sPlotObject obj){
  _splot = obj;
}

int Fitter::executeSideband(histObject histobj){
  //  fitObject signal = _sideband.signal;
  // fitObject sideband = _sideband.sideband;
  return 0;
  
}

int Fitter::extractSWeights(sPlotObject splot){
  std::vector<std::string> keyNames = getKeyNames(_wildcard);
  std::string outDir = splot.outDir;
  HS::FIT::sPlot RF;
  RF.SetUp().SetOutDir(outDir.c_str());
  //////////////////////////////////// Load Variables
  RF.SetUp().LoadVariable(splot.variable);
  RF.SetUp().SetIDBranchName(splot.idbranch);
  //////////////////////////////////// Make Signal
  RF.SetUp().FactoryPDF(splot.signalFactoryPDF);
  RF.SetUp().LoadSpeciesPDF("Signal",1);
  //////////////////////////////////// Make BG
  RF.SetUp().FactoryPDF(splot.bgFactoryPDF);
  RF.SetUp().LoadSpeciesPDF("BG",1);
  //////////////////////////////////// Load Data
  for(std::string keyName : keyNames){
    RF.LoadData(keyName.c_str(),_filepath.c_str());
    HS::FIT::PROCESS::Here::Go(&RF);
    RF.saveWeightedTree(Form("%s/DataWeightedTree.root",outDir.c_str()));
  }
  return 0;
}

int Fitter::perform1DBinnedFits(fitObject fitobj, histObject histobj){
  // Ensure fitobj parameters are correctly defined
  if(checkFitObj(fitobj)!=0)
    return -1;

  // Enter into TFile
  _fIn->cd();
  
  // Grab std::vector of keyNames
  std::vector<std::string> keyNames = getKeyNames(_wildcard);

  // Create fitting object
  TF1 *f1 = new TF1("f1",fitobj.fitFunction.c_str(),
  		    fitobj.xmin,fitobj.xmax,fitobj.npars);

  // For loop over all TTrees in TFile
  for(unsigned int i = 0 ; i < keyNames.size() ; i++){

    // Histogram name based on forloop index
    TString histName = Form("%s_%d",histobj.name.c_str(),i);
    
    // Set fit parameters and ranges for each histogram
    for(int j = 0 ; j < fitobj.npars ; j++){
      f1->SetParameter(j,fitobj.initialPars.at(j));
      f1->SetParLimits(j,fitobj.initialParRanges.at(j).first,
		         fitobj.initialParRanges.at(j).second);
    }
    
    // Create histogram object
    TH1F * h1 = new TH1F(histName,TString(histobj.title),
			 histobj.xbins,histobj.xmin,histobj.xmax);
    
    // Create and draw into TTree
    TTree * t = (TTree*)_fIn->Get(TString(keyNames.at(i)));
    t->Draw(Form("%s>>%s",histobj.paramX.c_str(),histName.Data()),"","goff");
    
    // Fit the histogram
    TFitResultPtr r = h1->Fit(f1, TString(fitobj.fitOptions));
    r->SetName(Form("fit_%s",keyNames.at(i).c_str()));
    
    // ***** ISSUE
    // Only one binnedFit per fit object saved
    // Should rewrite Fitter macro to only fit one tree in one file, not multiple
    //    fitobj.binnedFit = r;
  }
  return 0;
}

std::vector<std::string> Fitter::getKeyNames(std::string wildcard){
  
  // Vector of keyNames (essentially, TObject names in the TFile)
  std::vector<std::string> keyNames;
  
  // Enter into TFile
  _fIn->cd();  
  
  // Dummy TKey variable
  TKey *key;

  // TIter
  TIter _fKeys(_fIn->GetListOfKeys());

  // For each key in the TFile
  while((key=(TKey*)_fKeys())){

    // Get the title of the key
    std::string keyTitle =  key->GetName();
    // Test if the keyTitle contains the wildcard
    // Sometimes, duplicate keys are stored in the TTree. To deal with this, insist the cycle to be 0 or 1
    if(keyTitle.find(wildcard)!=std::string::npos && key->GetCycle()<=1)
      keyNames.push_back(keyTitle);
  }
  
  return keyNames;
}

int Fitter::checkFitObj(fitObject fitobj){
  if(fitobj.npars!=(int)(fitobj.initialPars.size()))
    {
      cout << "ERROR in Fitter::checkFitObj --- npars != initialPars.size() ... return -1" << endl;
      return -1;
    }
  else if(fitobj.npars!=(int)(fitobj.initialParRanges.size()))
    {
      cout << "ERROR in Fitter::checkFitObj --- npars != initialParRanges.size() ... return -1" << endl;
      return -1;
    }
  return 0;
}
 
void Fitter::printKeys(){
  std::vector<std::string> keyNames = getKeyNames(_wildcard);
  cout << "Printing keyNames in TFile " << _fIn->GetName() << endl;
  if(_wildcard.compare("")!=0)
    cout << " Wildcard --> " << _wildcard << endl;
  else
    cout << " No wildcard given" << endl;
  for(std::string kname : keyNames)
    cout << kname << endl;
}

int Fitter::End(){
  _fIn->Close();
  return 0;
}
