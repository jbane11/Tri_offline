#!bin/bash
set -x
first=$1
last=$2

x=$first
while [[ ${x} -le $last ]]
do
        analyzer -b -l -q .x "PID_SQL_preload.C(${x})"
        (( x++ ))
done
