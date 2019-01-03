i=$1
end=$2
if [[ $# -ge 3 ]]
then
  debug=$3
else
  debug=0
fi

while [[ $i -le $end ]]
do
	analyzer -l -n -q "preload_beaminfo.C(${i},${debug})"
	((i++))
done
