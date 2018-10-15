#!/usr/bin/env bash

#avrdude -c usbasp -P com3 -p m328p -b 19200 -U flash:r:sdint.hex:r


avrdude -c usbasp -P com1 -p m328p -b 115200 -U flash:w:../bin/bt-ibutton-reader.hex
