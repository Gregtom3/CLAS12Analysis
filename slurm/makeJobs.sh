#!/bin/bash
# --------------------------
# User edits fields below
# ----------------------------------------------------------------------------------------------
# Location of CLAS12Analysis directory
CLAS12Analysisdir="/path/to/CLAS12Analysis/"

# Location of hipo files for analysis
hipodir=/cache/clas12/rg-a/production/recon/fall2018/torus-1/pass1/v1/dst/train/nSidis/

# Beam energy associated with hipo files
beamE=10.6

# Name of output directory (to be created AHEAD OF TIME in CLAS12Analysis/data/<   >)
outdir="fall2018-torus-1-v1-nSidis"

# Prefix for the output files from the analysis
rootname="test_"

# Process macro code location
processcodename="macros/pipi0_process.C"

# Location of clas12root package
clas12rootdir="/path/to/clas12root"

#----------------------------------------------------------------------------------------------
# No more editing required below
#----------------------------------------------------------------------------------------------
workdir=$CLAS12Analysisdir
outputdir=$CLAS12Analysisdir+"data/"+$outdir
processdir=$CLAS12Analysisdir+$processcodename


runJobs="${workdir}/slurm/runJobs.sh"
touch $runJobs
chmod +x $runJobs
echo " " > $runJobs

i=0

for hipofile in "$hipodir"/*
do
    file="${workdir}/slurm/shells/${rootname}${i}.sh"
    touch $file
    echo "#!/bin/tcsh" > $file
    echo "#SBATCH --account=clas12" >> $file
    echo "#SBATCH --partition=production" >> $file
    echo "#SBATCH --mem-per-cpu=1000" >> $file
    echo "#SBATCH --job-name=${rootname}${i}" >> $file
    echo "#SBATCH --cpus-per-task=1" >> $file
    echo "#SBATCH --time=24:00:00" >> $file
    echo "#SBATCH --chdir=${workdir}" >> $file
    echo "#SBATCH --output=${workdir}/slurm/output/%x-%j-%N.out" >> $file
    echo "#SBATCH --error=${workdir}/slurm/output/%x-%j-%N.err" >> $file
    echo "echo ${workdir}" >> $file
    echo "source /group/clas12/packages/setup.csh" >> $file
    echo "module load clas12/pro" >> $file
    echo "set CLAS12ROOT=${clas12rootdir}" >> $file
    echo "set CCDB_HOME=${CLAS12ROOT}/ccdb" >> $file
    echo "source ${CCDB_HOME}/environment.csh" >> $file
    echo "cd ${processdir}" >> $file    
    echo "clas12root ${processcodename}\\(\\\"${hipofile}\\\",\\\"${outputdir}/${rootname}${i}.root\\\",${beamE}\\)" >> $file   
    echo "sbatch shells/${rootname}${i}.sh" >> $runJobs
    i=$((i+1))
done