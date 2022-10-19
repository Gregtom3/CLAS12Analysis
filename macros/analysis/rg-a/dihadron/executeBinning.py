import ROOT
import numpy as np
import copy
import sys
ROOT.gROOT.ProcessLine(".x $BRUFIT/macros/LoadBru.C")

from binner import BinGrid, Binnify
import fitter

infile_merged = str(sys.argv[1])
binGridfile = str(sys.argv[2])
rootname = "tree_postprocess"

# Create BINNIFY structure by reading the binGrid
my_file = open(binGridfile, "r")
lines = my_file.readlines()
bg=BinGrid()
BINNIFY=Binnify("reco",infile_merged,"tree_postprocess")
for line in lines:
    content = line.replace('\n','').split(' ')
    if(content==['']):
        BINNIFY.addGrid(copy.deepcopy(bg))
        bg=BinGrid()
        continue
    else:
        binType=content[0]
        bins=[float(b) for b in content[1:] if(b!='')]
        bg.setBins(binType,bins)
        if(line==lines[-1]):
            BINNIFY.addGrid(copy.deepcopy(bg))

# Split TTree into smaller bins
BINNIFY.Binnify()
