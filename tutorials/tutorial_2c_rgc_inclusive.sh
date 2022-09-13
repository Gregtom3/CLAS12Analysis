#!/bin/bash

# --------------------------
# Hipo dir defintions
# ** recon location added for beam charge asymmetry
# --------------------------

RGC_sidisdvcs="/volatile/clas12/rg-c/production/ana_data/dst/train/sidisdvcs/"
RGC_sidisdvcs_recon="/volatile/clas12/rg-c/production/ana_data/dst/recon/"

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
declare -a hipodirs=($RGC_sidisdvcs)

# Beam energy associated with hipo files
# --------------------------------------------------------
beamEs=(10.5473 10.5473 10.5473 10.5473)

# Name of output directory
# --------------------------------------------------------
outdir="tutorial_2c"

# Prefix for the output files from the analysis
# --------------------------------------------------------
rootname="test_2c"

# Code locations 
# --------------------------------------------------------
processcode="${CLAS12Analysisdir}/tutorials/tutorial_2a_rgc_inclusive.C"
organizecode="${CLAS12Analysisdir}/macros/organize_rgc.py"
# The above macro will run after all jobs are completed
# organize_rgc.py reads in from the RCDB to reorganize the output .root files

# Location of locally installed clas12root package
# See https://github.com/JeffersonLab/clas12root
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
farmoutdir="/farm_out/$username/$outdir/$rootname/"
volatiledir="/volatile/clas12/users/$username/$outdir"
outputdir="$volatiledir/$rootname"
workdir=$CLAS12Analysisdir
processdir="$CLAS12Analysisdir/$processcodelocation"

shellSlurmDir="${farmoutdir}/shells"
outputSlurmDir="${farmoutdir}/output"

runJobs="${shellSlurmDir}/runJobs.sh"
organizeFile="${shellSlurmDir}/organize.sh"
organizeSlurm="${shellSlurmDir}/organizeSlurm.slurm"

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
	if [[ -z $runNumber ]]; then
	    continue
	fi
	echo "Creating processing script for hipo file $((i+1)) and runNumber $runNumber"
	processFile="${shellSlurmDir}/${rootname}_${i}.sh"
	touch $processFile
	chmod +x $processFile
	echo "#!/bin/tcsh" > $processFile
	echo "source /group/clas12/packages/setup.csh" >> $processFile
	echo "module load clas12/pro" >> $processFile
	echo "set CLAS12ROOT=${clas12rootdir}" >> $processFile
	echo "cd ${outputSlurmDir}" >> $processFile
	echo "clas12root ${processcode}\\(\\\"${hipofile}\\\",\\\"${outputdir}/${rootname}_${runNumber}.root\\\",${beamE}\\)" >> $processFile   
	if ! cat "${CLAS12Analysisdir}/util/runHelicityCounts.csv" | cut -d, -f1 | grep -Fxq "${runNumber}" 
	then
	    echo "clas12root ${CLAS12Analysisdir}/macros/scanChargeAsymmetry.C\\(\\\"${RGC_sidisdvcs_recon}0${runNumber}/rec_clas_0${runNumber}.evio.\\\",\\\"${RGC_sidisdvcs}sidisdvcs_0${runNumber}.hipo\\\",\\\"${CLAS12Analysisdir}/util/runHelicityCounts.csv\\\"\\)" >> $processFile
	    echo "    Calculating run ${runNumber}'s beam charge asymmetry by parsing through recon"
	fi
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

# Job for organizing output files
# ---------------------------------
touch $organizeFile
chmod +x $organizeFile

cat >> $organizeFile <<EOF
#!/bin/bash
cd \$outputSlurmDir
organizeOutFile=\"organize.txt\"
touch \$organizeOutFile
jobsLeft=999
while [ \$jobsLeft -ne 0 ]
do
    read jobsLeft <<< \$(echo "\$(squeue -u gmat --format="%.18i %.9P %.30j %.8u %.8T %.10M %.9l %.6D %R")" | grep run${rootname} | awk 'END{print NR}')
    echo \$jobsLeft >> \$organizeOutFile
sleep 30
done 
/apps/python/2.7.18/bin/python $organizecode $outputdir/ $rootname
EOF

# Job for running the organize script
# ------------------------------------------------
touch $organizeSlurm
chmod +x $organizeSlurm
echo "#!/bin/bash" > $organizeSlurm
echo "#SBATCH --account=clas12" >> $organizeSlurm
echo "#SBATCH --partition=production" >> $organizeSlurm
echo "#SBATCH --mem-per-cpu=${memPerCPU}" >> $organizeSlurm
echo "#SBATCH --job-name=organizejob_${rootname}" >> $organizeSlurm
echo "#SBATCH --cpus-per-task=${nCPUs}" >> $organizeSlurm
echo "#SBATCH --time=24:00:00" >> $organizeSlurm
echo "#SBATCH --chdir=${workdir}" >> $organizeSlurm
echo "#SBATCH --output=${outputSlurmDir}/organize.out" >> $organizeSlurm
echo "#SBATCH --error=${outputSlurmDir}/organize.err" >> $organizeSlurm    
echo "${organizeFile}" >> $organizeSlurm







echo "Submitting analysis jobs for the selected HIPO files"
echo " "
sbatch $runJobs
echo "Submitting organize job to clean up output directory following job completion"
echo " "
sbatch $organizeSlurm
echo "----------------------------------------------------"
echo "Here is how you can monitor your runs..."
echo "----------------------------------------------------"
echo "Location of slurm .out/.err : $outputSlurmDir"
echo "Location of shell scripts (which run the tutorial_2a_rgc_inclusive.C file) : $shellSlurmDir"
echo "Location of saved .root files : $outputdir"

