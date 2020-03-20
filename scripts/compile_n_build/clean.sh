#!/usr/bin/env bash

cd ../.. ;
rm -rf CMakeFiles ;

cd src ;
rm -rf CMakeFiles ;
rm -rf Makefile ;
rm -rf cmake_install.cmake ;
rm -rf CMakeCache.txt ;
rm -rf *.eep *.elf *.hex *.a ;

cd .. ;
rm -rf Makefile ;
rm -rf cmake_install.cmake ;
rm -rf CMakeCache.txt ;

rm -rf build ;
mkdir build ;