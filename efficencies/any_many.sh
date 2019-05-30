#!bin/bash
set -x
first=$2
last=$3

x=$first
while [[ ${x} -le $last ]]
do
        analyzer -b -l -q .x "$1.C(${x})"
        (( x++ ))
done
