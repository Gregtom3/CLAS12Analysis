import ROOT
import numpy as np
import copy
from itertools import repeat
import os
import re

VALID_BINS=["x","Q2","pt","Mdihadron","x","z"]

# Class for maintaining single bin information
class Bin:
    
    def __init__(self):
        self.names=[]
        self.bounds=[]
        self.objName=""
        self.ttree=0
        self.params={
            "run" : np.array([0.0]),
            "helicity" : np.array([0.0]),
            "x" : np.array([10.0]),
            "Q2" : np.array([10.0]),
            "y" : np.array([0.0]),
            "W" : np.array([0.0]),
            "z" : np.array([0.0]),
            "pt" : np.array([0.0]),
            "Mdihadron" : np.array([0.0]),
            "Mdiphoton" : np.array([0.0]),
            "phi_h" : np.array([0.0]),
            "phi_R0" : np.array([0.0]),
            "phi_R1" : np.array([0.0]),
            "truex" : np.array([0.0]),
            "trueQ2" : np.array([0.0]),
            "truey" : np.array([0.0]),
            "trueW" : np.array([0.0]),
            "truez" : np.array([0.0]),
            "truept" : np.array([0.0]),
            "trueMdihadron" : np.array([0.0]),
            "trueMdiphoton" : np.array([0.0]),
            "truephi_h" : np.array([0.0]),
            "truephi_R0" : np.array([0.0]),
            "truephi_R1" : np.array([0.0]),
            "PID1" : np.array([0.0]),
            "PID2" : np.array([0.0]),
            "PIDpi" : np.array([0.0]),
            "parentPID1" : np.array([0.0]),
            "parentPID2" : np.array([0.0]),
            "parentPIDpi" : np.array([0.0]),
            "parentparentPID1" : np.array([0.0]),
            "parentparentPID2" : np.array([0.0]),
            "parentparentPIDpi" : np.array([0.0]),
            "mcconnect" : np.array([0.0]),
            "fidmerge"  : np.array([0.0])
        }
        self.dim=0
    
    def addBinInfo(self,name,lowerB,upperB):
        #if(name in self.names or [lowerB,upperB] in self.bounds):
        #    print("WARNING: addBinInfo() ")
        self.names.append(name)
        self.bounds.append([lowerB,upperB])
        self.objName=self.binName()
        self.dim=self.dim+1
    def binName(self):
        objName="bin{}d_".format(len(self.names))
        for n,b in zip(self.names,self.bounds):
            if(n==self.names[-1]):
                objName=objName+str(n)+"_"+str(b[0])+"_"+str(b[1])
            else:
                objName=objName+str(n)+"_"+str(b[0])+"_"+str(b[1])+"_"
        return objName
    
    def printBin(self):
        print(self.objName)
        
    # Returns true if the event is within the desired binning
    def contains(self,iev):
        for n,b in zip(self.names,self.bounds):
            exec("global val; val=iev.{}".format(n))
            if(val<b[0] or val>b[1]):
                return False
        return True
    
    # Set TTree
    def setTTree(self):
        self.ttree=ROOT.TTree(self.objName,"Binned TTree")
        for p in self.params:
            self.ttree.Branch(str(p),self.params[p],'{}/D'.format(str(p)))
            
    # Fill TTree
    def FillTree(self,iev):
        for p in self.params:
            if(p=="fidmerge"):
                self.params[p][0]=self.params[p][0]+1
            else:
                self.params[p][0]=0.0
                exec("global val; val=iev.{}".format(str(p)))
                self.params[p][0]=val
        self.ttree.Fill()
            
    # Get bin parameters from objName
    def setParamsFromName(self,binname):
        self.objName=binname
        substrs=binname.split('_')
        self.dim=[int(s) for s in substrs[0] if s.isdigit()][0]
        for isub in np.arange(1,len(substrs),3):
            self.names.append(substrs[isub])
            self.bounds.append([float(substrs[isub+1]),float(substrs[isub+2])])
        self.objName=self.binName()

# Class for storing multiple bin objects
class BinGrid:
    
    def __init__(self):
        self.dim=0
        self.Bins=[]
    
    # Create a multidimensional binning grid
    def createBinGrid(self,binnames,edges):
        # If a BinGrid had already been created, skip
        if(self.Bins!=[]):
            print("ERROR: BinGrid can only be created on a fresh initialization...Aborting...")
            return -1
        
        # Loop over the binnames and edges to generate the grid
        for n,e in zip(binnames,edges):
            if(self.__setBins(n,e)==-1):
                return -1
            self.dim=self.dim+1
            
    # Set binning along one direction
    # Can be called multiple times to produce multi-dimensional binnings
    # - binname = ["x","Q2","pT","Mh","x","z"]
    # - edges = [list of bin edges (including leftmost and rightmost boundaries)]
    # Public method for class
    def setBins(self,binname,edges):
        self.__setBins(binname,edges)
        self.dim=self.dim+1
    # Private method for class
    def __setBins(self,binname,edges):
        
        
        # Test if binname is valid
        if(not binname in VALID_BINS):
            print("ERROR: Binname",binname,"is not one of the bintypes this program supports. Please use one of",VALID_BINS,"...Aborting...")
            return -1
        
        
        # If bins have already bin created (i.e. already did 1d)
        if(self.Bins != []): 
            # Test to see if this binname already exists
            for B in self.Bins:
                if(binname in B.names):
                    print("ERROR: Binname",binname,"is already in use...Aborting...")
                    return -1
            # Duplicate each current bin for every new bin added
            #tmpBins=copy.deepcopy(self.Bins)
            tmpBins=[]
            for B in self.Bins:
                for l,u in zip(edges[:-1],edges[1:]):
                    BB=copy.deepcopy(B)
                    BB.addBinInfo(binname,l,u)
                    tmpBins.append(BB)
            self.Bins=tmpBins
            
            
        # If these are the first bins to be created
        else:
            for idx_edge in range(len(edges)-1):
                newBin=Bin()
                newBin.addBinInfo(binname,edges[idx_edge],edges[idx_edge+1])
                self.Bins.append(newBin)
            
            
        # Get current number of bins
        self.nbins=len(self.Bins)
    
    # Check if bin exists in the grid
    def hasBin(self,b1):
        for b2 in self.Bins:
            if(b1.names==b2.names and b1.bounds==b2.bounds):
                return True
        return False
        
    # Removing specifc bin from the grid
    def removeBin(self,b1):
        for b2 in self.Bins:
            if(b1.names==b2.names and b1.bounds==b2.bounds):
                self.Bins.remove(b2)
                return
        print("WARNING: removeBin() called, but bin to be removed was not found...continuing...")
        return

# Class for storign multiple BinGrids and separating TTree into multiple trees
class Binnify:
    
    def __init__(self,datatype,infile,intree):
        if(not datatype in ["MC","reco"]):
            print("ERROR: Datatype for Binnify must be either <MC> or <reco>...Aborting...")
            return -1
        if(not os.path.exists(infile)):
            print("ERROR: Input file",infile,"for Binnify does not exist...Aborting...")
            return -1
        self.tfile=ROOT.TFile(infile,"READ")
        self.outfile=ROOT.TFile(infile.replace(".root","_binned.root"),"RECREATE")
        if(not self.tfile.Get(intree)):
            print("ERROR: TTree",intree,"does not exist within",infile,"...Aborting...")
            
        
        self.datatype=datatype
        self.ttree=self.tfile.Get(intree)
        self.grids=[]
    
    # Add binGrid to Binnify
    def addGrid(self,binGrid):
        # Need to double check that two grids do not have duplicate bins
        for bg in self.grids:
            for b in copy.deepcopy(bg.Bins):
                if(binGrid.hasBin(b)):
                    print("Removing duplicate bin",b.objName)
                    bg.removeBin(b)
        
        # Append binGrid to list
        self.grids.append(copy.deepcopy(binGrid))
    
    # Create binned TTrees within binGrid
    def createTTrees(self):
        for bg in self.grids:
            for b in bg.Bins:
                b.setTTree()
        return 
    
    def saveTTrees(self):
        for bg in self.grids:
            for b in bg.Bins:
                self.outfile.WriteObject(b.ttree,"bin{}d_{}".format(bg.dim,b.objName))
        
    def Binnify(self):
        
        self.createTTrees()
        i,j=0,0
        N=self.ttree.GetEntries()
        for iev in self.ttree:
            j=j+1
            if(j%10000==0):
                print("Completed {}".format(j))
            for bg in self.grids:
                for b in bg.Bins:
                    if(b.contains(iev)):
                        b.FillTree(iev)   
                        i=i+1
        self.outfile.Write()    
        print(j)
        
