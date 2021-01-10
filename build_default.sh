#!/bin/bash

mkdir -p $PWD/tmp
mkdir -p $PWD/bin
pushd tmp
cmake .. -DALL=1 -DINSTALL_DIR=$PWD/../bin
make
make install
popd
