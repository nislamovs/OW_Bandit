#!/usr/bin/env bash

avrdude -v -v -v -c usbasp -P com0 -v -p atmega2560 -b 115200 -U flash:w:Blink.hex:i #-Ulock:w:0x0F:m
