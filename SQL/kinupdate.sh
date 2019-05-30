#!bin/bash

#rundir="/home/jbane/tritium/Runlist"
#alldir="/work/halla/triton/mnycz/Marathon/Analysis/Data_Quality/All_Runs"
#alldir="/w/halla-scifs17exp/triton/mnycz/Marathon/Analysis/Data_Quality/All_Runs"
rundir="./Runlist"



for f in $rundir/Carbon*kin*; do

echo " $f"

if echo $f | grep "~"
then
	continue
fi


	analyzer -b -l -q " SQL_kinupdate.C(\"${f}\")"


done
