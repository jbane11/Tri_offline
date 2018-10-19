#!bin/bash




tgt=$1



for x in 1 2 3 4 5 7 9 11 13 15
do

	analyzer -l -n MCyield.C"(\"${tgt}\",\"${x}\")"
done 

