#!/bin/bash

sidisdvcs="/volatile/clas12/rg-c/production/ana_data/*/8.3.2/dst/recon/*/"
tmp_prefix=/work/clas12/users/gmat/tmp/rgc-scaler-run
destination=/volatile/clas12/users/gmat/clas12analysis.sidis.data/rgc-scaler

for hipo in $sidisdvcs
do
    echo $(basename $hipo) | sed -e s/[^0-9]//g| while read -r line; 
do 	
    run="${line##0}"
    echo $run
    
    dir=/volatile/clas12/users/gmat/clas12analysis.sidis.data/rgc-scaler/
    #if ls $dir| sed -e s/[^0-9]//g| grep -w -q $run
    #then
#	echo "Skipping run ${run}"
#    else
    echo "Scanning recon train etc. for ${run}"
    cook=$(echo $hipo | grep -oP '(?<=ana_data/).*(?=/8.3.2/dst)')
    sbatch /work/clas12/users/gmat/CLAS12Analysis/macros/scanChargeAsymmetry.slurm ${run} ${cook}
#    fi
done    
done


