#!/bin/bash

clas12root -b -q 'scanRecon.C('${1}',"'${2}'")'

mv "${2}-${1}-HELScaler.csv" "${3}/run${1}_HELScaler.csv"
mv "${2}-${1}-RUNScaler.csv" "${3}/run${1}_RUNScaler.csv"

clas12root -b -q 'scanReconBig.C('${1}',"'${2}'")'

mv "${2}-${1}-HELScaler-all.csv" "${3}/run${1}_HELScaler-all.csv"
mv "${2}-${1}-RUNScaler-all.csv" "${3}/run${1}_RUNScaler-all.csv"


