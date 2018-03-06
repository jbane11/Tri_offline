#!/bin/bash

if [ $# -eq 0 ] 
then
	echo "Please input runlist"
	read runlist
else
	runlist=$1
fi

inline=(foo foo foo)
i=0
input="/adaqfs/home/a-onl/tritium_work/Runlist/$runlist"
while IFS= read -r var
do
	inline[$i]=$var
	i=$((i+1))
	echo "$i   $var"
done < "$input"

echo
runstring=${inline[2]}
kin=${inline[0]}"_kin"${inline[1]}
echo $kin
echo
IFS=',' read -a runarray <<< "$runstring"

for run in ${runarray[@]}
do

	if [ -e /chafs1/work1/tritium/Rootfiles/bqual/beam_qual_$run.root ]; then
		echo "$run is complete."

	else
		analyzer -b 'good_beam_sh.C++('$run',"'$kin'")'	
	fi
done
