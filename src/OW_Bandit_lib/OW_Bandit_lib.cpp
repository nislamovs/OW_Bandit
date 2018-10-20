#include <MAX17043.h>
#include <OneWire.h>
#include "OW_Bandit_lib.h"
#include "Arduino.h"

void OW_Bandit_lib::displayMenu() {
    Serial.println("#######################################");
    Serial.println("#-------Please select function:-------#");
    Serial.println("#######################################");
    Serial.println();
    Serial.println(" [0] - Check battery status");
    Serial.println(" [1] - Read iButton");
    Serial.println(" [2] - Read iButton and save in memory");
    Serial.println(" [3] - Get all recorded iButton codes");
    Serial.println(" [4] - Manual write iButton");
    Serial.println(" [5] - Write memory value to iButton");
    Serial.println(" [6] - Clone iButton");
    Serial.println(" [7] - Emulate iButton");
    Serial.println();
    Serial.println(" [h, H, m, M, ?] - Back to main menu");
    Serial.println("=======================================");
    Serial.println("Command could not be longer than 1 char");
    Serial.println("=======================================");

}

void OW_Bandit_lib::getBatteryStatus(MAX17043 batteryMonitor) {
    float cellVoltage = batteryMonitor.getVCell();
    float stateOfCharge = batteryMonitor.getSoC();

    Serial.print("Voltage:\t");
    Serial.print(cellVoltage, 4);
    Serial.print("V;\t");
    Serial.print("Charge:\t");
    Serial.print(stateOfCharge);
    Serial.println("%");
}

void OW_Bandit_lib::readIButton(OneWire ow) {

    Serial.println("Press 'M' to get back.");
    while (true) {

        if (Serial.available() > 0) {
            char inByte = Serial.read();
            if (inByte != 'M' && inByte != 'm') {
                Serial.println((String) "Invalid command [" + inByte + "]; Press 'M' to get back.");
            } else {
                return;
            }
        }

        while (true) {

            byte addr[8] = {0};

            if (!ow.search(addr)) {
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
        delay(400);
    }
}