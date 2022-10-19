import sys
import os
import glob
import rcdb
from rcdb.provider import RCDBProvider
from rcdb.model import ConditionType

import csv

filedir = str(sys.argv[1])
rootname = str(sys.argv[2])
version = ""
if(len(sys.argv)>3):
    version = "/" + str(sys.argv[3])

#for path in Path(filedir).glob("*.*"):
#    files.append(str(path))
tempfiles = os.listdir(filedir)
files = []
for tfile in tempfiles:
    try:
        temp = tfile.index(".root")
        files.append(tfile)
    except:
        continue
print(files)

db = RCDBProvider("mysql://rcdb@clasdb/rcdb")

cols = []
cnds = []
def add(str1,str2):
    cols.append(str1)
    cnds.append(str2)
add("beamE","beam_energy")
add("beamCurrent","beam_current")
add("Nevents","event_count")
add("eventRate","events_rate")
add("isValid","is_valid_run_end")
add("Tpol","target_polarization")
add("HWP","half_wave_plate")
add("Target","target")
cols.append("CookType")

df = csv.writer(open(filedir+'rcdb.csv','wb'), delimiter = ',')
df.writerow(['Run']+cols)
print(filedir+'rcdb.csv')

runNumberArr = []
HWPArr = []
targetArr = []

for file in files:
    # Get run number from .root file name
    start = len("run")
    end = file.index('.')
    runNumber = int(file[start:end])
    runNumberArr.append(int(runNumber))    
    # Get run from database
    run = db.get_run(runNumber)
    # Setup conditions from database
    vallist=[runNumber]
    for cnd in cnds:
        cnd_val = run.get_condition(cnd).value
        if(cnd == "target"):
            cnd_val=cnd_val.strip()
            if(cnd_val=="12C"):
                cnd_val="C"
        vallist.append(cnd_val)
        if(cnd == "half_wave_plate"):
            HWPArr.append(int(cnd_val))
        elif(cnd == "target"):
            targetArr.append(str(cnd_val))
    # Which Cooking type was used
    hipofiles = glob.glob('/volatile/clas12/rg-c/production/ana_data/*{}/dst/train/sidisdvcs/*{}.hipo'.format(version,runNumber))
    if(hipofiles==[]):
        vallist=vallist+['???']
    elif "TBT" in hipofiles[0]:
        vallist=vallist+['TBT']
    elif "HBT" in hipofiles[0]:
        vallist=vallist+['HBT']
    else:
        vallist=vallist+['???']

    # Add list to dataframe
    df.writerow(vallist)
    print("Run",runNumber,"added to csv")
    
'''targs = ['ND3','NH3','C']
hwps = ['hwp-in','hwp-out']
for _t in targs:
    for _h in hwps:
        path = filedir+_t+'-'+_h
        if(not os.path.exists(path)):
            os.mkdir(filedir+_t+'-'+_h)
        else:
            print("Exception: Directory",path,"already exists. Skipping...")

for _file,_target,_hwp,_runNumber in zip(files,targetArr,HWPArr,runNumberArr):
    # Default path
    path = filedir
    _file= path+_file
    # Find target type
    if(_target=="ND3"):
        path = path + "ND3"
    elif(_target=="NH3"):
        path = path + "NH3"
    elif(_target=="C" or _target=="12C"):
        path = path + "C"
    else:
        print("ERROR: Unknown target type",_target,"in file",_file,"...skipping...")
        continue
    # Find HWP param
    if(_hwp == 1):
        path = path + "-hwp-out"
    elif(_hwp == 0):
        path = path + "-hwp-in"
    else:
        print("ERROR: Unknown HWP param",_hwp,"in file",_file,"...skipping...")
        continue
    # Create new file name
    path = path + "/run-" + str(int(_runNumber)) + ".root"
    # Move file
    current = _file
    destination = path
    print(current)
    print(destination)
    print("---------------------------------------------------------------")
    if(os.path.exists(current)):
        os.rename(current,destination)
        print("Successfully moved file with run number",int(_runNumber))
    else:
        print("ERROR: File",current,"does not exist...skipping...")


'''

print("\n \n DONE \n \n")


