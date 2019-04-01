#!bin/bash
set -x
tgt=$1
if [[ $# -ge 2 ]]
then
	strt=$2
else
	strt=1
fi

if [[ $# -ge 3 ]]
then
	end=$3
else
	end=5
fi


if [[ $# -ge 4 ]]
then
	bins=$4
else
	bins=50

fi

if [ "CalcYield_test_C.so" -ot "CalcYield_test.C" ]
then
	echo "Makeing so file"
	analyzer -b -l -q "make_so.C"
fi

x=$strt
while [[ ${x} -le $end ]]
do
	analyzer -b -l -q .x "CY_preload.C(\"${tgt}\",\"$x\",$bins,3)"
	(( x++ ))
done
