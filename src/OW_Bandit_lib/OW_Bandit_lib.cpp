#include "Arduino.h"
//#include <MAX17043.h>
//#include <OneWire.h>
//#include <OneWireSlave.h>
//#include <EE24C32.h>

#include "OW_Bandit_lib.h"

// Initialize Class Variables //////////////////////////////////////////////////
MAX17043 OW_Bandit_lib::batteryMonitor;
OneWire OW_Bandit_lib::ow(ONE_WIRE_HOST);
OneWireSlave OW_Bandit_lib::ows(ONE_WIRE_SLAVE);
EE24C32 OW_Bandit_lib::eeprom(EEPROM_ADDRESS);

// Constructors ////////////////////////////////////////////////////////////////
OW_Bandit_lib::OW_Bandit_lib()
{
}

void OW_Bandit_lib::begin() {
    batteryMonitor.reset();
    batteryMonitor.quickStart();
}

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
    Serial.println(" [8] - Manual write to memory");
    Serial.println(" [9] - Read memory values");
    Serial.println(" [A, a] - Sound beacon");
    Serial.println();
    Serial.println(" [h, H, m, M, ?] - Back to main menu");
    Serial.println("=======================================");
    Serial.println("Command could not be longer than 1 char");
    Serial.println("=======================================");

}

void OW_Bandit_lib::soundBeacon() {
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
        for (int i = 0; i < 3; i++) {
            tone(BUZZER, 1000);
            delay(250);
            noTone(BUZZER);
            delay(250);
        }
        tone(BUZZER, 1000);
        delay(1000);
        noTone(BUZZER);
        delay(1000);
    }
}

void OW_Bandit_lib::getBatteryStatus() {
    float cellVoltage = batteryMonitor.getVCell();
    float stateOfCharge = batteryMonitor.getSoC();

    Serial.print("Voltage:\t");
    Serial.print(cellVoltage, 4);
    Serial.print("V;\t");
    Serial.print("Charge:\t");
    Serial.print(stateOfCharge);
    Serial.println("%");
}

void OW_Bandit_lib::emulateIButton() {
    Serial.println("Press 'M' to get back.");
    
    unsigned char rom[IBUTTON_KEY_LENGTH] = {0x28, 0xAD, 0xDA, 0xCE, 0x0F, 0x00, 0x11, 0x00};

    ows.init(rom);
    ows.waitForRequest(false);
}

void OW_Bandit_lib::readIButton(boolean saveToMemory) {

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

            byte addr[IBUTTON_KEY_LENGTH] = {0};

            if (!ow.search(addr)) {
                ow.reset_search();
                break;
            }

            if (OneWire::crc8(addr, 7) != addr[7]) {
                Serial.println("CRC is not valid!");
                break;
            }

            if (addr[0] != 0x01) {
                Serial.println("Device is not a DS1990A family device.");
                break;
            }

            for (int i = 0; i < IBUTTON_KEY_LENGTH; i++) {
                char buffer[2];
                sprintf(buffer, "%02X", addr[i]);
                Serial.print(buffer);
            }

//            if (saveToMemory) {
//                eeprom.writeBytes(0x00, IBUTTON_KEY_LENGTH, addr);
//            }

            Serial.println("");
            ow.reset();
        }
        delay(250);
    }
}

OW_Bandit_lib OW_Bandit = OW_Bandit_lib();