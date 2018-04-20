array=($(ls  ../Runlist/*kin$1.dat)) 

for i in "${array[@]}"
do
	analyzerl -b .x 'kin_cor_txt_multi.C++("'$i'",1,1)' >> err_$i.txt
done
