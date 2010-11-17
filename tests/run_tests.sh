#!/bin/bash

rm -f *.s *~ *.sym *.class

i=1;
for e in *.m; do
    echo "===================="
    echo "$i. ${e%%.m}"
    echo "===================="

    cat predefined_classes.m $e | mc > ${e%%.m}.s
    if [[ $? -ne 0 ]];
    then
        rm -f ${e%%.m}.s
        continue
    fi

    mas -s < ${e%%.m}.s > ${e%%.m}.sym
    if [[ $? -ne 0 ]];
    then
        rm -f ${e%%.m}.sym
        continue
    fi

    mas < ${e%%.m}.s > ${e%%.m}.class
    if [[ $? -ne 0 ]];
    then
        rm -f ${e%%.m}.class
        continue
    fi

    mvm ${e%%.m}.class < input

    i=$(($i+1));
done
