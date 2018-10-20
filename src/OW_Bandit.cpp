#include "Arduino.h"
#include <util/delay.h>
#include "Wire.h"

#include "MAX17043/MAX17043.h"
#include "OneWire/OneWire.h"
#include "OW_Bandit_lib/OW_Bandit_lib.h"

MAX17043 batteryMonitor;
OneWire  ow(12); // digital pin 12
OW_Bandit_lib OWB;

void setup() {

    Wire.begin();
    Serial.begin(115200);

    batteryMonitor.reset();
    batteryMonitor.quickStart();
    delay(1000);

    OWB.displayMenu();
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
                OWB.displayMenu();
                break;

            case '0':
                Serial.println("Battery status:");
                OWB.getBatteryStatus(batteryMonitor);
                delay(1000);
                break;

            case '1':
                Serial.println("Read iButton:");
                OWB.readIButton(ow);
                break;

            default:
                Serial.println((String)"Unknown command: [" + inByte + "] ");
                break;
        }
        OWB.displayMenu();
    }
}