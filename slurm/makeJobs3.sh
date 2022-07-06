#!/bin/bash
# --------------------------
# User edits fields below
# ----------------------------------------------------------------------------------------------
# Location of CLAS12Analysis directory
CLAS12Analysisdir="/work/clas12/users/gmat/CLAS12Analysis/"

# Location of hipo files for analysis
hipodir=/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v1/dst/train/nSidis/
#hipodir=/work/clas12/rg-a/montecarlo/fall2018/torus-1/clasdis/nobg/

# Beam energy associated with hipo files
beamE=10.6

# Name of output directory
outdir="/volatile/clas12/users/gmat/clas12analysis.sidis.data/fall2018-torus-1-v1-nSidis"

# Prefix for the output files from the analysis
rootname="july6"

# Process macro code location
processcodelocation="macros/dihadron_process/"
processcodename="pipi0_process.C"

# Location of clas12root package
clas12rootdir="/work/clas12/users/gmat/packages/clas12root"

#----------------------------------------------------------------------------------------------
# No more editing required below
#----------------------------------------------------------------------------------------------
workdir=$CLAS12Analysisdir
outputdir=$outdir
outputdir+="/"
outputdir+=$rootname
if [ -d "$outputdir" ]; then rm -Rf $outputdir; fi
mkdir $outputdir

processdir=$CLAS12Analysisdir
processdir+=$processcodelocation

shellSlurmDir="${outputdir}/shells"
outputSlurmDir="${outputdir}/output"
mkdir $shellSlurmDir
mkdir $outputSlurmDir

i=0

for hipofile in "$hipodir"/*
do
    file="${shellSlurmDir}/${rootname}_${i}.sh"
    touch $file
    chmod +x $file
    echo "#!/bin/tcsh" > $file
    echo "source /group/clas12/packages/setup.csh" >> $file
    echo "module load clas12/pro" >> $file
    echo "set CLAS12ROOT=${clas12rootdir}" >> $file
    echo "cd ${processdir}" >> $file    
    echo "clas12root ${processcodename}\\(\\\"${hipofile}\\\",\\\"${outputdir}/${rootname}_${i}.root\\\",${beamE}\\)" >> $file   
    i=$((i+1))
done

i=$((i-1))

runJobs="${workdir}slurm/runJobs.sh"
touch $runJobs
chmod +x $runJobs

echo "#!/bin/bash" > $runJobs
echo "#SBATCH --account=clas12" >> $runJobs
echo "#SBATCH --partition=production" >> $runJobs
echo "#SBATCH --mem-per-cpu=32000" >> $runJobs
echo "#SBATCH --job-name=${rootname}" >> $runJobs
echo "#SBATCH --cpus-per-task=4" >> $runJobs
echo "#SBATCH --time=24:00:00" >> $runJobs
echo "#SBATCH --chdir=${workdir}" >> $runJobs
echo "#SBATCH --array=0-${i}" >> $runJobs
echo "#SBATCH --output=${outputSlurmDir}/%x-%a.out" >> $runJobs
echo "#SBATCH --error=${outputSlurmDir}/%x-%a.err" >> $runJobs    
echo "${shellSlurmDir}/${rootname}_\${SLURM_ARRAY_TASK_ID}.sh" >> $runJobs
   
# Create a file which will wait until (int) 0 is outputted by each job
# This signifies the end of the batch 
waitFile="${outputSlurmDir}/wait.sh"
touch $waitFile
chmod +x $waitFile
echo "#!/bin/bash" > $waitFile
echo "cd $outputSlurmDir" >> $waitFile
echo "waitOutFile=\"wait.out\"" >> $waitFile
echo "touch \$waitOutFile" >> $waitFile
echo "finishedJobs=0" >> $waitFile
echo "nJobs=$i" >> $waitFile
echo "lastLine=\"\"" >> $waitFile
echo "while [ \$finishedJobs -ne \$nJobs ]" >> $waitFile
echo "do" >> $waitFile
echo "    \$finishedJobs=0" >> $waitFile
echo "    for outputFile in ./*.out" >> $waitFile
echo "    do" >> $waitFile
echo "        \$lastLine=\$(tail -1 \$outputFile)" >> $waitFile
echo "        if [[ \"\$lastLine\" == \"(int) 0\" ]]; then" >>  $waitFile
echo "            \$finishedJobs=\$((finishedJobs+1))" >> $waitFile
echo "        fi" >> $waitFile
echo "    done" >> $waitFile
echo "echo \"\$finishedJobs completed out of \$nJobs\" >> \$waitOutFile" >> $waitFile 
echo "sleep 30" >> $waitFile
echo "done" >> $waitFile

echo "${outputSlurmDir}/wait.sh" >> $runJobs
  
