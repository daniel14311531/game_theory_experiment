#!/bin/bash

mkdir -p build && cd build
cmake ..
make
cd ..
cp build/experiment ./experiment