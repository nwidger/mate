#!/bin/bash

rm -rf mate-1.0
tar xfz mate-1.0.tar.gz
cd mate-1.0
./configure --prefix=~nau7/bin
make
make install
