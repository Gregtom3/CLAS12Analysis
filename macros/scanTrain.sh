#!/bin/bash

clas12root -b -q '/work/clas12/users/gmat/CLAS12Analysis/macros/scanTrain.C('${1}',"'${2}'","'${4}'")'

mv "${2}-${1}-HELScaler-train.csv" "${3}/run${1}_HELScaler-train.csv"



