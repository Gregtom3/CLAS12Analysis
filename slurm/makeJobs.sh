#!/bin/bash
hl="---------------------------------------------------------------"
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

# ------------------------------------
# Set file names
# ------------------------------------
CLAS12Analysisdir=$(cd ../;pwd)
if [[ ! -d "/volatile/clas12/users/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/" ]]; then
    echo $hl
    echo "ERROR: Please create the directory /volatile/clas12/users/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/"
    exit 2
fi
if [[ ! -d "/farm_out/$USERNAME/" ]]; then
    echo $hl
    echo "ERROR: Please create the directory /farm_out/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/"
    exit 2
fi

volatiledir="/volatile/clas12/users/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/$dir"
farmoutdir="/farm_out/$USERNAME/clas12analysis.sidis.data/$rungroup/$ana/$dir"
existingruns=0
if [ -d "${volatiledir}/" ]; then
    if [ ! -z ${booleans["o"]} ] && [ ! -z ${booleans["a"]} ]; then
	echo $hl
	echo "You cannot use the -o (overwrite) and -a (append) flag simultaneously...Aborting..."
	exit 2
    elif [ ! -z ${booleans["o"]} ]; then
	echo $hl
	echo "Are you sure you would like to overwrite $volatiledir (Y/N):"
	read overwrite
	if [ $overwrite == "Y" ]; then
	    echo $hl
	    echo "Overwriting $volatiledir ..."
	    rm -r $volatiledir
	    echo "Overwritten!"
	    mkdir -p $volatiledir
	elif [ $overwrite == "N" ]; then
	    echo $hl
	    echo "Overwrite cancelled...Aborting..."
	    exit 2
	else
	    echo $hl
	    echo "ERROR: Please answer Y or N...Aborting..."
	    exit 2
	fi
    elif [ ! -z ${booleans["a"]} ]; then
	echo $hl
	echo "Are you sure you would like to append to $volatiledir (Y/N):"
	read append
	if [ $append == "Y" ]; then
	    echo $hl
	    echo "Appending to $volatiledir ..."
	    existingruns=$(ls $volatiledir | sed 's/[^0-9]//g' | sort -n)
	elif [ $append == "N" ]; then
	    echo $hl
	    echo "Appending cancelled...Aborting..."
	    exit 2
	else
	    echo $hl
	    echo "ERROR: Please answer Y or N...Aborting..."
	    exit 2
	fi
    else
	echo $hl
        echo "ERROR: Directory $volatiledir already exists. You must specify a new [dir] or use -o to overwrite OR -a to append...Aborting..."
        exit 2
    fi	    
else
    echo $hl
    echo "Making directory $volatiledir ..."
    mkdir -p $volatiledir
fi

if [[ -d "$farmoutdir" ]]; then
    echo $hl
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

echo $hl
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

echo $hl
echo "Grabbing hipo directory and files within"
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
echo $hl

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
	    read runNumber <<< $(basename $hipo | grep -oP '(?<='$ana'_00).*(?=.hipo)')
	    if echo $hipo | grep -w -q "spring2019"; then
		beamE=10.2
	    else
		beamE=10.6
	    fi
	elif [ $rungroup == "rgc" ]; then
	    runNumber=$(basename $hipo | grep -oP '(?<='$ana'_0).*(?=.hipo)')
	    beamE=10.5473
	fi
    fi
    
    if [ ! -z ${flags["runmin"]} ] && [ ! -z ${flags["runmax"]} ]; then
	if [[ "$runNumber" -gt ${flags["runmax"]} || "$runNumber" -lt ${flags["runmin"]} ]]; then
            continue
        fi
    fi
    
    if echo $existingruns | grep -w -q "$runNumber"; then
	echo "Skipping run $runNumber$ since it already exists in this to-be-appended directory ($volatiledir)"
	continue
    fi
    
    echo "Creating processing script and slurm script for run ${runNumber}"
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

    slurmprocessFile="${shellSlurmDir}/run${runNumber}.slurm"
    touch $slurmprocessFile
    cat >> $slurmprocessFile <<EOF
#!/bin/bash
#SBATCH --account=clas12
#SBATCH --partition=production
#SBATCH --mem-per-cpu=${memPerCPU}
#SBATCH --job-name=runprocess_${rungroup}_${ana}_${dir}
#SBATCH --cpus-per-task=${nCPUs}
#SBATCH --time=24:00:00
#SBATCH --output=${outputSlurmDir}/%x-%a.out
#SBATCH --error=${outputSlurmDir}/%x-%a.err
$processFile
EOF

    filenum=$((filenum+1))
    if [ ! -z ${flags["maxJobs"]} ]; then
	if [ $filenum -gt "${flags[\"maxJobs\"]}" ]; then
	    echo $hl
	    echo "Maximum jobs reached...Skipping rest"
	    break
	fi
    fi
done

organizeFile="${shellSlurmDir}/organize.sh"
organizeSlurm="${shellSlurmDir}/organizeSlurm.slurm"
touch $organizeFile
chmod +x $organizeFile


# If rgc, run the organize_rgc.py script
# If rga, run the merge files script
needsOrganize=0
if [ $rungroup == "rgc" ] && [ $ana != "MC" ]; then
    echo $hl
    echo "Creating an organizing script for the RGC dataset"
    needsOrganize=1
    cat >> $organizeFile <<EOF
#!/bin/bash
cd \$outputSlurmDir
organizeOutFile=\"organize.txt\"
touch \$organizeOutFile
jobsLeft=999
while [ \$jobsLeft -ne 0 ]
do
    read jobsLeft <<< \$(echo "\$(squeue -u $USERNAME --format="%.18i %.9P %.30j %.8u %.8T %.10M %.9l %.6D %R")" | grep runprocess_${rungroup}_${ana}_${dir} | awk 'END{print NR}')
    echo \$jobsLeft >> \$organizeOutFile
sleep 30
done 
/apps/python/2.7.18/bin/python ${CLAS12Analysisdir}/macros/organize_rgc.py $volatiledir/ $dir
EOF
elif [ $rungroup == "rga" ]; then
    echo $hl
    echo "Creating a merging script for the RGA dataset"
    needsOrganize=1
    cat >> $organizeFile <<EOF
#!/bin/bash
cd \$outputSlurmDir
organizeOutFile=\"merge.txt\"
touch \$organizeOutFile
jobsLeft=999
while [ \$jobsLeft -ne 0 ]
do
    read jobsLeft <<< \$(echo "\$(squeue -u $USERNAME --format="%.18i %.9P %.30j %.8u %.8T %.10M %.9l %.6D %R")" | grep runprocess_$rungroup_$ana_$dir | awk 'END{print NR}')
    echo \$jobsLeft >> \$organizeOutFile
sleep 30
done 
clas12root ${CLAS12Analysisdir}/macros/mergeTrees.C\(\"${volatiledir}\",\"${dir}\",\"tree_reco\"\)
clas12root ${CLAS12Analysisdir}/macros/mergeTrees.C\(\"${volatiledir}\",\"${dir}\",\"tree_postprocess\"\)
EOF
    if [ $ana == "MC" ]; then
	break
    fi
    echo $hl
    echo "For RGA analysis, several analysis scripts can run after file merging...would you like any of these to be run (Y/N):"
    read rgaAfter
    rgaAfterMacro=0
    if [ $rgaAfter == "Y" ]; then
	macroList=${ls "${CLAS12Analysisdir}/macros/process/rg-a"}
	echo "Please select one of the following analysis scripts [${macroList}]"
	read macro
	processcode="${CLAS12Analysisdir}/macros/process/rg-a/${macro}"
	if echo "$macro" | grep -w -q "$macroList"; then
	    echo "Using processing macro $macro"
	    rgaAfterMacro=$macro
	else
	    echo "Analysis macro $processcode not found...Aborting..."
	    exit 2
	fi
    elif [ $rgaAfter == "N" ]; then
	echo "Not running further analysis"
    else
	echo "Invalid answer...Not running further analysis...Continuing..."
    fi
    
    if [ $rgaAfterMacro != 0 ]; then
	echo $hl
	echo "Creating postprocessing scripts and slurm scripts for RGA analysis"
	fitFile="${shellSlurmDir}/fit.sh"
	fitSlurm="${shellSlurmDir}/fitSlurm.slurm"
	asymSlurm="${shellSlurmDir}/asym.slurm"
	cat >> $organizeFile <<EOF
clas12root ${rgaAfterMacro}\(\"${volatiledir}\",\"merged_${dir}\",\"\",1\)
${fitFile} ${volatiledir}/merged_${dir}.root.txt
EOF
	touch $fitFile
	chmod +x $fitFile
	cat >> $fitFile <<EOF
#!/bin/bash
j=0
cd ${outputSlurmDir}
rm fit*
while IFS= read -r line; do
    sbatch --output=${outputSlurmDir}/fit-\$line.out --error=${outputSlurmDir}/fit-\$line.err $fitSlurm \$line
    j=\$((j+1))
done < \$1
fitOutFile=\"fit.txt\"
touch \$fitOutFile
finishedJobs=0
nJobs=\$j
lastLine=\"\"
while [ \$finishedJobs -ne \$nJobs ]
do
    finishedJobs=0
    for outputFile in ../output/fit*.out
    do
        lastLine=\$(tail -1 \$outputFile)
        if [[ \"\$lastLine\" == \"(int) 0\" ]]; then
            finishedJobs=\$((finishedJobs+1))
        fi
    done
echo \"\$finishedJobs completed out of \$nJobs\" >> \$fitOutFile
sleep 10
done
sbatch $asymSlurm
EOF

	touch $fitSlurm
	cat >> $fitSlurm <<EOF
#!/bin/bash
binname=\$1
#SBATCH --account=clas12
#SBATCH --partition=production
#SBATCH --mem-per-cpu=${memPerCPU}
#SBATCH --job-name=fit_$rungroup_$ana_$dir
#SBATCH --cpus-per-task=${nCPUs}
#SBATCH --time=24:00:00
clas12root ${rgaAfterMacro}\(\"${volatiledir}\",\"merged_${dir}\",\"\$binname\",2\)
EOF
	
	touch $asymSlurm
	cat >> $asymSlurm <<EOF
#!/bin/bash
binname=\$1
#SBATCH --account=clas12
#SBATCH --partition=production
#SBATCH --mem-per-cpu=${memPerCPU}
#SBATCH --job-name=asym_$rungroup_$ana_$dir
#SBATCH --cpus-per-task=${nCPUs}
#SBATCH --time=24:00:00
clas12root ${rgaAfterMacro}\(\"${volatiledir}\",\"\",\"\",3\)
EOF
    fi
fi



if [ $needsOrganize == 1 ]; then
    touch $organizeSlurm
    cat >> $organizeSlurm <<EOF
#!/bin/bash
#SBATCH --account=clas12
#SBATCH --partition=production
#SBATCH --mem-per-cpu=${memPerCPU}
#SBATCH --job-name=merge_$rungroup_$ana_$dir
#SBATCH --cpus-per-task=${nCPUs}
#SBATCH --time=24:00:00
#SBATCH --output=${outputSlurmDir}/merge-%x-%a.out
#SBATCH --error=${outputSlurmDir}/merge-%x-%a.err
$organizeFile
EOF
fi    

echo $hl
echo $hl
echo "DONE WITH FILE CREATION"
echo $hl
echo $hl
echo "Submitting processing slurm jobs"
ls "${shellSlurmDir}/run*.slurm" | xargs sbatch -q
n1=$(ls "${shellSlurmDir}/run*.slurm" | wc -l)
echo "Submitted ${n1} processing slurm jobs"

if [ $needsOrganize == 1 ]; then
    echo "Submitting organzing job"
    sbatch $organizeSlurm
fi

echo "----------------------------------------------------"
echo "Here is how you can monitor your runs..."
echo "----------------------------------------------------"
echo "Location of slurm .out/.err : $outputSlurmDir"
echo "Location of shell scripts : $shellSlurmDir"
echo "Location of saved .root files : $volatiledir"
echo "----------------------------------------------------"
