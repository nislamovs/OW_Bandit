#!/usr/bin/env bash

./clean.sh

cd ../.. ;
cmake -H. -B./build;

cd build ;
make

cd .. ;
cp ./build/src/OW_Bandit.hex ./firmware/
