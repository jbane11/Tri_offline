#!bin/bash
#set -x
if [[ $# -gt 2 ]]
then
        for var in "$@"
        do
                analyzer -l -n -q "beamcharge.C++($var,0,1)"
        done
else

i=$1
end=$2
while [[ $i -le $end ]]
do
	#analyzer -l -n -q "PS_SQL.C(${i})"
	analyzer -l -n -q "beamcharge.C(${i},0,1)"
	((i++))
done
fi

