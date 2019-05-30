#!bin/bash
set -x
first=$1

if [ $# -ge 2 ]
then
	last=$2
else
	last=$1
fi

x=$first
while [[ ${x} -le $last ]]
do
        analyzer -b -l -q .x "trigger_preload.C(${x})"
        (( x++ ))
done
