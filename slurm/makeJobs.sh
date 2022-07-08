#!/bin/bash
# --------------------------
# Hipo dir defintions
# --------------------------
RGA_F18_IN="/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v1/dst/train/nSidis/"
RGA_F18_OUT="/cache/clas12/rg-a/production/recon/fall2018/torus+1/pass1/v1/dst/train/nSidis/"
RGA_S19_IN="/cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v1/dst/train/nSidis/"

RGA_S19_IN="/cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v1/dst/train/nSidis/"

RGA_F18_IN_BATCH="/cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v1/dst/train/nSidis/nSidis_005032.hipo"

# --------------------------
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# User edits fields below
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# --------------------------

# Location of CLAS12Analysis directory
# --------------------------------------------------------
CLAS12Analysisdir="/work/clas12/users/gmat/CLAS12Analysis/"

# Location of hipo directories for analysis
# --------------------------------------------------------
hipodirs=($RGA_F18_IN_BATCH)

# Beam energy associated with hipo files
# --------------------------------------------------------
beamEs=(10.6)

# Name of output directory
# --------------------------------------------------------
#outdir="/volatile/clas12/users/gmat/clas12analysis.sidis.data/fall2018-torus-1-v1-nSidis"
#outdir="/volatile/clas12/users/gmat/clas12analysis.sidis.data/rg-a"
outdir="/volatile/clas12/users/gmat/clas12analysis.sidis.data/fall2018-torus-1-small-batch"
# Prefix for the output files from the analysis
# --------------------------------------------------------
rootname="july8"

# Code locations
# --------------------------------------------------------
processcode="macros/dihadron_process/pipi0_process.C"
postprocesscode="macros/dihadron_process/pipi0_postprocess_only.C"
mergecode="macros/mergeTrees.C"
bincode="macros/dihadron_process/pipi0_binner.C"
fitcode="macros/dihadron_process/pipi0_fitter.C"

# Location of clas12root package
# --------------------------------------------------------
clas12rootdir="/work/clas12/users/gmat/packages/clas12root"

# Job Parameters
# --------------------------------------------------------
memPerCPU=8000
nCPUs=4

# --------------------------
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# No more editing below
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# *-*-*-*-*-*-*-*-*-*-*-*-*-
# --------------------------

workdir=$CLAS12Analysisdir
outputdir=$outdir
outputdir+="/"
outputdir+=$rootname
processdir=$CLAS12Analysisdir
processdir+=$processcodelocation
shellSlurmDir="${outputdir}/shells"
outputSlurmDir="${outputdir}/output"
runJobs="${shellSlurmDir}/runJobs.sh"
runJobsPostProcess="${shellSlurmDir}/runJobsPostProcess.sh"
mergeFile="${shellSlurmDir}/merge.sh"
mergeSlurm="${shellSlurmDir}/mergeSlurm.slurm"
mergeFile="${shellSlurmDir}/merge.sh"
mergeSlurm="${shellSlurmDir}/mergeSlurm.slurm"
fitFile="${shellSlurmDir}/fit.sh"
fitSlurm="${shellSlurmDir}/fitSlurm.slurm"


if [ -d "$outputdir" ]; then rm -Rf $outputdir; fi
mkdir $outputdir
mkdir $shellSlurmDir
mkdir $outputSlurmDir

i=0
for h_idx in "${!hipodirs[@]}"
do
    hipodir=${hipodirs[$h_idx]}
    for hipofile in "$hipodir"/*
    do
	beamE=${beamEs[$h_idx]}
	processFile="${shellSlurmDir}/${rootname}_${i}.sh"
	touch $processFile
	chmod +x $processFile
	echo "#!/bin/tcsh" > $processFile
	echo "source /group/clas12/packages/setup.csh" >> $processFile
	echo "module load clas12/pro" >> $processFile
	echo "set CLAS12ROOT=${clas12rootdir}" >> $processFile
	echo "cd ${outputSlurmDir}" >> $processFile
       	echo "rm ${rootname}_${i}.out" >> $processFile
	echo "clas12root ${processcode}\\(\\\"${hipofile}\\\",\\\"${outputdir}/${rootname}_${i}.root\\\",${beamE}\\)" >> $processFile   

	postprocessFile="${shellSlurmDir}/${rootname}_${i}_postprocess.sh"
	touch $postprocessFile
	chmod +x $postprocessFile
	echo "#!/bin/tcsh" > $postprocessFile
	echo "source /group/clas12/packages/setup.csh" >> $postprocessFile
	echo "module load clas12/pro" >> $postprocessFile
	echo "set CLAS12ROOT=${clas12rootdir}" >> $postprocessFile
	echo "cd ${outputSlurmDir}" >> $postprocessFile
	echo "rm ${rootname}_${i}.out" >> $postprocessFile
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
echo "#SBATCH --output=${outputSlurmDir}/%x-%a.out" >> $runJobsPostProcess
echo "#SBATCH --error=${outputSlurmDir}/%x-%a.err" >> $runJobsPostProcess    
echo "${shellSlurmDir}/${rootname}_\${SLURM_ARRAY_TASK_ID}_postprocess.sh" >> $runJobsPostProcess
echo "sbatch $mergeSlurm" >> $runJobsPostProcess

# Create a file which will wait until (int) 0 is outputted by each job
# This signifies the end of the batch
# When all jobs are complete, this script will proceed to then...
#    - Queue merging of all tree_postprocess in the separate .root files
#    - ???
#    - ???
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
echo "    for outputFile in ./*.out" >> $mergeFile
echo "    do" >> $mergeFile
echo "        lastLine=\$(tail -1 \$outputFile)" >> $mergeFile
echo "        if [[ \"\$lastLine\" == \"(int) 0\" ]]; then" >>  $mergeFile
echo "            finishedJobs=\$((finishedJobs+1))" >> $mergeFile
echo "        fi" >> $mergeFile
echo "    done" >> $mergeFile
echo "echo \"\$finishedJobs completed out of \$nJobs\" >> \$mergeOutFile" >> $mergeFile 
echo "sleep 30" >> $mergeFile
echo "done" >> $mergeFile
echo "clas12root ${bincode}\(\\\"${outputdir}/merged_${rootname}.root\\\"\)" >> $mergeFile
echo "${fitFile} ${outputdir}/merged_${rootname}.root.txt" >> $mergeFile

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
echo "${mergeFile}" >> $mergeSlurm

# Script for sending a job for each bin
# ------------------------------------------------
touch $fitFile
chmod +x $fitFile
echo "#!/bin/bash" > $fitFile
echo "j=0" >> $fitFile
echo "while IFS= read -r line; do" >> $fitFile
echo "    sbatch --output=${outputSlurmDir}/fit-\$line.out --error=${outputSlurmDir}/fit-\$line.err $fitSlurm \$line" >> $fitFile
echo "    j=\$((j+1))" >> $fitFile
echo "done" >> $fitFile
echo "cd $outputSlurmDir" >> $fitFile
echo "fitOutFile=\"fit.txt\"" >> $fitFile
echo "touch \$fitOutFile" >> $fitFile
echo "finishedJobs=0" >> $fitFile
echo "nJobs=\$j" >> $fitFile
echo "lastLine=\"\"" >> $fitFile
echo "while [ \$finishedJobs -ne \$nJobs ]" >> $fitFile
echo "do" >> $fitFile
echo "    finishedJobs=0" >> $fitFile
echo "    for outputFile in ./fit*.out" >> $fitFile
echo "    do" >> $fitFile
echo "        lastLine=\$(tail -1 \$outputFile)" >> $fitFile
echo "        if [[ \"\$lastLine\" == \"(int) 0\" ]]; then" >>  $fitFile
echo "            finishedJobs=\$((finishedJobs+1))" >> $fitFile
echo "        fi" >> $fitFile
echo "    done" >> $fitFile
echo "echo \"\$finishedJobs completed out of \$nJobs\" >> \$fitOutFile" >> $fitFile 
echo "sleep 30" >> $fitFile
echo "done" >> $fitFile




echo "Submitting analysis jobs for the selected HIPO files"
echo " "
sbatch $runJobs
echo "----------------------------------------------------"
echo "Submitting merge script"
echo " "
sbatch $mergeSlurm
echo "----------------------------------------------------"
