#include "Fitter.h"

using namespace std;

Fitter::Fitter(std::string filepath, std::string rootname, std::string treeName){
  _filepath = filepath;
  _rootname = rootname;
  _treeName = treeName;
  _fIn = new TFile(Form("%s/%s.root",filepath.c_str(),rootname.c_str()),"READ");
}

void Fitter::setSideband(sidebandObject obj){
  _sideband = obj;
}

void Fitter::setSplot(sPlotObject obj){
  _splot = obj;
}

int Fitter::executeSideband(){
  cout << "\n\n\nExecuting Sideband Method\n\n\n"<<endl;
  // Declaring Input TTree
  _fIn->cd();
  _tree = (TTree*)_fIn->Get(_treeName.c_str());
  // Create new TFile based on TTree name
  cout << "Sideband Method --- Removing sideband bin directory | " << Form("rm -r %s/sideband_%s",_filepath.c_str(),_treeName.c_str()) << endl; 
  gSystem->Exec(Form("rm -r %s/sideband_%s",_filepath.c_str(),_treeName.c_str()));
  cout << "Sideband Method --- Remaking sideband bin directory " << endl;
  gSystem->Exec(Form("mkdir %s/sideband_%s",_filepath.c_str(),_treeName.c_str()));
  cout << "Sideband Method --- Creating TFile for TObject saving " << endl;
  TFile *fOut = new TFile(Form("%s/sideband_%s/sidebandMethod.root",_filepath.c_str(),_treeName.c_str()),"RECREATE");
  fOut->cd();
  TTree *tOut = new TTree("tree","Sideband Method Related Variables");
  std::vector<double> sig_params_u0;
  std::vector<double> sig_errors_u0;
  std::vector<double> sig_params_u1;
  std::vector<double> sig_errors_u1;
  std::vector<double> sig_params_u2;
  std::vector<double> sig_errors_u2;
  std::vector<double> sig_params_u3;
  std::vector<double> sig_errors_u3;
  std::vector<double> sigbg_params_u0;
  std::vector<double> sigbg_errors_u0;
  std::vector<double> sigbg_params_u1;
  std::vector<double> sigbg_errors_u1;
  std::vector<double> sigbg_params_u2;
  std::vector<double> sigbg_errors_u2;
  std::vector<double> sigbg_params_u3;
  std::vector<double> sigbg_errors_u3;
  std::vector<double> bg_params_u0;
  std::vector<double> bg_errors_u0;
  std::vector<double> bg_params_u1;
  std::vector<double> bg_errors_u1;
  std::vector<double> bg_params_u2;
  std::vector<double> bg_errors_u2;
  std::vector<double> bg_params_u3;
  std::vector<double> bg_errors_u3;
  double u0, u1, u2, u3;
  tOut->Branch("u0",&u0);
  tOut->Branch("u1",&u1);
  tOut->Branch("u2",&u2);
  tOut->Branch("u3",&u3);
  tOut->Branch("A_sig_u0",&sig_params_u0);
  tOut->Branch("A_sig_errors_u0",&sig_errors_u0);
  tOut->Branch("A_sig_u1",&sig_params_u1);
  tOut->Branch("A_sig_errors_u1",&sig_errors_u1);
  tOut->Branch("A_sig_u2",&sig_params_u2);
  tOut->Branch("A_sig_errors_u2",&sig_errors_u2);
  tOut->Branch("A_sig_u3",&sig_params_u3);
  tOut->Branch("A_sig_errors_u3",&sig_errors_u3);
  tOut->Branch("A_sigbg_u0",&sigbg_params_u0);
  tOut->Branch("A_sigbg_errors_u0",&sigbg_errors_u0);
  tOut->Branch("A_sigbg_u1",&sigbg_params_u1);
  tOut->Branch("A_sigbg_errors_u1",&sigbg_errors_u1);
  tOut->Branch("A_sigbg_u2",&sigbg_params_u2);
  tOut->Branch("A_sigbg_errors_u2",&sigbg_errors_u2);
  tOut->Branch("A_sigbg_u3",&sigbg_params_u3);
  tOut->Branch("A_sigbg_errors_u3",&sigbg_errors_u3);
  tOut->Branch("A_bg_u0",&bg_params_u0);
  tOut->Branch("A_bg_errors_u0",&bg_errors_u0);
  tOut->Branch("A_bg_u1",&bg_params_u1);
  tOut->Branch("A_bg_errors_u1",&bg_errors_u1);
  tOut->Branch("A_bg_u2",&bg_params_u2);
  tOut->Branch("A_bg_errors_u2",&bg_errors_u2);
  tOut->Branch("A_bg_u3",&bg_params_u3);
  tOut->Branch("A_bg_errors_u3",&bg_errors_u3);
  // Perform binned fits in signal and sideband region
  fitObject fitobj = _sideband.fitobj;
  cout << "Sideband Method --- Fitting Signal+Sideband Region"<<endl;
  perform1DBinnedFits(fitobj,_sideband.hist);
  // Get signal+sideband fit
  TF1 * f = (TF1*)fitobj.binnedFit1D->Clone();
  cout << "Sideband Method --- Fit Parameters" << endl;
  for(int i = 0 ; i < f->GetNpar(); i++){
    cout << "\t Parameter " << i+1 << " | " << f->GetParameter(i) << " +/- " << f->GetParError(i) << endl;
  }
  // Get histogram
  TH1F * h = (TH1F*)_sideband.hist.h1->Clone();
  // Get +/- 2 StdDev from Gaussian peak
  // Below code may need to be re-written if signal is not fit to gaussian
  double xleft = f->GetParameter(1)-2*f->GetParameter(2);
  double xright = f->GetParameter(1)+2*f->GetParameter(2);
  double xmax = h->GetBinCenter(h->GetNbinsX());
  cout << "Sideband Method --- +/- 2 StdDev purity calculations [" << xleft << "," << xright << "]" <<endl;
  // Get integral parameters for purity calculations
  // Going to need to change b/c we assume gaussian + pol4 below
  TF1 * ftemp_sigbg = new TF1("ftemp_sigbg","gaus(0)",xleft,xright);
  TF1 * ftemp_bg = new TF1("ftemp_bg","pol4(0)",xright,xmax);
  for(int i = 0; i < 3 ; i++)
    ftemp_sigbg->SetParameter(i,f->GetParameter(i));
  for(int i = 3; i < 8 ; i++)
    ftemp_bg->SetParameter(i-3,f->GetParameter(i));
  double total_hist_sum = h->Integral(h->FindBin(xleft), h->FindBin(xright));
  double sig_fit_integral = ftemp_sigbg->Integral(xleft,xright)/h->GetBinWidth(1);
  double bg_fit_integral = ftemp_bg->Integral(xleft,xright)/h->GetBinWidth(1);
  double total_fit_integral = sig_fit_integral + bg_fit_integral;
  // Calculate and store purity
  u0 = sig_fit_integral/total_hist_sum;
  u1 = sig_fit_integral/total_fit_integral;
  u2 = 1-bg_fit_integral/total_hist_sum;
  u3 = 1-bg_fit_integral/total_fit_integral;
  double uarr[4] = {u0,u1,u2,u3};
  cout << "Sideband Method --- Purities calculated " << endl;
  cout << "u0 = " << u0 << endl;
  cout << "u1 = " << u1 << endl;
  cout << "u2 = " << u2 << endl;
  cout << "u3 = " << u3 << endl;
  cout << "Sideband Method --- Beginning binned asymmetry fit " << endl;

  // Default to binned 2D asymmetry fit
  // Create 4 histograms for binning
  cout << "Sideband Method --- Creating 4, 2D histograms " << endl;
  TH2F * h_sigbg_plus = (TH2F*)_sideband.hist_asym.h2->Clone(); h_sigbg_plus->SetName("h_sigbg_plus");
  TH2F * h_sigbg_minus = (TH2F*)_sideband.hist_asym.h2->Clone(); h_sigbg_minus->SetName("h_sigbg_minus");
  TH2F * h_bg_plus = (TH2F*)_sideband.hist_asym.h2->Clone(); h_bg_plus->SetName("h_bg_plus");
  TH2F * h_bg_minus = (TH2F*)_sideband.hist_asym.h2->Clone(); h_bg_minus->SetName("h_bg_minus");
  // Fill each histogram from TTree
  cout << "Sideband Method --- Drawing into TH2F from TTree " << endl;
  cout << "Sideband Method --- Drawing (0/4) " << endl;
  _tree->Draw(Form("%s:%s>>h_sigbg_plus",_sideband.hist_asym.paramY.c_str(), _sideband.hist_asym.paramX.c_str()),Form("%s > %f && %s < %f && helicity == 1",
														      _sideband.hist.paramX.c_str(),xleft,
														      _sideband.hist.paramX.c_str(),xright),"goff");
  cout << "Sideband Method --- Drawing (1/4) " << endl;
  _tree->Draw(Form("%s:%s>>h_sigbg_minus",_sideband.hist_asym.paramY.c_str(), _sideband.hist_asym.paramX.c_str()),Form("%s > %f && %s < %f && helicity == -1",
														       _sideband.hist.paramX.c_str(),xleft,
														       _sideband.hist.paramX.c_str(),xright),"goff");
  cout << "Sideband Method --- Drawing (2/4) " << endl;
  _tree->Draw(Form("%s:%s>>h_bg_plus",_sideband.hist_asym.paramY.c_str(), _sideband.hist_asym.paramX.c_str()),Form("%s > %f && %s < %f && helicity == 1",
														   _sideband.hist.paramX.c_str(),xright,
														   _sideband.hist.paramX.c_str(),xmax),"goff");
  cout << "Sideband Method --- Drawing (3/4) " << endl;
  _tree->Draw(Form("%s:%s>>h_bg_minus",_sideband.hist_asym.paramY.c_str(), _sideband.hist_asym.paramX.c_str()),Form("%s > %f && %s < %f && helicity == -1",
														    _sideband.hist.paramX.c_str(),xright,
														    _sideband.hist.paramX.c_str(),xmax),"goff");
  cout << "Sideband Method --- Drawing (4/4) --- Complete " << endl;
  // Create two TF2's for fitting
  cout << "Sideband Method --- Cloning Asymmetry 2D fits " << endl;
  TF2 * f_sigbg = (TF2*)_sideband.asym.binnedFit2D->Clone(); f_sigbg->SetName("f_sigbg");
  TF2 * f_bg = (TF2*)_sideband.asym.binnedFit2D->Clone(); f_bg->SetName("f_bg");
  // For each purity u variable, get fit params
  cout << "Sideband Method --- Saving asymmetries for each purity calculation " << endl;
  std::vector<double> sig_params;
  std::vector<double> sig_errors;
  std::vector<double> sigbg_params;
  std::vector<double> sigbg_errors;
  std::vector<double> bg_params;
  std::vector<double> bg_errors;
  for(int i = 0 ; i < 4 ; i++){
    cout << "\tPurity u" << i << endl;
    sig_params.clear();
    sig_errors.clear();
    sigbg_params.clear();
    sigbg_errors.clear();
    bg_params.clear();
    bg_errors.clear();

    double u = uarr[i];
    TH2F * hsp1 = (TH2F*)h_sigbg_plus->Clone();
    TH2F * hsp2 = (TH2F*)h_sigbg_plus->Clone();
    TH2F * hsm = (TH2F*)h_sigbg_minus->Clone();
    TH2F * hbp1 = (TH2F*)h_bg_plus->Clone();
    TH2F * hbp2 = (TH2F*)h_bg_plus->Clone();
    TH2F * hbm = (TH2F*)h_bg_minus->Clone();

    // Set Sumw2 for proper error propagation when dividing
    hsp1->Sumw2();
    hsp2->Sumw2();
    hsm->Sumw2();
    hbp1->Sumw2();
    hbp2->Sumw2();
    hbm->Sumw2();

    // For the sig+bg range ( N+ - N- ) / ( N+ + N-)
    hsp1->Add(hsm,-1);
    hsp2->Add(hsm,1);
    hsp1->Divide(hsp2);
    cout << "\tFitting Signal+Bkg Region " << endl;
    hsp1->Fit(f_sigbg,TString(_sideband.asym.fitOptions));
    
    hbp1->Add(hbm,-1);
    hbp2->Add(hbm,1);
    hbp1->Divide(hbp2);
    cout << "\tFitting Bkg Region\n" << endl;
    hbp1->Fit(f_bg,TString(_sideband.asym.fitOptions));
    
    // Extract signal only asymmetries using purity
    for(int j = 0 ; j < f_sigbg->GetNpar() ; j++){
      double sigbg_param = f_sigbg->GetParameter(j);
      double sigbg_error = f_sigbg->GetParError(j);
      double bg_param = f_bg->GetParameter(j);
      double bg_error = f_bg->GetParError(j);
      
      double sig_param = (1.0/u)*sigbg_param - ((1.0-u)/u)*bg_param;
      double sig_error = sqrt( pow(sigbg_error/u,2) + pow(bg_error*(1.0-u)/u,2) );
      sig_params.push_back(sig_param);
      sig_errors.push_back(sig_error);
      sigbg_params.push_back(sigbg_param);
      sigbg_errors.push_back(sigbg_error);
      bg_params.push_back(bg_param);
      bg_errors.push_back(bg_error);
    }
   
    if(i==0){
      sig_params_u0 = sig_params;
      sigbg_params_u0 = sigbg_params;
      bg_params_u0 = bg_params;
      sig_errors_u0 = sig_errors;
      sigbg_errors_u0 = sigbg_errors;
      bg_errors_u0 = bg_errors;
    }
    else if(i==1){
      sig_params_u1 = sig_params;
      sigbg_params_u1 = sigbg_params;
      bg_params_u1 = bg_params;
      sig_errors_u1 = sig_errors;
      sigbg_errors_u1 = sigbg_errors;
      bg_errors_u1 = bg_errors;
    }
    else if(i==2){
      sig_params_u2 = sig_params;
      sigbg_params_u2 = sigbg_params;
      bg_params_u2 = bg_params;
      sig_errors_u2 = sig_errors;
      sigbg_errors_u2 = sigbg_errors;
      bg_errors_u2 = bg_errors;
    }
    else if(i==3){
      sig_params_u3 = sig_params;
      sigbg_params_u3 = sigbg_params;
      bg_params_u3 = bg_params;
      sig_errors_u3 = sig_errors;
      sigbg_errors_u3 = sigbg_errors;
      bg_errors_u3 = bg_errors;
    } 
  }
  cout << "Sideband Method --- Writing to TFile " << endl;
  fOut->cd();
  // Write asymmetry parameters to TTree to TFile
  tOut->Fill();
  tOut->Write();
  // Write TF1 to TFile
  f->Write();
  // Write TF2's to TFile
  f_sigbg->Write();
  f_bg->Write();
  // Write TH1F to TFile
  h->Write();
  // Close TFile
  fOut->Close();
  return 0;  
}

int Fitter::executeSplot(){
  cout << "\n\n\nExecuting sPlots Method\n\n\n"<<endl;
  // Extract sWeights from distribution
  cout << "sWeights Method --- Extracting sWeights from distribution" << endl;
  extractSWeights();
  // Open DataWeightedTree.root (contains Signal and Bkg weights)
  cout << "sWeights Method --- Opening DataWeightedTree.root" << endl;
  TFile *fOut = new TFile(Form("%s/splot_%s/DataWeightedTree.root",_filepath.c_str(),_treeName.c_str()),"UPDATE");
  _tree = (TTree*)fOut->Get(_treeName.c_str());
  TTree *tOut = new TTree("tree","Splot Method Related Variables");
  std::vector<double> sig_params;
  std::vector<double> sig_errors;
  std::vector<double> sigbg_params;
  std::vector<double> sigbg_errors;
  std::vector<double> bg_params;
  std::vector<double> bg_errors;
  tOut->Branch("A_sig",&sig_params);
  tOut->Branch("A_sig_errors",&sig_errors);
  tOut->Branch("A_sigbg",&sigbg_params);
  tOut->Branch("A_sigbg_errors",&sigbg_errors);
  tOut->Branch("A_bg",&bg_params);
  tOut->Branch("A_bg_errors",&bg_errors);
  
  // Create 6 histograms for binning
  cout << "sWeights Method --- Creating helicity +1 and helicity -1 histograms (sig/sigbg/bg)" << endl;
  TH2F * h_sig_plus = (TH2F*)_splot.hist_asym.h2->Clone(); h_sig_plus->SetName("h_sig_plus");
  TH2F * h_sig_minus = (TH2F*)_splot.hist_asym.h2->Clone(); h_sig_minus->SetName("h_sig_minus");
  TH2F * h_sigbg_plus = (TH2F*)_splot.hist_asym.h2->Clone(); h_sigbg_plus->SetName("h_sigbg_plus");
  TH2F * h_sigbg_minus = (TH2F*)_splot.hist_asym.h2->Clone(); h_sigbg_minus->SetName("h_sigbg_minus");
  TH2F * h_bg_plus = (TH2F*)_splot.hist_asym.h2->Clone(); h_bg_plus->SetName("h_bg_plus");
  TH2F * h_bg_minus = (TH2F*)_splot.hist_asym.h2->Clone(); h_bg_minus->SetName("h_bg_minus");
  // Fill each histogram from TTree
  double xmin = _splot.hist.h1->GetBinCenter(1)-0.5*_splot.hist.h1->GetBinWidth(1);
  double xmax = _splot.hist.h1->GetBinCenter(_splot.hist.h1->GetNbinsX())+0.5*_splot.hist.h1->GetBinWidth(1);
  cout << "sWeights Method --- Drawing into TH2F from TTree " << endl;
  cout << "sWeights Method --- Drawing (0/6) " << endl;
  _tree->Draw(Form("%s:%s>>h_sig_plus",_splot.hist_asym.paramY.c_str(), _splot.hist_asym.paramX.c_str()),Form("(%s > %f && %s < %f && helicity == 1) * Signal",
					      _splot.hist.paramX.c_str(),xmin,
					      _splot.hist.paramX.c_str(),xmax), "goff");
  cout << "sWeights Method --- Drawing (1/6) " << endl;
  _tree->Draw(Form("%s:%s>>h_sig_minus",_splot.hist_asym.paramY.c_str(), _splot.hist_asym.paramX.c_str()),Form("(%s > %f && %s < %f && helicity == -1) * Signal",
					      _splot.hist.paramX.c_str(),xmin,
					      _splot.hist.paramX.c_str(),xmax), "goff");
  cout << "sWeights Method --- Drawing (2/6) " << endl;
  _tree->Draw(Form("%s:%s>>h_bg_plus",_splot.hist_asym.paramY.c_str(), _splot.hist_asym.paramX.c_str()),Form("(%s > %f && %s < %f && helicity == 1) * BG",
					      _splot.hist.paramX.c_str(),xmin,
					      _splot.hist.paramX.c_str(),xmax), "goff");
  cout << "sWeights Method --- Drawing (3/6) " << endl;
  _tree->Draw(Form("%s:%s>>h_bg_minus",_splot.hist_asym.paramY.c_str(), _splot.hist_asym.paramX.c_str()),Form("(%s > %f && %s < %f && helicity == -1) * BG",
					      _splot.hist.paramX.c_str(),xmin,
					      _splot.hist.paramX.c_str(),xmax), "goff");
  cout << "sWeights Method --- Drawing (4/6) " << endl;
  _tree->Draw(Form("%s:%s>>h_sigbg_plus",_splot.hist_asym.paramY.c_str(), _splot.hist_asym.paramX.c_str()),Form("(%s > %f && %s < %f && helicity == 1)",
					      _splot.hist.paramX.c_str(),xmin,
					      _splot.hist.paramX.c_str(),xmax), "goff");
  cout << "sWeights Method --- Drawing (5/6) " << endl;
  _tree->Draw(Form("%s:%s>>h_sigbg_minus",_splot.hist_asym.paramY.c_str(), _splot.hist_asym.paramX.c_str()),Form("(%s > %f && %s < %f && helicity == -1)",
					      _splot.hist.paramX.c_str(),xmin,
					      _splot.hist.paramX.c_str(),xmax), "goff");
  cout << "sWeights Method --- Drawing (6/6) --- Complete" << endl;
  // Clone histograms for (N+ - N-)/(N+ + N-)  
  cout << "sWeights Method --- Cloning drawn TH2F's " << endl;
  TH2F * hsp1 = (TH2F*)h_sig_plus->Clone();
  TH2F * hsp2 = (TH2F*)h_sig_plus->Clone();
  TH2F * hsm = (TH2F*)h_sig_minus->Clone();

  TH2F * hsbp1 = (TH2F*)h_sigbg_plus->Clone();
  TH2F * hsbp2 = (TH2F*)h_sigbg_plus->Clone();
  TH2F * hsbm = (TH2F*)h_sigbg_minus->Clone();

  TH2F * hbp1 = (TH2F*)h_bg_plus->Clone();
  TH2F * hbp2 = (TH2F*)h_bg_plus->Clone();
  TH2F * hbm = (TH2F*)h_bg_minus->Clone();
  
  // Set Sumw2() for proper error propagation when dividing
  hsp1->Sumw2();
  hsp2->Sumw2();
  hsm->Sumw2();

  hsbp1->Sumw2();
  hsbp2->Sumw2();
  hsbm->Sumw2();

  hbp1->Sumw2();
  hbp2->Sumw2();
  hbm->Sumw2();
  // Produce asymmetry histograms 
  cout << "sWeights Method --- Producing asymmetry histograms " << endl;
  hsp1->Add(hsm,-1);
  hsp2->Add(hsm,1);
  hsp1->Divide(hsp2); TH2F *h_sig = (TH2F*)hsp1->Clone();

  hsbp1->Add(hsbm,-1);
  hsbp2->Add(hsbm,1);
  hsbp1->Divide(hsbp2); TH2F *h_sigbg = (TH2F*)hsbp1->Clone();

  hbp1->Add(hbm,-1);
  hbp2->Add(hbm,1);
  hbp1->Divide(hbp2); TH2F *h_bg = (TH2F*)hbp1->Clone();
  // Create three TF2's for fitting
  cout << "sWeights Method --- Cloning Asymmetry 2D fits " << endl;
  TF2 * f_sig = (TF2*)_splot.asym.binnedFit2D->Clone(); f_sig->SetName("f_sig");
  TF2 * f_sigbg = (TF2*)_splot.asym.binnedFit2D->Clone(); f_sigbg->SetName("f_sigbg");
  TF2 * f_bg = (TF2*)_splot.asym.binnedFit2D->Clone(); f_bg->SetName("f_bg");
  // Fit asymmetry histograms to sinusoidal modulations
  cout << "sWeights Method --- Fitting asymmetry modulations " << endl;
  h_sig->Fit(f_sig,_splot.asym.fitOptions.c_str());
  h_sigbg->Fit(f_sigbg,_splot.asym.fitOptions.c_str());
  h_bg->Fit(f_bg,_splot.asym.fitOptions.c_str());
  for(int j = 0 ; j < f_sig->GetNpar() ; j++){
    sig_params.push_back(f_sig->GetParameter(j));
    sig_errors.push_back(f_sig->GetParError(j));
    sigbg_params.push_back(f_sigbg->GetParameter(j));
    sigbg_errors.push_back(f_sigbg->GetParError(j));
    bg_params.push_back(f_bg->GetParameter(j));
    bg_errors.push_back(f_bg->GetParError(j));
  }
  cout << "sWeights Method --- Writing results to TFile " << endl;
  fOut->cd();
  // Save TTree to TFile
  tOut->Fill();
  tOut->Write();
  // Save TF2's to TFile
  f_sig->Write();
  f_sigbg->Write();
  f_bg->Write();
  // Delete large TTree from file
  fOut->Delete(_treeName.c_str());
  fOut->Close();
  return 0;
}
int Fitter::extractSWeights(){
  std::string outDir = Form("%s/splot_%s",_filepath.c_str(),_treeName.c_str());
  HS::FIT::sPlot RF;
  RF.SetUp().SetOutDir(outDir.c_str());
  //////////////////////////////////// Load Variables
  RF.SetUp().LoadVariable(_splot.variable);
  RF.SetUp().SetIDBranchName(_splot.idbranch);
  //////////////////////////////////// Make Signal
  RF.SetUp().FactoryPDF(_splot.signalFactoryPDF);
  RF.SetUp().LoadSpeciesPDF("Signal",1);
  //////////////////////////////////// Make BG
  RF.SetUp().FactoryPDF(_splot.bgFactoryPDF);
  RF.SetUp().LoadSpeciesPDF("BG",1);
  //////////////////////////////////// Load Data
  RF.LoadData(_treeName.c_str(),Form("%s/%s.root",_filepath.c_str(),_rootname.c_str()));
  HS::FIT::PROCESS::Here::Go(&RF);
  RF.saveWeightedTree(Form("%s/DataWeightedTree.root",outDir.c_str()));
  gSystem->Exec(Form("rm %s/Tweights.root",outDir.c_str()));
  gSystem->Exec(Form("rm %s/Weights.root",outDir.c_str()));
  gSystem->Exec(Form("rm %s/ResultsHSMinuit2.root",outDir.c_str()));
  return 0;
}

int Fitter::perform1DBinnedFits(fitObject fitobj, histObject histobj){
  // Enter into TFile
  _fIn->cd();

  // Draw into histogram from TTree
  cout << "Fitter::perform1DBinnedFits --- Filling 1D " << histobj.paramX.c_str() << " histogram" << endl;
  _tree->Draw(Form("%s>>%s",histobj.paramX.c_str(),histobj.h1->GetName()),histobj.drawCut.c_str(),"goff");
  
  // If the histogram drops to 0 within xmin - xmax, adjust xmax
  double xmin, xmax;
  fitobj.binnedFit1D->GetRange(xmin,xmax);
  for(int i = 1; i <= histobj.h1->GetNbinsX() ; i++){
    double bin_center = histobj.h1->GetBinCenter(i);
    double bin_count = histobj.h1->GetBinContent(i);
    if(bin_center < xmin || bin_center > xmax)
      continue;
    else if(bin_count==0){
      fitobj.binnedFit1D->SetRange(xmin,bin_center);
      break;   
    }    
  }
  histobj.h1->Scale(1/histobj.h1->Integral());
  // Fit the histogram
  histobj.h1->Fit(fitobj.binnedFit1D, TString(fitobj.fitOptions));
  return 0;
}

int Fitter::perform2DBinnedFits(fitObject fitobj, histObject histobj){
  // Enter into TFile
  _fIn->cd();

  // Draw into histogram from TTree
  cout << "Fitter::perform2DBinnedFits --- Filling 2D " << histobj.paramY.c_str() << " vs. " << histobj.paramX.c_str() << " histogram" << endl;
  _tree->Draw(Form("%s:%s>>%s",histobj.paramY.c_str(),histobj.paramX.c_str(),histobj.h2->GetName()),histobj.drawCut.c_str(),"goff");
    
  // Fit the histogram
  histobj.h2->Fit(fitobj.binnedFit2D, TString(fitobj.fitOptions));
  return 0;
}
 

int Fitter::End(){
  _fIn->Close();
  return 0;
}
