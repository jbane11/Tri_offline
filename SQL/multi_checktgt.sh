#!bin/bash
#set -x

if [[ $# -gt 2 ]]
then
	for var in "$@"
	do
		analyzer -l -n -q 'checktgt.C("$var")'
	done
else

i=$1
end=$2
while [[ $i -le $end ]]
do
	analyzer -l -n -q "checktgt.C(${i})"
	((i++))
done
fi
