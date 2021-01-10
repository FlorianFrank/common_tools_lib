#!/bin/bash

mkdir -p ./tmp/armhf
mkdir -p ./bin/armhf
pushd tmp/armhf
cmake ../.. -DALL=1 -DINSTALL_DIR="$PWD/../../bin/armhf" -DCMAKE_C_COMPILER=arm-linux-gnueabihf-gcc -DCMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++
make
make install
popd
