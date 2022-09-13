#!/bin/bash

sidisdvcs="/volatile/clas12/rg-c/production/ana_data/dst/train/sidisdvcs/"
tmp_prefix=/work/clas12/users/gmat/tmp/rgc-scaler-run
destination=/volatile/clas12/users/gmat/clas12analysis.sidis.data/rgc-scaler


ls $sidisdvcs | sed -e s/[^0-9]//g| while read -r line; 
do 
run="${line##0}"
dir=/volatile/clas12/users/gmat/clas12analysis.sidis.data/rgc-scaler/
if ls $dir| sed -e s/[^0-9]//g| grep -w -q $run
then
    echo "Skipping run ${run}"
else
    echo "Scanning scalers for ${run}"
    sbatch scanSlurm.slurm ${run} ${tmp_prefix} ${destination}
fi
done    



