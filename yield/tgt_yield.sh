#!bin/bash
set -x
tgt=$1
if [[ $# -ge 2 ]]
then
	end=$2
else
	end=5
fi

x=1
while [[ ${x} -le $end ]]
do
	analyzer -b -l -q .x "CY_preload.C(\"${tgt}\",\"$x\",2,3)"
	(( x++ ))
done



