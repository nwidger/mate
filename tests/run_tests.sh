#!/bin/bash

PWD=`pwd`
DIR=`basename $PWD`

if [ "$DIR" != "tests" ];
then
    cd tests
fi

rm -f *.s *~ *.sym *.class

i=1;
for e in *.m; do
    echo "===================="
    echo "$i. ${e%%.m}"
    echo "===================="

    # generate assembler file
    mcc -S $e
    # generate symbol table
    mcc -s $e
    # generate class file
    mcc $e

    if [[ $? -eq 0 ]];
    then
	mvm ${e%%.m}.class < input
    fi

    i=$(($i+1));
done
