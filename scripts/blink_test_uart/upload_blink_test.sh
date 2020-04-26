#!/usr/bin/env bash

avrdude -Cavrdude.conf -v -patmega2560 -cwiring -P/dev/ttyUSB0 -b115200 -D -Uflash:w:Blink.hex:i
