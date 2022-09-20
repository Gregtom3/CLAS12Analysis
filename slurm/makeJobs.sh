#!/bin/bash

USERNAME="$USER"

if [ $# -lt 3 ]; then
  echo """
  USAGE: $0 [run-group] [MC/train] [dir] [flags(optional)]
  Automates the sending of slurm analysis jobs for CLAS12 hipo files
  Each job executes CLAS12Analysis process macros for a single run
   - [run-group]: rg-a rg-c
   - [MC/train]: Specifies MC or which train to use for the analysis
                   MC        (rga and rgc)
                   nSidis    (rga only)
                   sidisdvcs (rgc only) 
                   gmn       (rgc only)
   - [dir]: Path in /volatile/clas12/$USERNAME/clas12analysis.sidis.data/run-group/(MC/train)<dir> where the simulation output should be sent
            Path in /farm_out/$USERNAME/clas12analysis.sidis.data/run-group/(MC/train)/<dir> where the slurm output should be sent
   - [flags]:
              -runmin <INT>(specify the run minimum)
              -runmax <INT>(specify the run maximum)
              -o      (overwrite existing volatile_dir)
              -maxJobs <INT>(maximum number of jobs to send)
  """
  exit 2
fi

RUNGROUPS="rg-a rg-c rga rgc"
ANAS="MC sidisdvcs gmn"


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
    echo "ERROR: Run group $rungroup not valid. Must use from following list [$RUNGROUPS]"
    exit 2
else
    rungroup="${rungroup//-}"
fi
	
if ! echo "$ANAS" | grep -q "$ana"; then
    echo "ERROR: Analysis type $ana not valid. Must use from following list [$ANAS]"
    exit 2
fi

# ------------------------------------
# Set file names
# ------------------------------------
CLAS12Analysisdir=$(cd ../;pwd)
if [[ ! -d "/volatile/clas12/users/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/" ]]; then
    echo "ERROR: Please create the directory /volatile/clas12/users/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/"
    exit 2
fi
if [[ ! -d "/farm_out/$USERNAME/" ]]; then
    echo "ERROR: Please create the directory /farm_out/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/"
    exit 2
fi

volatiledir="/volatile/clas12/users/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/$dir"
farmoutdir="/farm_out/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/$dir"
existingruns=0
if [ -d "${volatiledir}/" ]; then
    if [ -z " ${booleans[\"o\"]}" ] && [ -z " ${booleans[\"a\"]}" ]; then
	echo "You cannot use the -o (overwrite) and -a (append) flag simultaneously...Aborting..."
	exit 2
    elif [ -z ${booleans["o"]} ]; then
	echo "Are you sure you would like to overwrite $volatiledir (Y/N):"
	read overwrite
	if [ $overwrite == "Y" ]; then
	    echo "Overwriting $volatiledir ..."
	    rm -r $volatiledir
	    echo "Overwritten!"
	elif [ $overwrite == "N" ]; then
	    echo "Overwrite cancelled...Aborting..."
	    exit 2
	else
	    echo "ERROR: Please answer Y or N...Aborting..."
	    exit 2
	fi
    elif [ -z ${booleans["a"]} ]; then
	echo "Are you sure you would like to append to $volatiledir (Y/N):"
	read append
	if [ $append == "Y" ]; then
	    echo "Appending to $volatiledir ..."
	    existingruns=$(ls $volatiledir | sed 's/[^0-9]//g' | sort -n)
	elif [ $append == "N" ]; then
	    echo "Appending cancelled...Aborting..."
	    exit 2
	else
	    echo "ERROR: Please answer Y or N...Aborting..."
	    exit 2
	fi
    else
        echo "ERROR: Directory $volatiledir already exists. You must specify a new [dir] or use -o to overwrite OR -a to append...Aborting..."
        exit 2
    fi	    
else
    echo "Making directory $volatiledir ..."
    mkdir -p $volatiledir
fi

if [[ -d "$farmoutdir" ]]; then
    echo "Found directory $farmoutdir , should we delete it (Y/N):"
    read farmoutdelete
    if [ $farmoutdelete == "Y" ]; then
	echo "Removing old /farm_out dir $farmoutdir"
	rm -r $farmoutdir
    elif [ $farmoutdelete == "N" ]; then
	echo "Deleting cancelled...Aborting..."
	exit 2
    else
	echo "ERROR: Please answer Y or N...Aborting..."
	exit 2
    fi
fi
mkdir -p $farmoutdir

shellSlurmDir="${farmoutdir}/shells"
outputSlurmDir="${farmoutdir}/output"
if [ -d $shellSlurmDir ]; then
    rm -r $shellSlurmDir
fi
if [ -d $outputSlurmDir ]; then
    rm -r $outputSlurmDir
fi
mkdir $shellSlurmDir
mkdir $outputSlurmDir

runJobs="${shellSlurmDir}/runJobs.sh"


processcode=""
if [ $rungroup == "rga" ]; then
    macroList=${ls "${CLAS12Analysisdir}/macros/process/rg-a"}
    echo "Please select one of the following processing scripts [${macroList}]"
    read macro
    processcode="${CLAS12Analysisdir}/macros/process/rg-a/${macro}"
    if echo "$macro" | grep -w -q "$macroList"; then
	echo "Using processing macro $macro"
    else
	echo "Processing macro $processcode not found...Aborting..."
	exit 2
    fi
elif [ $rungroup == "rgc" ]; then
    macroList=$(ls "${CLAS12Analysisdir}/macros/process/rg-c")
    echo "Please select one of the following processing scripts ${macroList}"
    read macro
    processcode="${CLAS12Analysisdir}/macros/process/rg-c/${macro}"
    if echo "$macro" | grep -w -q "$macroList"; then
	echo "Using processing macro $macro"
    else
	echo "Processing macro $processcode not found...Aborting..."
	exit 2
    fi
else
    echo "ERROR: Run-group $rungroup not found...Aborting..."
    exit 2
fi

trainhipodir=""
reconhipodir=""
if [ $rungroup == "rga" ]; then
    if [ $ana == "MC" ]; then
	trainhipodir="/cache/clas12/rg-a/production/montecarlo/clasdis/fall2018/torus-1/v1/bkg45nA_10604MeV/*.hipo"
    else
	trainhipodir="/cache/clas12/rg-a/production/recon/*20*/torus*/pass1/v1/dst/train/${ana}/*.hipo"
	reconhipodir="/cache/clas12/rg-a/production/recon/*20*/torus*/pass1/v1/dst/recon/*/*.hipo"
    fi
elif [ $rungroup == "rgc" ]; then
    if [ $ana == "MC" ]; then
	trainhipodir="/work/cebaf24gev/sidis/reconstructed/*/hipo/*.hipo"
    else
	trainhipodir="/volatile/clas12/rg-c/production/ana_data/*/dst/train/${ana}/*.hipo"
	reconhipodir="/volatile/clas12/rg-c/production/ana_data/*/dst/recon/*/*.hipo"
    fi
fi

filenum=0
for hipo in $trainhipodir
do
    beamE=0
    runNumber=0
    if [ $ana == "MC" ]; then
	if [ $rungroup == "rga"]; then
	    beamE=10.604
	else
	    beamE=10.5473
	fi
    else
	if [ $rungroup == "rga" ]; then
	    read runNumber <<< $(echo $hipo | grep -oP '(?<=${ana}_00).*(?=.hipo)')
	    if echo $hipo | grep -w -q "spring2019"; then
		beamE=10.2
	    else
		beamE=10.6
	    fi
	elif [ $rungroup == "rgc" ]; then
	    read runNumber <<< $(echo $hipo | grep -oP '(?<=${ana}_0).*(?=.hipo)')
	    beamE=10.5473
	fi
    fi
    
    if [ -z "${flags[\"runmin\"]}" ] && [ -z "${flags[\"runmax\"]}" ]; then
	if [[ "$runNumber" -gt ${flags["runmax"]} || "$runNumber" -lt ${flags["runmin"]} ]]; then
            continue
        fi
    fi
    
    if echo $existingruns | grep -w -q "$runNumber"; then
	echo "Skipping run $runNumber$ since it already exists in this to-be-appended directory ($volatiledir)"
	continue
    fi
    
    echo "Processing run ${runNumber} from hipo file $hipo"
    processFile="${shellSlurmDir}/run${runNumber}.sh"
    touch $processFile
    chmod +x $processFile
    cat >> $processFile <<EOF
    #!/bin/tcsh
    source /group/clas12/packages/setup.csh
    module load clas12/pro
    cd ${outputSlurmDir}
    clas12root ${processcode}\(\"${hipo}\",\"${volatiledir}/run${runNumber}.root\",${beamE}\)
EOF
    
    filenum=$((filenum+1))
    if [ -z "${flags[\"maxJobs\"]}" ]; then
	if [ $filenum -gt "${flags[\"maxJobs\"]}" ]; then
	    echo "Maximum jobs reached...Skipping rest"
	    break
	fi
    fi
done
