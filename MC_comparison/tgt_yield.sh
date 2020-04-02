#!bin/bash
#set -x

usage()
{
    echo "usage: sysinfo_page [[[-t tgt ] [-s S suf]] | [-h]]"
}



if [ "$1" == "-a" ]
then
	shift
	while [ "$1" != "" ]; do
    	case $1 in
        	-t | -tgt ) 					shift
                                tgt=$1
                                ;;
        	-s | -S | -suf )      shift
                                suf=$1
                                ;;
        	-strt | -start )      shift
                                strt=$1
                                ;;
					-end | -e )           shift
																end=$1
																;;
					-debug | -db )        shift
																db=$1
																;;
				-B | -b | -bin | -Bin ) shift
                                bins=$1
                                ;;
				-clean | -c )        shift
															clean=$1
														;;
				-model | -m )        shift
															model=$1
														;;
          -h | --help )         usage
                                exit
                                ;;
         * )                     usage
                                exit 1
    esac
    shift
	done
else



tgt=$1
if [[ "$1" = "-h" ]] || [[ $1 = "-help" ]] || [[ $1 = "h" ]] || [[ $1 = "help" ]] || [[ $# -eq 0 ]]
then
	echo "::::::Help::::::::"
	echo "arguments are  tgt, start, end, bins, clean, model, and suf"
	exit
fi


if [[ $# -ge 2 ]]
then
        strt=$2
else
        strt=1
fi

if [[ $# -ge 3 ]]
then
        end=$3
else
        end=5
fi

if [[ $# -ge 4 ]]
then
        bins=$4
else
        bins=50

fi

if [[ $# -ge 5 ]]
then
        clean=$5
else
        clean=0
fi
if [[ $# -ge 6 ]]
then
        model=$6
else
        model=0
fi

if [[ $# -ge 7 ]]
then
        suf=$7
else
        suf="1"
fi

fi
pwd=$PWD
#exit
cuttype="tightcut/"

if [ -e "/home/jbane/tritium/Tri_offline/MC_comparison/yield_output/$cuttype" ]
then
  echo "Good on cut dir"
else
  cd "/home/jbane/tritium/Tri_offline/MC_comparison/yield_output/"
  mkdir -v $cuttype
  cd $pwd
fi

if [ -e "/home/jbane/tritium/Tri_offline/MC_comparison/yield_output/$cuttype/${bins}bins" ]
then
	echo "good"
else
	cd "/home/jbane/tritium/Tri_offline/MC_comparison/yield_output/$cuttype/"
	mkdir -v ${bins}bins
	cd ${bins}bins
	mkdir xbj
	mkdir theta
	cd $pwd
fi

mcdir="/home/jbane/tritium/Tri_offline/MC_comparison/"
sofile="/home/jbane/tritium/Tri_offline/MC_comparison/MCyield_test_C.so"

if [ "${sofile}" -ot "${mcdir}/MCyield_test.C" -o \
		 "${sofile}" -ot "/home/jbane/headers/rootalias.h" -o \
		 "${sofile}" -ot "/home/jbane/headers/inc1.h" -o \
		 "${sofile}" -ot "/home/jbane/headers/SQLanalysis.h" ]
then
	echo
	echo "make s0 file"
	echo
	analyzer -b -q -l .x "make_so.C"
fi
#set -x
#exit
x=$strt
while [[ ${x} -le $end ]]
do
        analyzer -b -l -q .x "${mcdir}CY_preload.C(\"${tgt}\",\"$x\",\"$suf\",$bins,$clean,$model,$db)"
        (( x++ ))
done
