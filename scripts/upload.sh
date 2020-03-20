#!/usr/bin/env bash

cd ;
#cp Desktop/clion_projects/OW_Bandit/src/OW_Bandit.hex Desktop/clion_projects/OW_Bandit/firmware/
avrdude -v -v -v -c usbasp -P com0 -v -p atmega2560 -b 115200 -U flash:w:Desktop/Projects/clion_projects/OW_Bandit/firmware/OW_Bandit.hex:i #-Ulock:w:0x0F:m