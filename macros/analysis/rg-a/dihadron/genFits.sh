#!/bin/bash
hl="---------------------------------------------------------------"
USERNAME="$USER"

CLAS12Analysisdir="/work/clas12/users/$USERNAME/CLAS12Analysis"

if [ $# -lt 4 ]; then
  echo """
  USAGE: $0 [run-group] [MC/train] [dir] [binName]
  Automates the sending of slurm fitting jobs for CLAS12Analysis binned TTrees
  **THIS PROGRAM IS TYPICALLY RUN FOLLOWING genBins.sh, see ana.ipynb for details**
  Each job executes the binning procedure for each run's ana TTree
   - [run-group]: rg-a rg-c
   - [MC/train]: Specifies MC or which train to use for the analysis
                   MC        (rga and rgc)
                   nSidis    (rga only)
                   sidisdvcs (rgc only) 
                   gmn       (rgc only)
   - [dir]: Path in /volatile/clas12/users/$USERNAME/clas12analysis.sidis.data/run-group/(MC/train)<dir> where the simulation output of CLAS12Analysis is stored
   - [binName]:  specifies which speicifc bin to fit (must be the name of a TTree in merged_run_binned.root)
  """
  exit 2
fi

RUNGROUPS="rg-a rg-c rga rgc"
ANAS="MC sidisdvcs gmn nSidis"
nCPUs=1
memPerCPU=1000

declare -A flags
declare -A booleans
args=()

while [ "$1" ];
do
    arg=$1
    if [ "${1:0:1}" == "-" ]
    then
      shift
      rev=$(echo "$arg" | rev)
      if [ -z "$1" ] || [ "${1:0:1}" == "-" ] || [ "${rev:0:1}" == ":" ]
      then
        bool=$(echo ${arg:1} | sed s/://g)
        booleans[$bool]=true
      else
        value=$1
        flags[${arg:1}]=$value
        shift
      fi
    else
      args+=("$arg")
      shift
    fi
done

rungroup=${args[0]}
ana=${args[1]}
dir=${args[2]}
bin=${args[3]}
echo $hl
# -----------------------------------------------------------------------------------------

volatiledir="/volatile/clas12/users/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/$dir"
if [ ! -d ${volatiledir} ]; then
    echo $hl
    echo "ERROR: Directory in volatile ${volatiledir} doesn't exist. Are you sure that you ran CLAS12Analysis first?...Aborting..."
    exit 2
fi

farmoutdir="/farm_out/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/$dir"
if [ ! -d ${farmoutdir} ]; then
    echo $hl
    echo "ERROR: Directory in farm_out ${farmoutdir} doesn't exist. Are you sure that you ran CLAS12Analysis first?...Aborting..."
    exit 2
fi

shellSlurmDir="${farmoutdir}/shells"
outputSlurmDir="${farmoutdir}/output"

fitterScript="$CLAS12Analysisdir/macros/analysis/rg-a/dihadron/executeFitting.py"

binnedFile="${volatiledir}/merged_run_binned.root"
# -----------------------------------------------------------------------------------------
echo "Creating shell script for fitting ${bin}"
shellFile="${shellSlurmDir}/fitshell_${bin}.sh"
if [[ -f $shellFile ]]; then
    rm $shellFile
fi
touch $shellFile
chmod +x $shellFile
cat >> $shellFile <<EOF
#!/bin/tcsh
source /group/clas12/packages/setup.csh
module load clas12/pro
cd "$CLAS12Analysisdir/macros/analysis/rg-a/dihadron"
python3 $fitterScript $binnedFile $bin
EOF
echo "Creating slurm script for fitting ${bin}"
slurmprocessFile="${shellSlurmDir}/fitslurm_${bin}.slurm"
    if [[ -f $slurmprocessFile ]]; then
	rm $slurmprocessFile
    fi
    touch $slurmprocessFile
    cat >> $slurmprocessFile <<EOF
#!/bin/bash
#SBATCH --account=clas12
#SBATCH --partition=production
#SBATCH --mem-per-cpu=${memPerCPU}
#SBATCH --job-name=fit_${rungroup}_${ana}_${dir}_${bin}
#SBATCH --cpus-per-task=${nCPUs}
#SBATCH --time=24:00:00
#SBATCH --output=${outputSlurmDir}/fit${bin}.out
#SBATCH --error=${outputSlurmDir}/fit${bin}.err
$shellFile
EOF
echo $hl
echo $hl
echo "DONE WITH FILE CREATION"
echo $hl
echo $hl
echo "Submitting processing slurm job"
sbatch $slurmprocessFile
echo "DONE"
