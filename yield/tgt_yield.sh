#!bin/bash

tgt=$1
for x in 1 2 3 4 5
do
	analyzer -l -q " CalcYield.C++(\"${tgt}\",\"$x\",3)"
done



