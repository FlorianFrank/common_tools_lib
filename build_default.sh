#!/bin/bash

mkdir -p tmp
pushd tmp
cmake .. -DALL=1
make
popd
