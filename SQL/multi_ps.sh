#!bin/bash
#set -x

i=$1
end=$2
while [[ $i -le $end ]]
do
	#analyzer -l -n -q "PS_SQL.C(${i})"
	analyzer -l -n -q "SQL_PSupdate.C(${i})"
	((i++))
done
