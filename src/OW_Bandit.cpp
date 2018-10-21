#include "Arduino.h"
#include <util/delay.h>
#include "Wire.h"

#include "MAX17043/MAX17043.h"
#include "OneWire/OneWire.h"
#include "OneWireSlave/OneWireSlave.h"
#include "EE24C32/EE24C32.h"
#include "OW_Bandit_lib/OW_Bandit_lib.h"

//#define EEPROM_ADDRESS 0x50

//MAX17043 batteryMonitor;
//OneWire ow(ONE_WIRE_HOST);
OW_Bandit_lib OW_BANDIT;
//EE24C32 eeprom(EEPROM_ADDRESS);

void setup() {

    Wire.begin();
    Serial.begin(115200);
    OW_BANDIT.begin();
    delay(1000);

    OW_BANDIT.displayMenu();
}

void loop() {

    // read from the serial input
    if (Serial.available() > 0) {
//         get a command
        char inByte = Serial.read();
        switch (inByte) {
            case 'M':
            case 'm':
            case 'H':
            case 'h':
            case '?':
                OW_BANDIT.displayMenu();
                break;

            case '0':
                Serial.println("Battery status:");
                OW_BANDIT.getBatteryStatus();
                break;

            case '1':
                Serial.println("Read iButton:");
                OW_BANDIT.readIButton(false);
                break;

            case '2':
                Serial.println("Read iButton and save to memory:");
                OW_BANDIT.readIButton(true);
                break;

            case '7':
                Serial.println("Emulate iButton: ");
                OW_BANDIT.emulateIButton();
                break;

            case 'A':
            case 'a':
                Serial.println("Sound beacon: ");
                OW_BANDIT.soundBeacon();
                break;

            default:
                Serial.println((String)"Unknown command: [" + inByte + "] ");
                break;
        }
        delay(1000);
        OW_BANDIT.displayMenu();
    }
}