#!/bin/bash

if [[ $# -ne 1 ]];
then
    echo "usage: FILE"
    exit
fi

for host in agate.cs c0.cs gauss;
do
    scp $1 nau7@$host.unh.edu:
done
