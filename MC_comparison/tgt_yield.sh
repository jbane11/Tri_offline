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

if [[ $# -ge 5 ]]
then
        clean=$5
else
        clean=0
fi
if [[ $# -ge 6 ]]
then
        model=$6
else
        model=0
fi

if [ "MCyield_test_C.so" -ot "MCyield_test.C" ]
then
	echo "make s0 file"
	analyzer -b -q -l .x "make_so.C"
fi


x=$strt
while [[ ${x} -le $end ]]
do
        analyzer -b -l -q .x "CY_preload.C(\"${tgt}\",\"$x\",$bins,$clean,$model,0,0,3)"
        (( x++ ))
done
