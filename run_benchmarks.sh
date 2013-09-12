#!/bin/bash

trap "{ exit 1; }" SIGINT SIGTERM

pushd tests/dmp

for class in radix.class jacobi.class dpl.class
do
    if [ "$class" == "radix.class" ];
    then
	input="ints-16bits-500.dat"
    elif [ "$class" == "jacobi.class" ];
    then
	input=""
    elif [ "$class" == "dpl.class" ];
    then
	input="dpl/uf20-91/uf20-01.cnf"
    fi

    cmd="time echo $threads | cat - $input | mvm $class"
    echo "RUNNING: $cmd"
    eval $cmd > "nondmp_${class}.log" 2>&1

    for threads in 2 4 8 16;
    do
	for quantum in 1000 10000 100000;
	do
	    q_arg="-Q $quantum"
	    
	    for serial_mode in full reduced;
	    do
		if [ "$serial_mode" == "reduced" ];
		then
		    r_arg=""
		else
		    r_arg="-r"
		fi
		
		for depth in 1 5 10;
		do
		    g_arg="-g$depth"

		    cmd="time echo $threads | cat - $input | mvm -p $q_arg $r_arg $g_arg $class"
		    echo "RUNNING: $cmd"
		    output="dmp_${class}_${quantum}_${serial_mode}_${depth}.log"
		    eval $cmd > $output 2>&1
		done
	    done
	done
    done
done

popd
