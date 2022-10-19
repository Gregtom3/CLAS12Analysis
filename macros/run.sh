#!/bin/bash
sidisdvcs=""
if [ ! -z ${1} ]; then
    sidisdvcs="/volatile/clas12/rg-c/production/ana_data/*BT/${1}/dst/train/sidisdvcs/*"
else
    sidisdvcs="/volatile/clas12/rg-c/production/ana_data/*BT/dst/train/sidisdvcs/*"
fi
tmp_prefix=/work/clas12/users/gmat/tmp/rgc-scaler-run
destination=/volatile/clas12/users/gmat/clas12analysis.sidis.data/rgc-scaler${1}

for hipo in $sidisdvcs
do
    echo $(basename $hipo) | sed -e s/[^0-9]//g| while read -r line; 
do 	
    run="${line##0}"
    dir=/volatile/clas12/users/gmat/clas12analysis.sidis.data/rgc-scaler${1}/
    if ls $dir| sed -e s/[^0-9]//g| grep -w -q $run
    then
	echo "Skipping run ${run}"
    else
	echo "Scanning scalers for ${run} ${1}"
	cook=$(echo $hipo | grep -oP '(?<=ana_data/).*(?=/dst)')
	sbatch /work/clas12/users/gmat/CLAS12Analysis/macros/scanSlurm.slurm ${run} ${tmp_prefix} ${destination} ${cook}
    fi
done    
done


