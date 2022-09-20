#!/bin/bash

USERNAME="$USER"

if [ $# -lt 3 ]; then
  echo """
  USAGE: $0 [run-group] [MC/train] [dir] [flags(optional)]
  Automates the sending of slurm analysis jobs for CLAS12 hipo files
  Each job executes CLAS12Analysis process macros for a single run
   - [run-group]: rg-a rg-c
   - [MC/train]: Specifies MC or which train to use for the analysis
                   MC
                   sidisdvcs 
                   gmn
   - [dir]: Path in /volatile/clas12/$USERNAME/ where the simulation output should be sent
            Path in /farm_out/$USERNAME/ where the slurm output should be sent
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
#cd ..
CLAS12Analysisdir=$(cd ../;pwd)

#cd -
if [[ ! -d "/volatile/clas12/users/$USERNAME/" ]]; then
    echo "ERROR: Please create the directory /volatile/users/$USERNAME/"
    exit 2
fi
if [[ ! -d "/farm_out/$USERNAME/" ]]; then
    echo "ERROR: Please create the directory /volatile/$USERNAME/"
    exit 2
fi

volatiledir="/volatile/clas12/users/$USERNAME/$dir"
farmoutdir="/farm_out/$USERNAME/$dir"

if [[ -d "$volatiledir" ]]; then
    if [ [ ${booleans["o"]} ] && [ ${booleans["a"]} ] ]; then
	echo "You cannot use the -o (overwrite) and -a (append) flag simultaneously...Aborting..."
	exit 2
    fi
    elif [ ${booleans["o"]} ]; then
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
    elif [ ${booleans["a"]} ]; then
	echo "Are you sure you would like to append to $volatiledir (Y/N):"
	read append
	if [ $append == "Y" ]; then
	    echo "Appending to $volatiledir ..."
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




if [ ${flags["myFlag"]} ]; then
    echo $rungroup
fi

