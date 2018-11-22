#!/usr/bin/env bash

cd ;
cp -rf Desktop/clion_projects/OW_Bandit/build/src/OW_Bandit.hex Desktop/clion_projects/OW_Bandit/firmware/
avrdude -c usbasp -P com0 -p m328p -b 115200 -U flash:w:Desktop/clion_projects/OW_Bandit/build/src/OW_Bandit.hex
