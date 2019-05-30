#!bin/bash
set -x
first=$1
last=$2

x=$first
while [[ ${x} -le $last ]]
do
        analyzer -b -l -q .x "trigger_preload.C(${x})"
        (( x++ ))
done
