#!/bin/bash

mkdir -p tmp
mkdir -p bin
pushd tmp
cmake .. -DALL=1 -DINSTALL_DIR=../bin
make
make install
popd
