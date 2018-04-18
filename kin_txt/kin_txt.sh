array=($(ls  ../Runlist/*kin$1.dat)) 

for i in "${array[@]}"
do
	analyzer -b .x 'kin_cor_txt_multi.C++("'$i'",0)' >> kin_err.txt
done
