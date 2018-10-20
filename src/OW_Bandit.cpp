#include "Arduino.h"
#include <util/delay.h>


#include "MAX17043/MAX17043.h"
#include "OneWire/OneWire.h"
#include "Wire.h"

MAX17043 batteryMonitor;
OneWire  ow(12); // digital pin 12

void setup() {

    Wire.begin();
    Serial.begin(9600);

    Serial.println("MAX17043 Example: reading voltage and SoC");
    Serial.println();

    batteryMonitor.reset();
    batteryMonitor.quickStart();
    delay(1000);

    float cellVoltage = batteryMonitor.getVCell();
    Serial.print("Voltage:\t\t");
    Serial.print(cellVoltage, 4);
    Serial.println("V");

    float stateOfCharge = batteryMonitor.getSoC();
    Serial.print("State of charge:\t");
    Serial.print(stateOfCharge);
    Serial.println("%");
}

void loop() {
    while(true) {
        byte addr[8] = {0};

        if (!ow.search(addr)) {
//                serial.println("No more addresses.");
            ow.reset_search();
            break;
        }

        for (int i = 0; i < 8; i++) {
            char buffer[2];
            sprintf(buffer, "%02X", addr[i]);
            Serial.print(buffer);
        }
        Serial.println("");

        if (OneWire::crc8(addr, 7) != addr[7]) {
            Serial.println("CRC is not valid!");
            break;
        }

        if (addr[0] != 0x01) {
            Serial.println("Device is not a DS1990A family device.");
            break;
        }
        ow.reset();
    }
    _delay_ms(600);

}