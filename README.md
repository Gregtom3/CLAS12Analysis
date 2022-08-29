# CLAS12Analysis
---
Analysis tool for reading hipo4 files with clas12root
# Setup Tutorial
---
1. Ensure both **ROOTSYS** and **CLAS12ROOT** are environment variables (see https://github.com/JeffersonLab/clas12root for details on installing CLAS12ROOT). For ROOT, we need 6.24.06, which should automatically install with 'module load clas12/pro'

2. Load in the CLAS12Analysis git repo, along with submodules (currently on brufit)

        git clone --recurse-submodules https://www.github.com/Gregtom3/CLAS12Analysis

3. Install brufit first, as the CLAS12Analysis repo depends on it heavily for fitting. Read the README inside brufit for instructions

4. Exit brufit and open the CLAS12Analysis directory. Enter into the build directory

        cd build
        

5. Run 

        ./autogen.sh --prefix=/path/to/CLAS12Analysis/


6. Run `make` within the build directory

7. Run `make install` within the build directory

8. Leave the build directory and check to see that a lib and include directory have been created.

9. In your home directory, edit the .cshrc, add the following line

        set LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/path/to/CLAS12Analysis/lib
        
10. Reopen your terminal or `source ~/.cshrc` to have the LD_LIBRARY_PATH appended to

11. Keep in mind that, as of the current git build, I have not figured out how to `#include "<INSERT-CLAS12ANA-HEADER.h"` in programs such as **macros/rg-a/pipi0_process.C**. You will need to edit the path to the corresponding header files yourself. 

# Structure
---
- **build/**
    - Directory containing necessary files for building/making the clas12ana.so library
- **data/**
    - Directory for storing ".root" files after processing/post-processing hipo4 files. The ".root" files stored here typically contain TTrees of particle kinematics, event variables, and other postprocessing features (such as diphoton mass for pi0 decays)
- **include/**
    - Directory created after running 'make' in "build". See **Setup** for instructions
- **lib/**
    - Same as above
- **macros/**
    - Directory which hosts two types of analysis scripts. All of these scripts are intended for the user to edit and run themselves. There is no other directory (other than **src/** and **slurm/**) which the user must edit files within to conduct their desired analysis.
        - *Processing Scripts*: These C++ macros are run via [clas12root](https://github.com/JeffersonLab/clas12root) (ex: `clas12root pipi0_process.root`). The macro creates a `SIDISKinematicsReco` object from the compiled clas12ana.so library, and serves as the backbone of the analysis. A `Settings` object is additionally created, allowing the user to set cuts for the analysis using built-in functions. This object is also used to specify which banks the user wants to analyze (Monte Carlo and/or Reco). Lastly, after all events within the hipo4 file had been analyzed, the user can set a post processing method. This will trigger the `SIDISKinematicsReco` object to reload the reconstructed event TTree to perform additional analysis, storing the results in *tree_postprocess*. Some scripts are listed with the tag `postprocess_only`. These scripts will quickly load up a pre-existing reconstructed event TTree to run a newly defined post-processing method. Post-processing methods can be found in *src/PostProcess.C*
        - *Analysis Scripts*: These C++ macros are (typically) *.ipynb* files. They read in the TTree's (*tree_MC* , *tree_reco*, *tree_postprocess* ) created by the aforementioned processing scripts. 
- **slurm/**
    - Directory containing the shell script `./makeJobs-*.sh`. This shell script is edited by the user to run parallel Processing Scripts in **macros/**. The script is told which directory contains the .hipo files of interest, then loops over each file in the list, creating a slurm job for each. A tutorial on how to use slurm shell scripts is included in **tutorials/**
- **src/**
    - Directory containing several .C and .h files for conducting the analysis. Workflow follows `SIDISKinematicsReco.C`, which loops over events in the `process_events` method. Any user edits to this file will not take into effect until the library is recompiled with make
- **tutorials/**
    - Directory containing some basic tutorial scripts for processing hipo files and slurm scripts using the clas12ana.so library.
---
# Analysis Suite Flowchart
![FLOWCHART](/util/poster.png)
---
# Tutorials

### 1. Processing a small hipo file


To get started using the library, `cd tutorials/` and open up the *tutorial_1a_rga_dihadron.C* file with a text editor. Here, you can specify the hipo4 file desired for the analysis (its already setup for you at this stage), the cuts you want to apply, and the postprocessing method you want to complete. The file has plenty of comments to get you familiarized with how the cuts are set. Additional methods do exist in *src/Settings.h*, so feel free to check those out and play around with them.

** Please note you must change the location of the header files in `#include` to match their location in your local repo **

To run the tutorial script, type...

        clas12root tutorial_1a_rga_dihadron.C
        
While running, the program uses functions native to `clas12root` to read hipo banks event-by-event. The program identifies the scattered electron to skip events which do not satisfy $Q^{2}$, $y$, and $W$ cuts. Additional particle specific cuts are made as well. So long as an event passes the reconstructed particle/event cuts, analysis from the monte carlo banks are also saved.

Using root, we can then open the `output.root` file to check out the resulting TTrees. I'd recommend printing each of the trees to see what exactly was stored. More information on how `tree_reco` and `tree_MC` store their data can be found by digging through **src/SIDISKinematicsReco.C**.

        root output.root
        tree_MC->Print()
        tree_reco->Print()
        tree_postprocess->Print()
        
If you have graphics enabled, you can plot the diphoton mass stored in `tree_postprocess`. The parameter (flag) saved in the tree tells the user (-1 --> diphotons did not originate from same parent pi0) or (1 --> diphotons originated from same parent pi0)

        tree_postprocess->Draw("Mgg","flag==1")

### 2. Processing multiple hipo files using slurm

With an understanding of how **process**ing macros, such as `tutorial_1a_rga_dihadron.C`, work, we can begin taking advantage of slurm to process many HIPO files in parallel. The first program provided in the tutorial which accomplishes this is `tutorial_2b_rgc_inclusive.sh`. This shell script will (after user-edits) send 4 slurm analysis jobs of  `tutorial_2a_rgc_inclusive.C` for 4 separate runs of RGC. The user can open `tutorial_2a_rgc_inclusive.C` to see the settings selected in the analysis.

To send the 4 analysis jobs, first open the `tutorial_2b_rgc_inclusive.sh` shell script. Inside, you should see comments denoting where the user will typically need to edit the shell script to personalize it, set the location of the hipo files, coding macros and beam energies. Three fields must be edited for the program to operate. These are `username` , `CLAS12Analysisdir`, and `clas12rootdir`.

Sending the jobs is accomplished by typing

        ./tutorial_2b_rgc_inclusive.sh
        
The progress of the slurm jobs can be monitored by the `squeue -u <USERNAME>` command. The shell script should also output the location of the root files, output & error text files being generated. All in all, you will be left with 4 root files, each containing the separate analysis of a single RGC run.

### 3. Processing all RGC analysis files

A slurm script with extra steps has been added for the purpose of RGC analysis, `tutorial_2c_rgc_inclusive.sh`. As of August 26th 2022, cooked RGC trains for sidis/dvcs are located in */volatile/clas12/rg-c/production/ana_data/dst/train/sidisdvcs*. Since there are several dozen HIPO files here, this slurm script sends every HIPO file in that directory individually to `tutorial_2a_rgc_inclusive.C` , passing it through the SIDISKinematicsReco workflow. After all jobs are completed, an extra step, handled by the program `macros/organize_rgc.py`, uses the RCDB (Run Conditions Database) to organize the several dozen analyzed ROOT files into subdirectories. These subdirectories are currently split by the target type (NH3, ND3, C) and whether or not the half wave plate (which flips the incoming electron's beam helicity before striking the target) is IN or OUT. A final `rcdb.csv` is created in the ROOT file output directory, storing some key RCDB values such as target polarization, nEvents, etc.



---
Author: Gregory Matousek

Contact: gregory.matousek@duke.edu
