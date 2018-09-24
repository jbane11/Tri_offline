#!bin/bash
#set -x

i=$1
end=$2
while [[ $i -le $end ]]
do
	analyzer -l -n -q "checktgt.C(${i})"
	((i++))
done
