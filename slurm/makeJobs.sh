#!/bin/bash

workdir="/work/clas12/users/gmat/CLAS12Analysis"
hipodir=/cache/clas12/rg-a/production/montecarlo/clasdis/fall2018/torus-1/v1/bkg45nA_10604MeV/
outputdir="/work/clas12/users/gmat/CLAS12Analysis/data/fall2018-torus-1-v1-bkg45nA_10604MeV"
rootname="test_"
processdir="/work/clas12/users/gmat/CLAS12Analysis/macros/dihadron_process/"
processcodename="pipi0_process.C"

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
    echo "#SBATCH --mem-per-cpu=5000" >> $file
    echo "#SBATCH --job-name=${rootname}${i}" >> $file
    echo "#SBATCH --time=01:00:00" >> $file
    echo "#SBATCH --chdir=${workdir}" >> $file
    echo "#SBATCH --output=${workdir}/slurm/output/%x-%j-%N.out" >> $file
    echo "#SBATCH --error=${workdir}/slurm/output/%x-%j-%N.err" >> $file
    echo "echo ${workdir}" >> $file
    echo "source /group/clas12/packages/setup.csh" >> $file
    echo "module load clas12/pro" >> $file
    echo "set CLAS12ROOT=/w/hallb-scshelf2102/clas12/users/gmat/packages/clas12root" >> $file
    echo "set CCDB_HOME=${CLAS12ROOT}/ccdb" >> $file
    echo "source ${CCDB_HOME}/environment.csh" >> $file
    echo "cd ${processdir}" >> $file    
    echo "clas12root ${processcodename}\\(\\\"${hipofile}\\\",\\\"${outputdir}/${rootname}${i}.root\\\"\\)" >> $file   
    echo "sbatch shells/${rootname}${i}.sh" >> $runJobs
    exit
    #i=$((i+1))
done
