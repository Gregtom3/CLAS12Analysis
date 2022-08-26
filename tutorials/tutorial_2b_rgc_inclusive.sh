#!/bin/bash

# --------------------------
# Hipo dir defintions
# --------------------------

RGC_16327="/volatile/clas12/rg-c/production/ana_data/dst/train/sidisdvcs/sidisdvcs_016327.hipo"
RGC_16328="/volatile/clas12/rg-c/production/ana_data/dst/train/sidisdvcs/sidisdvcs_016328.hipo"
RGC_16329="/volatile/clas12/rg-c/production/ana_data/dst/train/sidisdvcs/sidisdvcs_016329.hipo"
RGC_16330="/volatile/clas12/rg-c/production/ana_data/dst/train/sidisdvcs/sidisdvcs_016330.hipo"

# --------------------------
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# User edits fields below
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# --------------------------

# Username
# --------------------------------------------------------
username="<USERNAME>"

# Location of CLAS12Analysis directory
# --------------------------------------------------------
CLAS12Analysisdir="/path/to/CLAS12Analysis/"

# Location of hipo directories for analysis
# --------------------------------------------------------
declare -a hipodirs=($RGC_16327 $RGC_16328 $RGC_16329 $RGC_16330)

# Beam energy associated with hipo files
# --------------------------------------------------------
beamEs=(10.5473 10.5473 10.5473 10.5473)

# Name of output directory
# --------------------------------------------------------
outdir="tutorial_2b"

# Prefix for the output files from the analysis
# --------------------------------------------------------
rootname="test"

# Code locations 
# --------------------------------------------------------
processcode="${CLAS12Analysisdir}/tutorials/tutorial_2a_rgc_inclusive.C"

# Location of locally installed clas12root package
# See https://github.com/JeffersonLab/clas12root
# --------------------------------------------------------
clas12rootdir="/path/to/clas12root"

# Job Parameters
# --------------------------------------------------------
memPerCPU=4000
nCPUs=4

# --------------------------
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# No more editing below
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# --------------------------
farmoutdir="/farm_out/$username/$outdir/$rootname/"
volatiledir="/volatile/clas12/users/$username/$outdir"
outputdir="$volatiledir/$rootname"
workdir=$CLAS12Analysisdir
processdir="$CLAS12Analysisdir/$processcodelocation"

shellSlurmDir="${farmoutdir}/shells"
outputSlurmDir="${farmoutdir}/output"

runJobs="${shellSlurmDir}/runJobs.sh"

if [ ! -d "/farm_out/$username/$outdir/" ]; then mkdir "/farm_out/$username/$outdir/"; fi
if [ -d "$farmoutdir" ]; then rm -Rf $farmoutdir; fi
if [ ! -d "$volatiledir" ]; then mkdir $volatiledir; fi
if [ -d "$outputdir" ]; then rm -Rf $outputdir; fi
mkdir $farmoutdir
mkdir $outputdir
mkdir $shellSlurmDir
mkdir $outputSlurmDir

i=0
for idx in "${!beamEs[@]}"
do
    beamE=${beamEs[$idx]}
    hipodir=${hipodirs[$idx]}
    for hipofile in "$hipodir"*
    do
	read runNumber <<< $(echo $hipofile | grep -oP '(?<=sidisdvcs_0).*(?=.hipo)')
	echo "Creating processing script for hipo file $((i+1))"
	processFile="${shellSlurmDir}/${rootname}_${i}.sh"
	touch $processFile
	chmod +x $processFile
	echo "#!/bin/tcsh" > $processFile
	echo "source /group/clas12/packages/setup.csh" >> $processFile
	echo "module load clas12/pro" >> $processFile
	echo "set CLAS12ROOT=${clas12rootdir}" >> $processFile
	echo "cd ${outputSlurmDir}" >> $processFile
	echo "clas12root ${processcode}\\(\\\"${hipofile}\\\",\\\"${outputdir}/${rootname}_${runNumber}.root\\\",${beamE}\\)" >> $processFile   
	i=$((i+1))
    done
done

i=$((i-1))

# Create script to run SIDISKinematicsReco process+postprocess on hipo files
# --------------------------------------------------------------------------
touch $runJobs
chmod +x $runJobs
echo "#!/bin/bash" > $runJobs
echo "#SBATCH --account=clas12" >> $runJobs
echo "#SBATCH --partition=production" >> $runJobs
echo "#SBATCH --mem-per-cpu=${memPerCPU}" >> $runJobs
echo "#SBATCH --job-name=run${rootname}" >> $runJobs
echo "#SBATCH --cpus-per-task=${nCPUs}" >> $runJobs
echo "#SBATCH --time=24:00:00" >> $runJobs
echo "#SBATCH --chdir=${workdir}" >> $runJobs
echo "#SBATCH --array=0-${i}" >> $runJobs
echo "#SBATCH --output=${outputSlurmDir}/%x-%a.out" >> $runJobs
echo "#SBATCH --error=${outputSlurmDir}/%x-%a.err" >> $runJobs    
echo "${shellSlurmDir}/${rootname}_\${SLURM_ARRAY_TASK_ID}.sh" >> $runJobs

echo "Submitting analysis jobs for the selected HIPO files"
echo " "
sbatch $runJobs
echo "----------------------------------------------------"
echo "Here is how you can monitor your runs..."
echo "----------------------------------------------------"
echo "Location of slurm .out/.err : $outputSlurmDir"
echo "Location of shell scripts (which run the tutorial_2a_rgc_inclusive.C file) : $shellSlurmDir"
echo "Location of saved .root files : $outputdir"

