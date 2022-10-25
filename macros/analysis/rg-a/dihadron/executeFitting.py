import ROOT
import sys

ROOT.gROOT.ProcessLine(".x $BRUFIT/macros/LoadBru.C")
from binner import *
from fitter import *

infile_binned = str(sys.argv[1])
binTree = str(sys.argv[2])

# Create Fitting Bin from "merged_run_binned.root" and "binName"
fbin=FitBin(infile_binned,binTree)

# Perform the sideband fit
fbin.sidebandFit(1)

# Perform the sPlot fit
fbin.splotUnBinnedFit(1)

# Set the rows for their output csv
fbin.setsPlotRow()
fbin.setSidebandRow()

# Saves key fitting parameters from the sideband and splot algorithms into two separate csvs
fbin.writeRows() 

