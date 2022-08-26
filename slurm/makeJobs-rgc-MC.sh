#!/bin/bash

# --------------------------
# Hipo dir defintions
# --------------------------

RGC_MC_MINUS_NEUTRON="/work/cebaf24gev/sidis/reconstructed/polarized-minus-10.5GeV-neutron/hipo/"
RGC_MC_PLUS_NEUTRON="/work/cebaf24gev/sidis/reconstructed/polarized-plus-10.5GeV-neutron/hipo/"
RGC_MC_MINUS_PROTON="/work/cebaf24gev/sidis/reconstructed/polarized-minus-10.5GeV-proton/hipo/"
RGC_MC_PLUS_PROTON="/work/cebaf24gev/sidis/reconstructed/polarized-plus-10.5GeV-proton/hipo/"

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
declare -a hipodirs=($RGC_MC_MINUS_NEUTRON $RGC_MC_PLUS_NEUTRON $RGC_MC_MINUS_PROTON $RGC_MC_PLUS_PROTON)
declare -a hiponames=("minus_neutron" "plus_neutron" "minus_proton" "plus_proton") # Output file tags
max=100 # Number of hipo files to analyze per folder

# Beam energy associated with hipo files
# --------------------------------------------------------
beamEs=(10.5 10.5 10.5 10.5)

# Name of output directory
# --------------------------------------------------------
outdir="rgc-MC"

# Prefix for the output files from the analysis
# --------------------------------------------------------
rootname="my-rgc-MC"

# Code locations
# --------------------------------------------------------
processcode="${CLAS12Analysisdir}/macros/process/rg-c/inclusive_RGC_MC_process.C"

# Location of clas12root package
# --------------------------------------------------------
clas12rootdir="/path/to/clas12root"

# Job Parameters
# --------------------------------------------------------
memPerCPU=2000
nCPUs=1

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
    hiponame=${hiponames[$idx]}
    j=0
    for hipofile in "$hipodir"*
    do
	echo "Creating processing script for hipo file $((i+1))"
	processFile="${shellSlurmDir}/${rootname}_${i}.sh"
	touch $processFile
	chmod +x $processFile
	echo "#!/bin/tcsh" > $processFile
	echo "source /group/clas12/packages/setup.csh" >> $processFile
	echo "module load clas12/pro" >> $processFile
	echo "set CLAS12ROOT=${clas12rootdir}" >> $processFile
	echo "cd ${outputSlurmDir}" >> $processFile
	echo "clas12root ${processcode}\\(\\\"${hipofile}\\\",\\\"${outputdir}/${rootname}_${hiponame}_${i}.root\\\",${beamE}\\)" >> $processFile   
	i=$((i+1))
	j=$((j+1))
	if [[ $j == $max ]]; then
	    break
	fi
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

echo "----------------------------------------------------"
echo "Submitting analysis jobs for the selected HIPO files"
echo " "
sbatch $runJobs
echo "----------------------------------------------------"
echo "Here is how you can monitor your runs..."
echo "----------------------------------------------------"
echo "Location of slurm .out/.err : $outputSlurmDir"
echo "Location of shell scripts : $shellSlurmDir"
echo "Location of saved .root files : $outputdir"
