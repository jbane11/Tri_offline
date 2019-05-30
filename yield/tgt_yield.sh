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
	suf=$5
else
	suf="1"
fi

if [[ $# -ge 6 ]]
then
	db=$6
else
	db=3
fi
pwd=$PWD
if [ -e "/home/jbane/tritium/Tri_offline/yield/yield_output/tightcut/${bins}bins" ]
then
	echo "good"
else
	cd "/home/jbane/tritium/Tri_offline/yield/yield_output/tightcut/"
	mkdir -v ${bins}bins
	cd ${bins}bins
	mkdir xbj
	mkdir theta
	cd $pwd
fi
if [ "CalcYield_test_C.so" -ot "CalcYield_test.C" -o "CalcYield_test_C.so" -ot "/home/jbane/headers/rootalias.h" -o "CalcYield_test_C.so" -ot "/home/jbane/headers/inc1.h" -o "CalcYield_test_C.so" -ot "/home/jbane/header/SQLanalysis.h" ]
then
	echo "Makeing so file"
	analyzer -b -l -q "make_so.C"
fi

x=$strt
while [[ ${x} -le $end ]]
do
	analyzer -b -l -q .x "CY_preload.C(\"${tgt}\",\"$x\",$bins,\"$suf\",$db)"
	(( x++ ))
done
