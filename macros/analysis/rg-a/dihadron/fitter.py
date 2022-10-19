import ROOT
import os
import numpy as np
import pandas as pd
from binner import Bin

class Fit:
    def __init__(self,name,infile,intree):
        self.name=name
        self.infile=infile
        self.intree=intree
        if(not os.path.exists(infile)):
            print("ERROR: Input file",infile,"for Fit does not exist...Aborting...")
            return -1
        self.tfile=ROOT.TFile(infile,"READ")
        if(not self.tfile.Get(intree)):
            print("ERROR: TTree",intree,"does not exist within",infile,"...Aborting...")
            return -1
        self.ttree=self.tfile.Get(intree)
        
    def perform2dFit(self):
        pass

class sdbnd(Fit):
    def __init__(self,infile,intree,verbosity=0):
        Fit.__init__(self,"sideband",infile,intree)
        self.mods="[0]*sin(2*x-2*y)+[1]*sin(x-y)+[2]*sin(-x+2*y)+[3]*sin(y)+[4]*sin(x)+[5]*sin(2*x-y)+[6]*sin(3*x-2*y)"
        self.nmods=7
        self.latexmods=["sin(2#phi_{h}-2#phi_{R})","sin(#phi_{h}-#phi_{R})", "sin(-#phi_{h}+2#phi_{R})" , "sin(#phi_{R})", "sin(#phi_{h})", "sin(2#phi_{h}-#phi{R})" , "sin(3#phi_{h}-2#phi_{R})"]
        self.v=verbosity
        # Defining bounds for signal/sideband
        # -------EDIT BELOW----------#
        self.signalBounds=[0.108,0.160]
        self.sidebandBounds=[0.170,0.4]
        # ---------------------------#
        
        
        # Defining fit function
        # -------EDIT BELOW----------#
        self.fitname="f_diphoton"
        self.fitbounds=[0.08,0.4]
        self.fitopts="SR Q0"
        self.fitfunc_sigbg="gaus"
        self.fitparams_sigbg=[1,0.13,0.01] # Starting values
        self.fitparamlimits_sigbg=[0,[0.128,0.135],[0.001,0.06]] # Ranges (0-->No range)
        self.fitfunc_bg="pol4"
        self.fitparams_bg=[1,1,1,1,1]
        self.fitparamlimits_bg=[0,0,0,0,0]
        # ---------------------------#
        self.fitfunc=self.fitfunc_sigbg+"(0)"+"+"+self.fitfunc_bg+"({})".format(len(self.fitparams_sigbg))
        self.fitparams=self.fitparams_sigbg+self.fitparams_bg
        self.fiterrors=[0,0,0,0,0,0,0,0]
        self.diphotonPeak=-999
        self.diphotonSpread=-999
        self.diphotonchi2=0
        self.diphotonndf=0
        self.fitparamlimits=self.fitparamlimits_sigbg+self.fitparamlimits_bg
        self.setTF1()
        
        
        # Defining histogram for Mdiphoton plotting
        # -------EDIT BELOW----------#
        self.histbounds=self.fitbounds
        self.histname="h1_sbnd"
        self.histbins=100
        # ---------------------------#
        self.setTH1()
        
        # Defining histograms for azimuthal modulation binning
        # -------EDIT BELOW----------#
        self.str_phih="phi_h"
        self.str_phiR="phi_R0" # HERMES 0803.2367
        self.bins_phih=8
        self.bins_phiR=8
        # ---------------------------#
        self.th2_sigbg_plus=0
        self.th2_bg_plus=0
        self.th2_sigbg_minus=0
        self.th2_bg_minus=0
        self.uarr=[0,0,0,0]
        
        self.sigbg_params_u0 = [0]*self.nmods
        self.sigbg_errors_u0 = [0]*self.nmods
        self.sig_params_u0 = [0]*self.nmods
        self.sig_errors_u0 = [0]*self.nmods
        self.bg_params_u0 = [0]*self.nmods
        self.bg_errors_u0 = [0]*self.nmods
        
        self.sigbg_params_u1 = [0]*self.nmods
        self.sigbg_errors_u1 = [0]*self.nmods
        self.sig_params_u1 = [0]*self.nmods
        self.sig_errors_u1 = [0]*self.nmods
        self.bg_params_u1 = [0]*self.nmods
        self.bg_errors_u1 = [0]*self.nmods
        
        self.sigbg_params_u2 = [0]*self.nmods
        self.sigbg_errors_u2 = [0]*self.nmods
        self.sig_params_u2 = [0]*self.nmods
        self.sig_errors_u2 = [0]*self.nmods
        self.bg_params_u2 = [0]*self.nmods
        self.bg_errors_u2 = [0]*self.nmods
        
        self.sigbg_params_u3 = [0]*self.nmods
        self.sigbg_errors_u3 = [0]*self.nmods
        self.sig_params_u3 = [0]*self.nmods
        self.sig_errors_u3 = [0]*self.nmods
        self.bg_params_u3 = [0]*self.nmods
        self.bg_errors_u3 = [0]*self.nmods
        
    def setTF1(self):
        self.tf1=ROOT.TF1( self.fitname , self.fitfunc , self.fitbounds[0] , self.fitbounds[1] )
        for i,p,l in zip(range(len(self.fitparams)),self.fitparams,self.fitparamlimits):
            self.tf1.SetParameter(i,p)
            if(l!=0):
                self.tf1.SetParLimits(i,l[0],l[1])
    
    def setTH1(self):
        self.th1=ROOT.TH1F( self.histname , "" , self.histbins , self.histbounds[0] , self.histbounds[1])
    
    def doDiphotonFit(self):
        boundCut="Mdiphoton>{} && Mdiphoton<{}".format(self.histbounds[0],self.histbounds[1])
        self.ttree.Draw("Mdiphoton>>{}".format(self.histname),boundCut,"goff")
        # If the histogram drops to zero, alter the fitting range
        [xmin,xmax]=self.fitbounds
        for i in np.arange(1,1+self.th1.GetNbinsX(),1):
            i=int(i)
            bincenter=self.th1.GetBinCenter(i)
            bincount=self.th1.GetBinContent(i)
            if(bincenter<xmin or bincenter>xmax):
                continue
            if(bincount==0):
                self.fitbounds[1]=bincenter
                break
        # Test if too few entries
        if(self.th1.Integral()<10):
            if(self.v>0):
                print("WARNING: TH1 empty...skipping",end=" ")
            return -1
        # Scale the histogram
        self.th1.Scale(1/self.th1.Integral())
        # Perform the fit
        self.th1.Fit(self.tf1,self.fitopts)
        # Store fit params
        self.diphotonchi2=self.tf1.GetChisquare()
        self.diphotonndf=self.tf1.GetNDF()
        for i in range(len(self.fitparams)):
            p = self.tf1.GetParameter(i)
            e = self.tf1.GetParError(i)
            self.fitparams[i]=p
            self.fiterrors[i]=e
            if(i<len(self.fitparams_sigbg)):
                self.fitparams_sigbg[i]=p
            else:
                self.fitparams_bg[i-len(self.fitparams_sigbg)]=p
    # Using the 1d fitted diphoton peak, calculate the 2 sigma bounds
    def evaluatePeak(self):
        self.diphotonPeak=self.tf1.GetParameter(1)
        self.diphotonSpread=self.tf1.GetParameter(2)
        self.diphotonxleft=self.diphotonPeak-2*self.diphotonSpread
        self.diphotonxright=self.diphotonPeak+2*self.diphotonSpread
        self.diphotonxmax=self.fitbounds[1]
    
    # Evaluate purities (see RGA analysis note)
    def evaluatePurities(self):
        
        # Set fit functions for around the pi0 peak
        self.ftemp_sigbg=ROOT.TF1("ftemp_sigbg",self.fitfunc_sigbg,self.diphotonxleft,self.diphotonxright)
        self.ftemp_bg=ROOT.TF1("ftemp_bg",self.fitfunc_bg,self.diphotonxleft,self.diphotonxright)
        for i,p in zip(range(len(self.fitparams_sigbg)),self.fitparams_sigbg):
            self.ftemp_sigbg.SetParameter(i,p)
        for j,p in zip(range(len(self.fitparams_bg)),self.fitparams_bg):
            self.ftemp_bg.SetParameter(j,p)
        
        # Parameters for purity calculations
        total_hist_sum = self.th1.Integral(self.th1.FindBin(self.diphotonxleft),
                                           self.th1.FindBin(self.diphotonxright))
        sig_fit_integral = self.ftemp_sigbg.Integral(self.diphotonxleft,self.diphotonxright)/self.th1.GetBinWidth(1)
        bg_fit_integral = self.ftemp_bg.Integral(self.diphotonxleft,self.diphotonxright)/self.th1.GetBinWidth(1)
        total_fit_integral = sig_fit_integral + bg_fit_integral
        if(total_hist_sum==0):
            if(self.v>0):
                print("WARNING: total_hist_sum=0...skipping...",end=" ")
            return -1
        if(total_fit_integral==0):
            if(self.v>0):
                print("WARNING: total_fit_integral=0...skipping...",end=" ")
            return -1
        self.u0 = sig_fit_integral/total_hist_sum
        self.u1 = sig_fit_integral/total_fit_integral
        self.u2 = 1-bg_fit_integral/total_hist_sum
        self.u3 = 1-bg_fit_integral/total_fit_integral
        self.uarr = [self.u0 , self.u1 , self.u2 , self.u3]
        
        
    def constructTH2s(self):
        self.th2_sigbg_plus = ROOT.TH2F("h2_sigbg_plus",r";$\phi_{h}$;$\phi_{R}$",self.bins_phih,-np.pi,np.pi,self.bins_phiR,-np.pi,np.pi)
        self.th2_bg_plus = ROOT.TH2F("h2_bg_plus",r";$\phi_{h}$;$\phi_{R}$",self.bins_phih,-np.pi,np.pi,self.bins_phiR,-np.pi,np.pi)
        self.th2_sigbg_minus = ROOT.TH2F("h2_sigbg_minus",r";$\phi_{h}$;$\phi_{R}$",self.bins_phih,-np.pi,np.pi,self.bins_phiR,-np.pi,np.pi)
        self.th2_bg_minus = ROOT.TH2F("h2_bg_minus",r";$\phi_{h}$;$\phi_{R}$",self.bins_phih,-np.pi,np.pi,self.bins_phiR,-np.pi,np.pi)
        
    def drawTH2s(self):
        self.ttree.Draw("{}:{}>>h2_sigbg_plus".format(self.str_phiR,self.str_phih),
                        "Mdiphoton>{} && Mdiphoton<{} && helicity == 1".format(self.diphotonxleft,self.diphotonxright),"goff")
        self.ttree.Draw("{}:{}>>h2_sigbg_minus".format(self.str_phiR,self.str_phih),
                        "Mdiphoton>{} && Mdiphoton<{} && helicity == -1".format(self.diphotonxleft,self.diphotonxright),"goff")
        self.ttree.Draw("{}:{}>>h2_bg_plus".format(self.str_phiR,self.str_phih),
                        "Mdiphoton>{} && Mdiphoton<{} && helicity == 1".format(self.diphotonxright,self.diphotonxmax),"goff")
        self.ttree.Draw("{}:{}>>h2_bg_minus".format(self.str_phiR,self.str_phih),
                        "Mdiphoton>{} && Mdiphoton<{} && helicity == -1".format(self.diphotonxright,self.diphotonxmax),"goff")
    
    def asymTH2s(self):
        
        # Return -1 if there are not enough events
        if(self.th2_sigbg_plus.Integral()==0 or
           self.th2_sigbg_minus.Integral()==0 or
           self.th2_bg_plus.Integral()==0 or
           self.th2_bg_minus.Integral()==0):
            if(self.v>0):
                print("WARNING: PhiH PhiR hist is empty...Skipping...",end=" ")
            return -1
        
        hnumerator_sigbg = self.th2_sigbg_plus.Clone()
        hdenominator_sigbg = self.th2_sigbg_plus.Clone()
        hnumerator_sigbg.Sumw2()
        hdenominator_sigbg.Sumw2()
        
        hnumerator_sigbg.Add(self.th2_sigbg_minus,-1)
        hdenominator_sigbg.Add(self.th2_sigbg_minus,1)
        
        self.h_asym_sigbg = hnumerator_sigbg.Clone()
        self.h_asym_sigbg.Divide(hdenominator_sigbg)
        
        hnumerator_bg = self.th2_bg_plus.Clone()
        hdenominator_bg = self.th2_bg_plus.Clone()
        hnumerator_bg.Sumw2()
        hdenominator_bg.Sumw2()
        
        hnumerator_bg.Add(self.th2_bg_minus,-1)
        hdenominator_bg.Add(self.th2_bg_minus,1)
        
        self.h_asym_bg = hnumerator_bg.Clone()
        self.h_asym_bg.Divide(hdenominator_bg)
        
    def constructTF2s(self):
        self.fmod_sigbg = ROOT.TF2("f_sigbg",self.mods,-np.pi,np.pi,-np.pi,np.pi)
        self.fmod_bg = ROOT.TF2("f_bg",self.mods,-np.pi,np.pi,-np.pi,np.pi)
        
    def doModulationFits(self):
        self.h_asym_sigbg.Fit(self.fmod_sigbg,self.fitopts)
        self.h_asym_bg.Fit(self.fmod_bg,self.fitopts)
    
    def perform2dFit(self):
        
        # Perform the diphoton fit in 1d using self.th1
        if(self.doDiphotonFit()==-1):
            return -1
        
        # Using the peak and spread, extract 2 sigma bounds
        self.evaluatePeak()
        
        # Get purities based on 4 methods
        if(self.evaluatePurities()==-1):
            return -1
        
        # Create phi_h and phi_R 2d histograms
        self.constructTH2s()
        
        # Drawing into phi_h and phi_R 2d histograms
        self.drawTH2s()
        
        # Create (N^{+}-N^{-})/(N^{+}+N^{-}) histograms
        if(self.asymTH2s()==-1):
            return -1

        
        # Create modulation 2d fit functions
        self.constructTF2s()
        
        # Fit the 2d modulations
        self.doModulationFits()
        
        # For each purity u, and for each fit parameter, extract signal only value with error
        self.sigbg_params_u0 = []
        self.sigbg_errors_u0 = []
        self.sig_params_u0 = []
        self.sig_errors_u0 = []
        self.bg_params_u0 = []
        self.bg_errors_u0 = []
        
        self.sigbg_params_u1 = []
        self.sigbg_errors_u1 = []
        self.sig_params_u1 = []
        self.sig_errors_u1 = []
        self.bg_params_u1 = []
        self.bg_errors_u1 = []
        
        self.sigbg_params_u2 = []
        self.sigbg_errors_u2 = []
        self.sig_params_u2 = []
        self.sig_errors_u2 = []
        self.bg_params_u2 = []
        self.bg_errors_u2 = []
        
        self.sigbg_params_u3 = []
        self.sigbg_errors_u3 = []
        self.sig_params_u3 = []
        self.sig_errors_u3 = []
        self.bg_params_u3 = []
        self.bg_errors_u3 = []
        
        for u,uidx in zip(self.uarr,range(len(self.uarr))):
            for i in range(self.nmods):
                sigbg_param = self.fmod_sigbg.GetParameter(i)
                sigbg_error = self.fmod_sigbg.GetParError(i)
                bg_param = self.fmod_bg.GetParameter(i)
                bg_error = self.fmod_bg.GetParError(i)
                
                sig_param = (1.0/u)*sigbg_param - ((1.0-u)/u)*bg_param
                sig_error = np.sqrt((sigbg_error/u)**2+(bg_error*(1.0-u)/u)**2)
                
                if(uidx==0):
                    self.sigbg_params_u0.append(sigbg_param)
                    self.sigbg_errors_u0.append(sigbg_error)
                    self.sig_params_u0.append(sig_param)
                    self.sig_errors_u0.append(sig_error)
                    self.bg_params_u0.append(bg_param)
                    self.bg_errors_u0.append(bg_error)
                elif(uidx==1):
                    self.sigbg_params_u1.append(sigbg_param)
                    self.sigbg_errors_u1.append(sigbg_error)
                    self.sig_params_u1.append(sig_param)
                    self.sig_errors_u1.append(sig_error)
                    self.bg_params_u1.append(bg_param)
                    self.bg_errors_u1.append(bg_error)
                elif(uidx==2):
                    self.sigbg_params_u2.append(sigbg_param)
                    self.sigbg_errors_u2.append(sigbg_error)
                    self.sig_params_u2.append(sig_param)
                    self.sig_errors_u2.append(sig_error)
                    self.bg_params_u2.append(bg_param)
                    self.bg_errors_u2.append(bg_error)
                else:
                    self.sigbg_params_u3.append(sigbg_param)
                    self.sigbg_errors_u3.append(sigbg_error)
                    self.sig_params_u3.append(sig_param)
                    self.sig_errors_u3.append(sig_error)
                    self.bg_params_u3.append(bg_param)
                    self.bg_errors_u3.append(bg_error)

class splotUnBinnedFit(Fit):
    def __init__(self,infile,intree,verbosity=0):
        Fit.__init__(self,"splotUnbinned",infile,intree)
        self.verbosity=verbosity
        # Create sPlot object
        self.splot = ROOT.sPlot()
        self.splot.SetUp().SetOutDir("/work/clas12/users/gmat/CLAS12Analysis/macros/analysis/rg-a/splot/splot_{}".format(intree))
        # sPlot parameter setup
        # ---------------------------#
        self.diphotonMin=0.08
        self.diphotonMax=self.determineDiphotonMax()
        self.splot.SetUp().LoadVariable("Mdiphoton[{},{}]".format(self.diphotonMin,self.diphotonMax));
        self.splot.SetUp().SetIDBranchName("fidmerge")
        # -------EDIT BELOW----------#
        self.splot.SetUp().FactoryPDF("Gaussian::Signal(Mdiphoton,#mu[0.131,0.129,0.136],#sigma[0.1,0.001,0.2])")
        self.splot.SetUp().LoadSpeciesPDF("Signal")
        
        self.splot.SetUp().FactoryPDF("Polynomial::BG(Mdiphoton,{a0[0.1,-10,10],a1[0.1,-10,10],a2[0.1,-10,10],a3[0.1,-10,10]})")
        self.splot.SetUp().LoadSpeciesPDF("BG",1)
        # ---------------------------#
        # LoadData into sPlot (binned TTree)
        # ---------------------------#
        self.splot.LoadData(self.intree,self.infile)
        self.splot.SetRedirectOutput()
        
        # ---------------------------#
        # Compile modulation PDF
        # ---------------------------#
        ROOT.Loader.Compile("/work/clas12/users/gmat/CLAS12Analysis/brufit/tutorials/CLAS12Analysis_test/dihadronModulationPDF.cxx");
        
        # ---------------------------#
        # Setup the fit manager
        # ---------------------------#
        self.fm = ROOT.FitManager()
        self.fm.SetUp().SetOutDir("/work/clas12/users/gmat/CLAS12Analysis/macros/analysis/rg-a/splot/obs_{}".format(intree))
        
        self.fm.SetUp().LoadVariable("phi_h[-3.1415,3.1515]"); 
        self.fm.SetUp().LoadVariable("phi_R0[-3.1415,3.1515]"); 
        self.fm.SetUp().LoadVariable("helicity[-2,2]")
        
        self.fm.SetUp().SetIDBranchName("fidmerge");
        
        self.fm.SetUp().FactoryPDF("dihadronModulationPDF::SigAsym( phi_h,phi_R0,helicity,a0[0,-1,1],a1[0,-1,1],a2[0,-1,1],a3[0,-1,1],a4[0,-1,1],a5[0,-1,1],a6[0,-1,1] )");
        self.fm.SetUp().LoadSpeciesPDF("SigAsym",1);
        
        self.fm.LoadData(self.intree,(self.infile));
    def determineDiphotonMax(self):
        df = ROOT.RDataFrame(self.intree,self.infile)
        return np.amin([df.Filter("Mdiphoton<1").Max("Mdiphoton").GetValue(),0.4])
    
    def perform2dFit(self):
        if(self.verbosity>0):
            print("splotUnBinnedFit: Performing unbinned Mdiphoton fit for",self.intree)
        ROOT.Here.Go(self.splot)
        
        if(self.verbosity>0):
            print("splotUnBinnedFit: Completed unbinned Mdiphoton fit for",self.intree)
       
        print("splotUnBinnedFit: Loading sWeights for",self.intree)
        
        self.fm.Data().LoadWeights("Signal", "/work/clas12/users/gmat/CLAS12Analysis/macros/analysis/rg-a/splot/splot_{}/Weights.root".format(self.intree));
        
        print("splotUnbinnedFit: Fitting 2d modulations for",self.intree)
        self.fm.SetRedirectOutput();
        ROOT.Here.Go(self.fm)

# Class for storing Fit Functions on the Bin structure
# Inhertits the "Bin" Class
class FitBin(Bin):
    # Initializer
    def __init__(self,infile,intree):
        Bin.__init__(self)
        self.infile=infile
        self.intree=intree
        self.isempty=False
        self.sdbnd=0
    
    # Perform a sideband fit on this specific bin
    def sidebandFit(self,verbosity):
        self.sdbnd=sdbnd(self.infile,self.intree,verbosity)
        if(self.sdbnd.perform2dFit()==-1):
            self.isempty=True
            return -1
        else:
            return 1
        
    # Perform an unbinned splot fit on this specific bin
    def splotUnBinnedFit(self,verbosity):
        self.splotunbin=splotUnBinnedFit(self.infile,self.intree,verbosity)
        if(self.splotunbin.perform2dFit()==-1):
            self.isempty=True
            return -1
        else:
            return 1

# Class which stores multiple FitBins
# Each FitBin will perform sideband fitting, splot, etc.
class Fittify():
    # Initializer
    def __init__(self,infile,verbosity=0):
        self.v = verbosity # Set verbosity
        if(not os.path.exists(infile)): # Make sure file exists
            print("ERROR: Input file",infile,"for Fittify does not exist...Aborting...")
            return -1
        self.infile=infile
        self.tfile=ROOT.TFile(infile,"READ")
        self.bins=[] # Empty FitBins array
        self.collectBins() # Collect all the FitBins from the TFile (*_binned.root)
        self.doFits() # Perform the desired fits on each bin
        self.collectParams() # Collect all the FitParameters
                             # Save into a CSV
    # Read in each of the binned TTrees within the TFile to determine bin objects
    def collectBins(self):
        for bhash in self.tfile.GetListOfKeys():
            binname=bhash.GetName()
            fbin=FitBin(self.infile,binname)
            fbin.setParamsFromName(binname)
            self.bins.append(fbin)
    
    # Fit all the bins
    def doFits(self):
        # Loop over every unique kinematic binning
        for b in self.bins:
            # Fit the sideband
            if(b.sidebandFit(self.v)==-1):
                if(self.v>0): # Error logging
                    print(b.objName)
            # Fit the splot (unbinned)
            if(b.splotUnBinnedFit(self.v)==-1):
                if(self.v>0): # Error logging
                    print(b.objName)
            return 0
    # Collect all the parameters from the bins
    # Save into a pandas dataframe
    def collectParams(self):
        # ------------------------------------
        #              Sideband
        # ------------------------------------
        #       CREATING THE DATAFRAME
        # ------------------------------------
        # Get maximum bin dimensions
        dimMax = np.amax([b.dim for b in self.bins])
        # Determine columns for dataframe
        cols=["dim","hasData"]
        # Columns for each bin dimension
        for d in range(dimMax):
            cols+=["bintype{}".format(d+1)]
            cols+=["binlower{}".format(d+1)]
            cols+=["bincenter{}".format(d+1)]
            cols+=["binupper{}".format(d+1)]
        # Columns for diphoton peak & spread values
        cols+=["diphotonPeak","diphotonSpread"]
        # Column for diphoton fit parameters
        cols+=["diphotonFitFunc","diphotonChi2","diphotonNDF"]
        for fidx in range(len(self.bins[0].sdbnd.fitparams)):
            cols+=["diphotonParam{}".format(fidx)]
            cols+=["diphotonError{}".format(fidx)]
        # Columns for the purities
        cols+=["u0","u1","u2","u3"]
        # Columns for the azimuthal fit parameters
        for fidx in range(self.bins[0].sdbnd.nmods):
            cols+=["modName{}".format(fidx)]
        for fidx in range(self.bins[0].sdbnd.nmods):
            for uidx in range(4):
                cols+=["sigbgParam{}_u{}".format(fidx,uidx)]
                cols+=["sigParam{}_u{}".format(fidx,uidx)]
                cols+=["bgParam{}_u{}".format(fidx,uidx)]
                cols+=["sigbgErrror{}_u{}".format(fidx,uidx)]
                cols+=["sigError{}_u{}".format(fidx,uidx)]
                cols+=["bgError{}_u{}".format(fidx,uidx)]
        df_sdbnd=pd.DataFrame(columns=cols)
        # ------------------------------------
        #       LOADING INTO DATAFRAME
        # ------------------------------------
        for b in self.bins: # Loop over each of the bins
            row=[]
            # Dimension of bin
            row+=[b.dim,not b.isempty]
            # Check if bin was empty
            # Bin lower and upper edges
            for d in np.arange(1,dimMax+1):
                if(d>b.dim):
                    row+=["0","0","0","0"]
                else:
                    row+=[b.names[d-1],b.bounds[d-1][0],0.5*(b.bounds[d-1][0]+b.bounds[d-1][1]),b.bounds[d-1][1]]
            # Diphoton peak and spread
            row+=[b.sdbnd.diphotonPeak,b.sdbnd.diphotonSpread]
            # Diphoton fit function
            row+=[b.sdbnd.fitfunc]
            # Diphoton fit parameters
            row+=[b.sdbnd.diphotonchi2,b.sdbnd.diphotonndf]
            for fidx in range(len(b.sdbnd.fitparams)):
                row+=[b.sdbnd.fitparams[fidx]]
                row+=[b.sdbnd.fiterrors[fidx]]
            # Purities
            for u in b.sdbnd.uarr:
                row+=[u]
            # Modulation names
            for modname in b.sdbnd.latexmods:
                row+=[modname]
            # Modulation parameters
            for fidx in range(b.sdbnd.nmods):
                row+=[b.sdbnd.sigbg_params_u0[fidx], b.sdbnd.sig_params_u0[fidx], b.sdbnd.bg_params_u0[fidx],
                      b.sdbnd.sigbg_errors_u0[fidx], b.sdbnd.sig_errors_u0[fidx], b.sdbnd.bg_errors_u0[fidx]]
                row+=[b.sdbnd.sigbg_params_u1[fidx], b.sdbnd.sig_params_u1[fidx], b.sdbnd.bg_params_u1[fidx],
                      b.sdbnd.sigbg_errors_u1[fidx], b.sdbnd.sig_errors_u1[fidx], b.sdbnd.bg_errors_u1[fidx]]
                row+=[b.sdbnd.sigbg_params_u2[fidx], b.sdbnd.sig_params_u2[fidx], b.sdbnd.bg_params_u2[fidx],
                      b.sdbnd.sigbg_errors_u2[fidx], b.sdbnd.sig_errors_u2[fidx], b.sdbnd.bg_errors_u2[fidx]]
                row+=[b.sdbnd.sigbg_params_u3[fidx], b.sdbnd.sig_params_u3[fidx], b.sdbnd.bg_params_u3[fidx],
                      b.sdbnd.sigbg_errors_u3[fidx], b.sdbnd.sig_errors_u3[fidx], b.sdbnd.bg_errors_u3[fidx]]
            # Add row
            df_sdbnd.loc[len(df_sdbnd.index)]=row
        self.df_sdbnd=df_sdbnd
