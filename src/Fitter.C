#include "Fitter.h"

using namespace std;

Fitter::Fitter(std::string filepath, std::string rootname, std::string treeName){
  _filepath = filepath;
  _rootname = rootname;
  _treeName = treeName;
  _fIn = new TFile(Form("%s/%s.root",filepath.c_str(),rootname.c_str()),"READ");
  _tree = (TTree*)_fIn->Get(_treeName);
}

void Fitter::setSideband(sidebandObject obj){
  _sideband = obj;
}

void Fitter::setSplot(sPlotObject obj){
  _splot = obj;
}

int Fitter::executeSideband(histObject histobj){
  
  
  cout << "\n\n\nExecuting Sideband Method\n\n\n"<<endl;
  // Create new TFile based on TTree name
  gProcessLine(Form("mkdir %s/sideband_%s",_filepath.c_str(),_treeName.c_str()));
  TFile *fOut = new TFile(Form("%s/sideband_%s/sidebandMethod.root",_filepath.c_str(),_treeName.c_str()),"RECREATE");
  TTree *tOut = new TTree("tree","Sideband Method Related Variables");
  std::vector<double> sig_params_u0;
  std::vector<double> sig_errors_u0;
  std::vector<double> sig_params_u1;
  std::vector<double> sig_errors_u1;
  std::vector<double> sig_params_u2;
  std::vector<double> sig_errors_u2;
  std::vector<double> sig_params_u3;
  std::vector<double> sig_errors_u3;
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
  // Perform binned fits in signal and sideband region
  fitObject signal = _sideband.signal;
  fitObject sideband = _sideband.sideband;
  cout << "Sideband Method --- Fitting Signal Region"<<endl;
  perform1DBinnedFits(signal,histobj);
  cout << "Sideband Method --- Fitting Sideband Region"<<endl;
  perform1DBinnedFits(sideband,histobj);
  // Get signal and sideband fits
  TF1 * f_peak = signal.binnedFit1D->Clone();
  TF1 * f_sideband = sideband.binnedFit1D->Clone();
  // Get histogram
  TH1F * h = histobj.h1->Clone();
  // Get +/- 2 StdDev from Gaussian peak
  // Below code may need to be re-written if signal is not fit to gaussian
  double xmin = f_peak->GetParameter(1)-2*f_peak->GetParameter(2);
  double xmax = f_peak->GetParameter(1)+2*f_peak->GetParameter(2);
  cout << "Sideband Method --- +/- 2 StdDev purity calculations [" << xmin << "," << xmax << "]" <<endl;
  // Get integral parameters for purity calculations
  double total_hist_sum = h->Integral(h->FindBin(xmin), h->FindBin(xmax));
  double sig_fit_integral = f_peak->Integral(xmin,xmax)/h->GetBinWidth(1);
  double bg_fit_integral = f_sideband->Integral(xmin,xmax)/h->GetBinWidth(1);
  double total_fit_integral = sig_fit_integral + bg_fit_integral;
  // Calculate and store purity
  double u0 = sig_fit_integral/total_hist_sum;
  double u1 = sig_fit_integral/total_fit_integral;
  double u2 = 1-bg_fit_integral/total_hist_sum;
  double u3 = 1-bg_fit_integral/total_fit_integral;
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
  TH2F * h_sigbg_minus = (TH2F*)_sideband.hist_asym.h2->Clone(); h_bg_minus->SetName("h_sigbg_minus");
  TH2F * h_bg_plus = (TH2F*)_sideband.hist_asym.h2->Clone(); h_sigbg_plus->SetName("h_bg_plus");
  TH2F * h_bg_minus = (TH2F*)_sideband.hist_asym.h2->Clone(); h_bg_minus->SetName("h_bg_minus");
  // Fill each histogram from TTree
  cout << "Sideband Method --- Drawing into TH2F from TTree " << endl;
  cout << "Sideband Method --- Drawing (0/4) " << endl;
  _tree->Draw("phi_R0:phi_h>>h_sigbg_plus",Form("%s > %f && %s < %f && helicity == 1",
				       histobj.paramX.c_str(),f_peak->GetXmin(),
				       histobj.paramX.c_str(),f_peak->GetXmax()),"goff");
  cout << "Sideband Method --- Drawing (1/4) " << endl;
  _tree->Draw("phi_R0:phi_h>>h_sigbg_minus",Form("%s > %f && %s < %f && helicity == -1",
				       histobj.paramX.c_str(),f_peak->GetXmin(),
				       histobj.paramX.c_str(),f_peak->GetXmax()),"goff");
  cout << "Sideband Method --- Drawing (2/4) " << endl;
  _tree->Draw("phi_R0:phi_h>>h_bg_plus",Form("%s > %f && %s < %f && helicity == 1",
 				       histobj.paramX.c_str(),f_sideband->GetXmin(),
				       histobj.paramX.c_str(),f_sideband->GetXmax()),"goff");
  cout << "Sideband Method --- Drawing (3/4) " << endl;
  _tree->Draw("phi_R0:phi_h>>h_bg_minus",Form("%s > %f && %s < %f && helicity == -1",
				       histobj.paramX.c_str(),f_sideband->GetXmin(),
				       histobj.paramX.c_str(),f_sideband->GetXmax()),"goff");
  cout << "Sideband Method --- Drawing (4/4) --- Complete " << endl;
  // Create two TF2's for fitting
  cout << "Sideband Method --- Cloning Asymmetry 2D fits " << endl;
  TF2 * f_sigbg = (TF2*)_sideband.asym.binnedFit2D->Clone(); f_sigbg->SetName("f_sigbg");
  TF2 * f_bg = (TF2*)_sideband.asym.binnedFit2D->Clone(); f_bg->SetName("f_bg");
  // For each purity u variable, get fit params
  cout << "Sideband Method --- Saving asymmetries for each purity calculation " << endl;
  for(int i = 0 ; i < 4 ; i++){
    cout << "\tPurity u" << j << endl;
    sig_params.clear();
    sig_errors.clear();

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
    hsp1->Fit(f_sigbg,_sideband.asym.fitOptions);
    
    hbp1->Add(hbm,-1);
    hbp2->Add(hbm,1);
    hbp1->Divide(hbp2);
    cout << "\tFitting Bkg Region\n" << endl;
    hbp1->Fit(f_bg,_sideband.asym.fitOptions);
    
    // Extract signal only asymmetries using purity
    for(int j = 0 ; j < f_sigbg->GetNpar() ; j++){
      double sigbg_param = f_sigbg->GetParameter(j);
      double sigbg_error = f_sigbg->GetParError(j);
      double bg_param = f_bg->GetParameter(j);
      double bg_error = f_bg->GetParError(j);
      
      double sig_param = (1.0/u)*sigbg_param - ((1.0-u)/u)*bg_param;
      double sig_error = sqrt( pow(sigbg_error/u,2) + pow(bg_error*(1.0-u)/u,2) );
      
      sig_params.push_back(sig_param);
      sig_error.push_back(sig_error);
    }
   
    if(j==0)
      _sideband.asym_params_u0 = sig_params;
    else if(j==1)
      _sideband.asym_params_u1 = sig_params;
    else if(j==2)
      _sideband.asym_params_u2 = sig_params;
    else if(j==3)
      _sideband.asym_params_u3 = sig_params;
  }
  
  
  return 0;  
}

int Fitter::extractSWeights(sPlotObject splot){
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
  RF.LoadData(_treeName.c_str(),_filepath.c_str());
  HS::FIT::PROCESS::Here::Go(&RF);
  RF.saveWeightedTree(Form("%s/DataWeightedTree.root",outDir.c_str()));
  return 0;
}

int Fitter::appendSidebandInfo(){
  
}

int Fitter::perform1DBinnedFits(fitObject fitobj, histObject histobj){
  // Enter into TFile
  _fIn->cd();

  // Draw into histogram from TTree
  _tree->Draw(Form("%s>>%s",histobj.paramX.c_str(),histobj.h1->GetName()),histobj.drawCut,"goff");
    
  // Fit the histogram
  histobj.h1->Fit(fitobj.binnedFit1D, TString(fitobj.fitOptions));
  return 0;
}

int Fitter::perform2DBinnedFits(fitObject fitobj, histObject histobj){
  // Enter into TFile
  _fIn->cd();

  // Draw into histogram from TTree
  _tree->Draw(Form("%s:%s>>%s",histobj.paramY.c_str(),histobj.paramX.c_str(),histobj.h2->GetName()),histobj.drawCut,"goff");
    
  // Fit the histogram
  histobj.h2->Fit(fitobj.binnedFit2D, TString(fitobj.fitOptions));
  return 0;
}
 

int Fitter::End(){
  _fIn->Close();
  return 0;
}
