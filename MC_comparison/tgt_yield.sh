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

if [[ $# -ge 7 ]]
then
        suf=$7
else
        suf="1"
fi
pwd=$PWD
if [ -e "/home/jbane/tritium/Tri_offline/MC_comparison/yield_output/tightcut/${bins}bins" ]
then
	echo "good"
else
	cd "/home/jbane/tritium/Tri_offline/MC_comparison/yield_output/tightcut/"
	mkdir -v ${bins}bins
	cd ${bins}bins
	mkdir xbj
	mkdir theta
	cd $pwd
fi

if [ "MCyield_test_C.so" -ot "MCyield_test.C" -o "MCYield_test_C.so" -ot "/home/jbane/headers/rootalias.h" -o "MCYield_test_C.so" -ot "/home/jbane/headers/inc1.h" -o "MCYield_test_C.so" -ot "/home/jbane/header/SQLanalysis.h" ]
then
	echo "make s0 file"
	analyzer -b -q -l .x "make_so.C"
fi


x=$strt
while [[ ${x} -le $end ]]
do
        analyzer -b -l -q .x "CY_preload.C(\"${tgt}\",\"$x\",\"$suf\",$bins,$clean,$model,3)"
        (( x++ ))
done
