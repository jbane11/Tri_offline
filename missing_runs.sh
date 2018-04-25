!/bin/sh



cd Runlist/
array=($(ls *kin*.dat)) 
cd ..
j=0
for i in "${array[@]}"
do
	file=${array[$j]}
	echo " $i ${i} "${i}" ${file}"
	analyzer -q -l -n .x ' check_list.C("'${file}'") '
	j=$j+1
done
