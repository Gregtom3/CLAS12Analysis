#!/bin/bash
# --------------------------
# User edits fields below
# ----------------------------------------------------------------------------------------------
# Location of CLAS12Analysis directory
CLAS12Analysisdir="/work/clas12/users/gmat/CLAS12Analysis/"

# Location of hipo files for analysis
#hipodir=/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v1/dst/train/nSidis/
hipodir=/work/clas12/rg-a/montecarlo/fall2018/torus-1/clasdis/nobg/

# Beam energy associated with hipo files
beamE=10.6

# Name of output directory
outdir="/volatile/clas12/users/gmat/clas12analysis.sidis.data/fall2018-torus-1-nobg"

# Prefix for the output files from the analysis
rootname="june30"

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


runJobs="${workdir}slurm/runJobs.sh"
touch $runJobs

chmod +x $runJobs
echo " " > $runJobs

shellSlurmDir="${outputdir}/shells"
outputSlurmDir="${outputdir}/output"
mkdir $shellSlurmDir
mkdir $outputSlurmDir

i=0

for hipofile in "$hipodir"/*
do
    file="${shellSlurmDir}/${rootname}_${i}.sh"
    touch $file
    echo "#!/bin/tcsh" > $file
    echo "#SBATCH --account=clas12" >> $file
    echo "#SBATCH --partition=production" >> $file
    echo "#SBATCH --mem-per-cpu=4000" >> $file
    echo "#SBATCH --job-name=${rootname}_${i}" >> $file
    echo "#SBATCH --cpus-per-task=4" >> $file
    echo "#SBATCH --time=24:00:00" >> $file
    echo "#SBATCH --chdir=${workdir}" >> $file
    echo "#SBATCH --output=${outputSlurmDir}/%x-%j-%N.out" >> $file
    echo "#SBATCH --error=${outputSlurmDir}/%x-%j-%N.err" >> $file
    echo "echo ${workdir}" >> $file
    echo "source /group/clas12/packages/setup.csh" >> $file
    echo "module load clas12/pro" >> $file
    echo "set CLAS12ROOT=${clas12rootdir}" >> $file
    echo "set CCDB_HOME=${CLAS12ROOT}/ccdb" >> $file
    #echo "source ${CCDB_HOME}/environment.csh" >> $file
    echo "cd ${processdir}" >> $file    
    echo "clas12root ${processcodename}\\(\\\"${hipofile}\\\",\\\"${outputdir}/${rootname}_${i}.root\\\",${beamE}\\)" >> $file   
    echo "sbatch ${shellSlurmDir}/${rootname}_${i}.sh" >> $runJobs
    i=$((i+1))
done
