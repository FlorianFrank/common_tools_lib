#!/bin/bash

mkdir -p ./tmp/amd64
mkdir -p ./bin/amd64
pushd tmp/amd64
cmake ../.. -DALL=1 -DINSTALL_DIR="$PWD/../../bin/amd64"
make
make install
popd
