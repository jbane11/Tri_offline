
i=$1
end=$2
tgt=$3
if [ $# -ge 5 ]
then
  ver=$4
else
  ver=""
fi

if [ $# -eq 5 ]
then
  delta=$5
else
  delta=1
fi

if [ $ver -eq 0 ]
then
  ver=""
fi

analyzer -l -n -q .L "Eff_scan_cal.C++"

while [[ $i -le $end ]]
do
	#analyzer -l -n -q "PS_SQL.C(${i})"

	analyzer -l -n -q .x "preload_cal_scan.C(\"${i}\",\"${tgt}\",\"${ver}\",${delta})"
	((i++))
done
