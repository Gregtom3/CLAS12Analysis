#!/bin/bash
#SBATCH --account=clas12
#SBATCH --partition=production
#SBATCH --mem-per-cpu=4000
#SBATCH --job-name=rgc-scaler
#SBATCH --cpus-per-task=4
#SBATCH --output=/farm_out/gmat/rgc-scaler-%j.out
#SBATCH --error=/farm_out/gmat/rgc-scaler-%j.err
/work/clas12/users/gmat/CLAS12Analysis/macros/scanTrain.sh $1 $2 $3 $4
/work/clas12/users/gmat/CLAS12Analysis/macros/scanRecon.sh $1 $2 $3 $4
