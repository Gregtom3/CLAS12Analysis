#!/bin/bash
# --------------------------
# Hipo dir defintions
# --------------------------

RGA_F18_IN="/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v1/dst/train/nSidis/"
RGA_F18_OUT="/cache/clas12/rg-a/production/recon/fall2018/torus+1/pass1/v1/dst/train/nSidis/"
RGA_S19_IN="/cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v1/dst/train/nSidis/"

RGA_F18_IN_BATCH="/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v1/dst/train/nSidis/nSidis_005032.hipo"

MC_F18_NOBG_OUT="/cache/clas12/rg-a/production/montecarlo/clasdis/fall2018/torus-1/v1/nobkg_10604MeV/"
# --------------------------
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# User edits fields below
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# --------------------------

# Username
# --------------------------------------------------------
username="gmat"

# Location of CLAS12Analysis directory
# --------------------------------------------------------
CLAS12Analysisdir="/work/clas12/users/gmat/CLAS12Analysis/"

# Location of hipo directories for analysis
# --------------------------------------------------------
declare -a hipodirs=($RGA_F18_IN $RGA_F18_OUT $RGA_S19_IN)

# Beam energy associated with hipo files
# --------------------------------------------------------
beamEs=(10.6 10.6 10.6)

# Analysis chain parameters
# Base chain = Processing  --> PostProcessing --> Merging
# --------------------------------------------------------
doAsymmetry=true # Binning --> Fitting --> Asymmetry 

# Name of output directory
# --------------------------------------------------------
outdir="clas12analysis.sidis.data/rg-a"

# Prefix for the output files from the analysis
# --------------------------------------------------------
rootname="aug23"

# Code locations
# --------------------------------------------------------
processcode="${CLAS12Analysisdir}/macros/process/rg-a/pipi0_process.C"
postprocesscode="${CLAS12Analysisdir}/macros/process/rg-a/pipi0_postprocess_only.C"
mergecode="${CLAS12Analysisdir}/macros/mergeTrees.C"
asymmetrycode="${CLAS12Analysisdir}/macros/process/rg-a/pipi0_asymmetry.C"

# Location of clas12root package
# --------------------------------------------------------
clas12rootdir="/work/clas12/users/gmat/packages/clas12root"

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
farmoutdir="/farm_out/$username/$outdir/$rootname"
volatiledir="/volatile/clas12/users/$username/$outdir"
outputdir="$volatiledir/$rootname"
workdir=$CLAS12Analysisdir
processdir="$CLAS12Analysisdir/$processcodelocation"

shellSlurmDir="${farmoutdir}/shells"
outputSlurmDir="${farmoutdir}/output"

runJobs="${shellSlurmDir}/runJobs.sh"
makeJobsPostProcess="${shellSlurmDir}/makeJobsPostProcess.sh"
runJobsPostProcess="${shellSlurmDir}/runJobsPostProcess.sh"
mergeFile="${shellSlurmDir}/merge.sh"
mergeSlurm="${shellSlurmDir}/mergeSlurm.slurm"
fitFile="${shellSlurmDir}/fit.sh"
fitSlurm="${shellSlurmDir}/fitSlurm.slurm"
asymSlurm="${shellSlurmDir}/asym.slurm"

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
	echo "Creating processing script for hipo file $((i+1))"
	processFile="${shellSlurmDir}/${rootname}_${i}.sh"
	touch $processFile
	chmod +x $processFile
	echo "#!/bin/tcsh" > $processFile
	echo "source /group/clas12/packages/setup.csh" >> $processFile
	echo "module load clas12/pro" >> $processFile
	echo "set CLAS12ROOT=${clas12rootdir}" >> $processFile
	echo "cd ${outputSlurmDir}" >> $processFile
	echo "clas12root ${processcode}\\(\\\"${hipofile}\\\",\\\"${outputdir}/${rootname}_${i}.root\\\",${beamE}\\)" >> $processFile   
	postprocessFile="${shellSlurmDir}/${rootname}_${i}_postprocess.sh"
	touch $postprocessFile
	chmod +x $postprocessFile
	echo "#!/bin/tcsh" > $postprocessFile
	echo "source /group/clas12/packages/setup.csh" >> $postprocessFile
	echo "module load clas12/pro" >> $postprocessFile
	echo "set CLAS12ROOT=${clas12rootdir}" >> $postprocessFile
	echo "cd ${outputSlurmDir}" >> $postprocessFile
	echo "clas12root ${postprocesscode}\\(\\\"${outputdir}/${rootname}_${i}.root\\\",${beamE}\\)" >> $postprocessFile   
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
echo "#SBATCH --job-name=${rootname}" >> $runJobs
echo "#SBATCH --cpus-per-task=${nCPUs}" >> $runJobs
echo "#SBATCH --time=24:00:00" >> $runJobs
echo "#SBATCH --chdir=${workdir}" >> $runJobs
echo "#SBATCH --array=0-${i}" >> $runJobs
echo "#SBATCH --output=${outputSlurmDir}/%x-%a.out" >> $runJobs
echo "#SBATCH --error=${outputSlurmDir}/%x-%a.err" >> $runJobs    
echo "${shellSlurmDir}/${rootname}_\${SLURM_ARRAY_TASK_ID}.sh" >> $runJobs


# Create script to run SIDISKinematicsReco postprocess on hipo files
# Unlike the script above, this postprocess exclusive script is meant to be
# run manually by the user in the case they want to make subtle changes
# to the post-processing module, without completely re-analyzing the HIPO files
# --------------------------------------------------------------------------
touch $runJobsPostProcess
chmod +x $runJobsPostProcess

echo "#!/bin/bash" > $runJobsPostProcess
echo "#SBATCH --account=clas12" >> $runJobsPostProcess
echo "#SBATCH --partition=production" >> $runJobsPostProcess
echo "#SBATCH --mem-per-cpu=${memPerCPU}" >> $runJobsPostProcess
echo "#SBATCH --job-name=${rootname}" >> $runJobsPostProcess
echo "#SBATCH --cpus-per-task=${nCPUs}" >> $runJobsPostProcess
echo "#SBATCH --time=24:00:00" >> $runJobsPostProcess
echo "#SBATCH --chdir=${workdir}" >> $runJobsPostProcess
echo "#SBATCH --array=0-${i}" >> $runJobsPostProcess
echo "#SBATCH --output=${outputSlurmDir}/%x-%a-postprocess.out" >> $runJobsPostProcess
echo "#SBATCH --error=${outputSlurmDir}/%x-%a-postprocess.err" >> $runJobsPostProcess    
echo "rm ${outputSlurmDir}/${rootname}*.out" >> $runJobsPostProcess
echo "${shellSlurmDir}/${rootname}_\${SLURM_ARRAY_TASK_ID}_postprocess.sh" >> $runJobsPostProcess

touch $makeJobsPostProcess
chmod +x $makeJobsPostProcess
echo "#!/bin/bash" > $makeJobsPostProcess
echo "sbatch $runJobsPostProcess" >> $makeJobsPostProcess
echo "sbatch $mergeSlurm 1" >> $makeJobsPostProcess

# Create a file which will wait until (int) 0 is outputted by each job
# This signifies the end of the batch
# Analysis chain flags can add Binning, Fitting, and Asymmetry calculations
# --------------------------------------------------------------------
touch $mergeFile
chmod +x $mergeFile
echo "#!/bin/bash" > $mergeFile
echo "cd $outputSlurmDir" >> $mergeFile
echo "mergeOutFile=\"merge.txt\"" >> $mergeFile
echo "touch \$mergeOutFile" >> $mergeFile
echo "finishedJobs=0" >> $mergeFile
echo "nJobs=$(($i+1))" >> $mergeFile
echo "lastLine=\"\"" >> $mergeFile
echo "while [ \$finishedJobs -ne \$nJobs ]" >> $mergeFile
echo "do" >> $mergeFile
echo "    finishedJobs=0" >> $mergeFile
echo "    for outputFile in ../output/${rootname}*.out" >> $mergeFile
echo "    do" >> $mergeFile
echo "        lastLine=\$(tail -1 \$outputFile)" >> $mergeFile
echo "        if [[ \"\$lastLine\" == \"(int) 0\" ]]; then" >>  $mergeFile
echo "            finishedJobs=\$((finishedJobs+1))" >> $mergeFile
echo "        fi" >> $mergeFile
echo "    done" >> $mergeFile
echo "echo \"\$finishedJobs completed out of \$nJobs\" >> \$mergeOutFile" >> $mergeFile 
echo "sleep 10" >> $mergeFile
echo "done" >> $mergeFile
echo "if [ \$1 == 0 ] ; then" >> $mergeFile
echo "    clas12root ${mergecode}\(\\\"${outputdir}\\\",\\\"${rootname}\\\",\\\"tree_reco\\\"\)" >> $mergeFile
echo "fi" >> $mergeFile
echo "clas12root ${mergecode}\(\\\"${outputdir}\\\",\\\"${rootname}\\\",\\\"tree_postprocess\\\"\)" >> $mergeFile
if [ "$doAsymmetry" == true ] ; then
    echo "clas12root ${asymmetrycode}\(\\\"${outputdir}\\\",\\\"merged_${rootname}\\\",\\\"\\\",1\)" >> $mergeFile
    echo "${fitFile} ${outputdir}/merged_${rootname}.root.txt" >> $mergeFile
fi


# Job for running the merge script
# ------------------------------------------------
touch $mergeSlurm
chmod +x $mergeSlurm
echo "#!/bin/bash" > $mergeSlurm
echo "#SBATCH --account=clas12" >> $mergeSlurm
echo "#SBATCH --partition=production" >> $mergeSlurm
echo "#SBATCH --mem-per-cpu=${memPerCPU}" >> $mergeSlurm
echo "#SBATCH --job-name=${rootname}_mergejob" >> $mergeSlurm
echo "#SBATCH --cpus-per-task=${nCPUs}" >> $mergeSlurm
echo "#SBATCH --time=24:00:00" >> $mergeSlurm
echo "#SBATCH --chdir=${workdir}" >> $mergeSlurm
echo "#SBATCH --output=${outputSlurmDir}/merge.out" >> $mergeSlurm
echo "#SBATCH --error=${outputSlurmDir}/merge.err" >> $mergeSlurm    
echo "${mergeFile} \$1" >> $mergeSlurm

# Script which sends a fitting job per bin
# After all fitting is completed, the asymmetry
# compilation program begins
# ------------------------------------------------
if [ "$doAsymmetry" == true ] ; then
    touch $fitFile
    chmod +x $fitFile
    echo "#!/bin/bash" > $fitFile
    echo "j=0" >> $fitFile
    echo "cd $outputSlurmDir" >> $fitFile
    echo "rm fit*" >> $fitFile
    echo "while IFS= read -r line; do" >> $fitFile
    echo "    sbatch --output=${outputSlurmDir}/fit-\$line.out --error=${outputSlurmDir}/fit-\$line.err $fitSlurm \$line" >> $fitFile
    echo "    j=\$((j+1))" >> $fitFile
    echo "done < \$1" >> $fitFile
    echo "fitOutFile=\"fit.txt\"" >> $fitFile
    echo "touch \$fitOutFile" >> $fitFile
    echo "finishedJobs=0" >> $fitFile
    echo "nJobs=\$j" >> $fitFile
    echo "lastLine=\"\"" >> $fitFile
    echo "while [ \$finishedJobs -ne \$nJobs ]" >> $fitFile
    echo "do" >> $fitFile
    echo "    finishedJobs=0" >> $fitFile
    echo "    for outputFile in ../output/fit*.out" >> $fitFile
    echo "    do" >> $fitFile
    echo "        lastLine=\$(tail -1 \$outputFile)" >> $fitFile
    echo "        if [[ \"\$lastLine\" == \"(int) 0\" ]]; then" >>  $fitFile
    echo "            finishedJobs=\$((finishedJobs+1))" >> $fitFile
    echo "        fi" >> $fitFile
    echo "    done" >> $fitFile
    echo "echo \"\$finishedJobs completed out of \$nJobs\" >> \$fitOutFile" >> $fitFile 
    echo "sleep 10" >> $fitFile
    echo "done" >> $fitFile
    echo "sbatch $asymSlurm" >> $fitFile

    # Slurm job for running the fitter
    # ------------------------------------------------
    touch $fitSlurm
    chmod +x $fitSlurm
    echo "#!/bin/bash" > $fitSlurm
    echo "binname=\$1" >> $fitSlurm
    echo "#SBATCH --account=clas12" >> $fitSlurm
    echo "#SBATCH --partition=production" >> $fitSlurm
    echo "#SBATCH --mem-per-cpu=${memPerCPU}" >> $fitSlurm
    echo "#SBATCH --job-name=${rootname}_fit" >> $fitSlurm
    echo "#SBATCH --cpus-per-task=${nCPUs}" >> $fitSlurm
    echo "#SBATCH --time=24:00:00" >> $fitSlurm
    echo "#SBATCH --chdir=${workdir}" >> $fitSlurm
    echo "clas12root ${asymmetrycode}\(\\\"${outputdir}\\\",\\\"merged_${rootname}\\\",\\\"\$binname\\\",2\)" >> $fitSlurm

    # Slurm job for running the asymmetry compiler
    # ------------------------------------------------
    touch $asymSlurm
    chmod +x $asymSlurm
    echo "#!/bin/bash" > $asymSlurm
    echo "binname=\$1" >> $asymSlurm
    echo "#SBATCH --account=clas12" >> $asymSlurm
    echo "#SBATCH --partition=production" >> $asymSlurm
    echo "#SBATCH --mem-per-cpu=${memPerCPU}" >> $asymSlurm
    echo "#SBATCH --job-name=${rootname}_asym" >> $asymSlurm
    echo "#SBATCH --cpus-per-task=${nCPUs}" >> $asymSlurm
    echo "#SBATCH --time=24:00:00" >> $asymSlurm
    echo "#SBATCH --chdir=${workdir}" >> $asymSlurm
    echo "clas12root ${asymmetrycode}\(\\\"${outputdir}\\\",\\\"\\\",\\\"\\\",3\)" >> $asymSlurm
fi


echo "Submitting analysis jobs for the selected HIPO files"
echo " "
sbatch $runJobs
echo "----------------------------------------------------"
echo "Submitting merge script"
echo " "
sbatch $mergeSlurm 0
echo "----------------------------------------------------"
