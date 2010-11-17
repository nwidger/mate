#!/bin/bash

DOXYGEN=/Applications/Doxygen.app/Contents/Resources/doxygen

pushd doc
$DOXYGEN mc.doxygen
$DOXYGEN mas.doxygen
$DOXYGEN mvm.doxygen
popd
make clean
make distclean
autoreconf && ./configure && make && sudo make install && make dist && cp -v mate-1.0.tar.gz ~/Dropbox && ./send.sh mate-1.0.tar.gz
