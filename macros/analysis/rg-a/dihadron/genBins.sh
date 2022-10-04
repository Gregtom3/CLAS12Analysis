
#!/bin/bash
hl="---------------------------------------------------------------"
USERNAME="$USER"

if [ $# -lt 3 ]; then
  echo """
  USAGE: $0 [run-group] [MC/train] [dir]
  Automates the sending of slurm binning jobs for CLAS12Analysis TTrees
  Each job executes the binning procedure for each run's ana TTree
   - [run-group]: rg-a rg-c
   - [MC/train]: Specifies MC or which train to use for the analysis
                   MC        (rga and rgc)
                   nSidis    (rga only)
                   sidisdvcs (rgc only) 
                   gmn       (rgc only)
   - [dir]: Path in /volatile/clas12/users/$USERNAME/clas12analysis.sidis.data/run-group/(MC/train)<dir> where the simulation output of CLAS12Analysis is stored
  """
  if [ $# == 2 ]; then
      echo "Directories currently in /volatile/clas12/users/$USERNAME/clas12analysis.sidis.data/$1/$2/..."
      ls /volatile/clas12/users/$USERNAME/clas12analysis.sidis.data/$1/$2/
  fi
  exit 2
fi

RUNGROUPS="rg-a rg-c rga rgc"
ANAS="MC sidisdvcs gmn nSidis"
nCPUs=4
memPerCPU=4000

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

if ! echo "$RUNGROUPS" | grep -q "$rungroup"; then
    echo $hl
    echo "ERROR: Run group $rungroup not valid. Must use from following list [$RUNGROUPS]"
    exit 2
else
    rungroup="${rungroup//-}"
fi

if ! echo "$ANAS" | grep -q "$ana"; then
    echo $hl
    echo "ERROR: Analysis type $ana not valid. Must use from following list [$ANAS]"
    exit 2
fi

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
CLAS12Analysisdir="/work/clas12/users/$USERNAME/CLAS12Analysis"

binnerScript="$CLAS12Analysisdir/macros/analysis/rg-a/dihadron/executeBinning.py"

# -----------------------------------------------------------------------------------------

binGridList=$(ls "${CLAS12Analysisdir}/macros/analysis/rg-a/dihadron/binGrids")
echo "Please select one of the following binGrids ${binGridList}"
read binGrid
binGridFile="${CLAS12Analysisdir}/macros/analysis/rg-a/dihadron/binGrids/${binGrid}"
if echo "$binGrid" | grep -w -q "$binGridList"; then
    echo "Using binGrid $binGrid"
else
    echo "binGrid $binGrid not found...Aborting..."
    exit 2
fi







for rootfile in $volatiledir/*
do
    if [[ ! "$rootfile" == *"run"* ]]; then
	continue
    fi
    if [[ ${rootfile} == *"binned"* ]]; then
	continue
    fi
    if [[ ${rootfile} == *"merged"* ]]; then
	continue
    fi
    runNumber=-9999
    read runNumber <<< $(basename $rootfile | grep -oP '(?<=run).*(?=.root)')
    

    echo "Creating binning for run ${runNumber}"
    processFile="${shellSlurmDir}/binnedrun${runNumber}.sh"
    if [[ -f $processFile ]]; then
	rm $processFile
    fi
    touch $processFile
    chmod +x $processFile
    cat >> $processFile <<EOF
#!/bin/tcsh
source /group/clas12/packages/setup.csh
module load clas12/pro
cd ${outputSlurmDir}
python3 $binnerScript $rootfile $binGridFile
EOF
    slurmprocessFile="${shellSlurmDir}/binnedrun${runNumber}.slurm"
    if [[ -f $slurmprocessFile ]]; then
	rm $slurmprocessFile
    fi
    touch $slurmprocessFile
    cat >> $slurmprocessFile <<EOF
#!/bin/bash
#SBATCH --account=clas12
#SBATCH --partition=production
#SBATCH --mem-per-cpu=${memPerCPU}
#SBATCH --job-name=runbin_${rungroup}_${ana}_${dir}
#SBATCH --cpus-per-task=${nCPUs}
#SBATCH --time=24:00:00
#SBATCH --output=${outputSlurmDir}/runbin${runNumber}.out
#SBATCH --error=${outputSlurmDir}/runbin${runNumber}.err
$processFile
EOF
done

echo $hl
echo "Creating bin merge script" 
echo $hl

binmergeFile="${shellSlurmDir}/binmerge.sh"
binmergeSlurm="${shellSlurmDir}/binmergeSlurm.slurm"t
if [[ -f $binmergeFile ]]; then
    rm $binmergeFile
fi
if [[ -f $binmergeSlurm ]]; then
    rm $binmergeSlurm
fi
touch $binmergeFile
chmod +x $binmergeFile

cat >> $organizeFile <<EOF
#!/bin/bash
cd $outputSlurmDir
organizeOutFile=binmerge.txt
touch \$organizeOutFile
jobsLeft=999
while [ \$jobsLeft -ne 0 ]
do
    read jobsLeft <<< \$(echo "\$(squeue -u $USERNAME --format="%.18i %.9P %.30j %.8u %.8T %.10M %.9l %.6D %R")" | grep runbin_${rungroup}_${ana}_${dir} | awk 'END{print NR}')
    echo \$jobsLeft >> \$organizeOutFile
sleep 30
done 
clas12root ${CLAS12Analysisdir}/macros/mergeTrees.C\(\"${volatiledir}\",\"run*binned\",\"EVERYTHING\"\)
EOF

touch $binmergeSlurm
cat >> $binmergeSlurm <<EOF
#!/bin/bash
#SBATCH --account=clas12
#SBATCH --partition=production
#SBATCH --mem-per-cpu=${memPerCPU}
#SBATCH --job-name=binmerge_$rungroup_$ana_$dir
#SBATCH --cpus-per-task=${nCPUs}
#SBATCH --time=24:00:00
#SBATCH --output=${outputSlurmDir}/binmerge.out
#SBATCH --error=${outputSlurmDir}/binmerge.err
$binmergeFile
EOF

echo $hl
echo $hl
echo "DONE WITH FILE CREATION"
echo $hl
echo $hl
echo "Submitting processing slurm jobs"
ls $shellSlurmDir/binnedrun*.slurm | xargs -l bash -c 'sbatch -Q $0'
n1=$(ls $shellSlurmDir/binnedrun*.slurm | wc -l)
echo "Submitted ${n1} processing slurm jobs"
echo $hl
echo "Submitting binmerging job"
sbatch $binmergeSlurm
