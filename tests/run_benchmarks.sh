#!/bin/bash

trap "{ rm -f deleteme.log; set +x; exit 1; }" SIGINT SIGTERM
pushd dmp

rm -f results.log

echo "#+STARTUP: showall"                                                  >> results.log
echo "#+STARTUP: hidestars"                                                >> results.log
echo ""                                                                    >> results.log
echo "| dmp    | class  | threads | quantum | serial  | depth |   time |"  >> results.log
echo "|--------+--------+---------+---------+---------+-------+--------|"  >> results.log

for class in radix jacobi dpl
do
    if [ "$class" == "radix" ];
    then
	input="ints-16bits-500.dat"
    elif [ "$class" == "jacobi" ];
    then
	input=""
    elif [ "$class" == "dpl" ];
    then
	input="dpl/uf20-91/uf20-01.cnf"
    fi

    for threads in 2 4 8 16;
    do
	echo $threads > deleteme.log

	if [ "$input" != "" ];
	then
	    cat $input >> deleteme.log
	fi

	output="nondmp_${class}_${threads}.log"
	set -x
	/usr/bin/time -p mvm ${class}.class < deleteme.log &> "$output"
	set +x

	t=$(grep real "$output" | awk '{ print $2 }')
	printf "| %-6s | %-6s | %7s | %7s | %-7s | %5s | %6s |\n" \
	    nondmp $class $threads x x x $t >> results.log

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

		    output="dmp_${class}_${threads}_${quantum}_${serial_mode}_${depth}.log"
		    set -x
		    /usr/bin/time -p mvm -Dp $q_arg $r_arg $g_arg ${class}.class < deleteme.log &> "$output"
		    set +x

		    t=$(grep real "$output" | awk '{ print $2 }')
		    printf "| %-6s | %-6s | %7s | %7s | %-7s | %5s | %6s |\n" \
			dmp $class $threads $quantum $serial_mode $depth $t >> results.log

		done
	    done
	done
    done
done

rm -f deleteme.log

popd

