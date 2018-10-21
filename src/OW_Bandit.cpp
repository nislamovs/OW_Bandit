#include "Arduino.h"
#include <util/delay.h>
#include "Wire.h"

#include "MAX17043/MAX17043.h"
#include "OneWire/OneWire.h"
#include "OW_Bandit_lib/OW_Bandit_lib.h"
#include "OneWireSlave/OneWireSlave.h"

#define ONE_WIRE_HOST 12  // digital pin 12
#define ONE_WIRE_SLAVE 11
#define BUZZER 10


MAX17043 batteryMonitor;
OneWire ow(ONE_WIRE_HOST);
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
                break;

            case '1':
                Serial.println("Read iButton:");
                OWB.readIButton(ow);
                break;

            case '7':
                Serial.println("Emulate iButton: ");
                OWB.emulateIButton(ONE_WIRE_SLAVE);
                break;

            case 'A':
            case 'a':
                Serial.println("Sound beacon: ");
                OWB.soundBeacon(BUZZER);
                break;

            default:
                Serial.println((String)"Unknown command: [" + inByte + "] ");
                break;
        }
        delay(1000);
        OWB.displayMenu();
    }
}