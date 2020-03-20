#!/usr/bin/env bash


avrdude -C./avrdude.conf -v -patmega2560 -cusbasp -Pusb -e -Ulock:w:0x3F:m -Ulfuse:w:0xFF:m -Uhfuse:w:0xD9:m -Uefuse:w:0xFD:m
#avrdude -C./avrdude.conf -v -patmega2560 -cusbasp -Pusb -e -Ulock:w:0x3F:m -Ulfuse:w:0xFF:m -Uhfuse:w:0xD8:m -Uefuse:w:0xFD:m

avrdude -C./avrdude.conf -v -patmega2560 -cusbasp -Pusb -Uflash:w:./stk500boot_v2_mega2560.hex:i -Ulock:w:0x0F:m
