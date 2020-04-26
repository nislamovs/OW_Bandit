#!/usr/bin/env bash

avrdude -v -patmega2560 -cwiring -P/dev/ttyUSB0 -b115200 -D -Uflash:w:../firmware/OW_Bandit.hex:i