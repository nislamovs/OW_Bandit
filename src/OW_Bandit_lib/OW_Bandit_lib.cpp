#include "Arduino.h"
#include "OW_Bandit_lib.h"

// Initialize Class Variables //////////////////////////////////////////////////
MAX17043 OW_Bandit_lib::batteryMonitor;
OneWire OW_Bandit_lib::ow(ONE_WIRE_HOST);
OneWireSlave OW_Bandit_lib::ows(ONE_WIRE_SLAVE);
EE24C32 OW_Bandit_lib::eeprom(EEPROM_ADDRESS);
int OW_Bandit_lib::usedMemory;
int OW_Bandit_lib::availableMemory;
int OW_Bandit_lib::totalMemory;
float OW_Bandit_lib::cellVoltage;
float OW_Bandit_lib::stateOfCharge;

// Constructors ////////////////////////////////////////////////////////////////
OW_Bandit_lib::OW_Bandit_lib()
{
}

void OW_Bandit_lib::begin() {
    batteryMonitor.reset();
    batteryMonitor.quickStart();
    eeprom.begin(&Wire);
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
    Serial.println(" [B, b] - Clear memory");
    Serial.println();
    Serial.println(" [h, H, m, M, ?] - Back to main menu");
    Serial.println("=======================================");
    Serial.println("Command could not be longer than 1 char");
    Serial.println("=======================================");

}

void OW_Bandit_lib::displaySystemStatus() {

    updateBatteryStatus();
    updateMemoryStatus();

    Serial.println("==============Battery==================");
    Serial.print("Voltage:\t");
    Serial.print(cellVoltage, 4);
    Serial.print("V;\t");
    Serial.print("Charge:\t");
    Serial.print(stateOfCharge);
    Serial.println("%");
    Serial.println();

    Serial.println("===============Keys====================");
    Serial.print("Used space:\t");
    Serial.print(usedMemory);
    Serial.println(" keys;\t");
    Serial.print("Available space:\t");
    Serial.print(availableMemory);
    Serial.println(" keys;\t");
    Serial.print("Total space:\t");
    Serial.print(totalMemory);
    Serial.println(" keys;\t");
    Serial.println();
}

void OW_Bandit_lib::displayShortMemoryStatus() {
    updateMemoryStatus();
    Serial.print((String)"   [" + usedMemory + "/" + totalMemory + "]");
}

void OW_Bandit_lib::makeBeep(unsigned long duration, unsigned long freq) {
    tone(BUZZER, freq);
    delay(duration);
    noTone(BUZZER);
    delay(duration);
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

        for (int i = 0; i < 3; i++) makeBeep(250, 1000);
        makeBeep(1000, 1000);
    }
}

void OW_Bandit_lib::updateBatteryStatus() {
    cellVoltage = batteryMonitor.getVCell();
    stateOfCharge = batteryMonitor.getSoC();
}

void OW_Bandit_lib::updateMemoryStatus() {
    int used = 0;
    EEPROM.get(MEMORY_ADDRESS_CELL, used);
    usedMemory = used / IBUTTON_KEY_LENGTH;

    totalMemory = EE24C32_SIZE / IBUTTON_KEY_LENGTH;
    availableMemory = totalMemory - usedMemory;
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

            byte key[IBUTTON_KEY_LENGTH] = {0};

            if (!ow.search(key)) {
                ow.reset_search();
                break;
            }

            if (OneWire::crc8(key, 7) != key[7]) {
                Serial.println("CRC is not valid!");
                break;
            }

            if (key[0] != 0x01) {
                Serial.println("Device is not a DS1990A family device.");

//                Uncomment this if You want to deal only with Dallas iButtons
//                break;
            }

            for (int i = 0; i < IBUTTON_KEY_LENGTH; i++) {
                char buffer[2];
                sprintf(buffer, "%02X", key[i]);
                Serial.print(buffer);
            }


            if (saveToMemory) {
                int address = 0;
                EEPROM.get(MEMORY_ADDRESS_CELL, address);
//                Serial.println();                                      //
//                Serial.println((String)"memAddr before: " + address);  //

                eeprom.write(address, key, IBUTTON_KEY_LENGTH);
                address += IBUTTON_KEY_LENGTH;

//                Serial.println((String)"memAddr after: " + address);  //
//                Serial.println();                                     //

                EEPROM.update(MEMORY_ADDRESS_CELL, address);
                displayShortMemoryStatus();
            }
            Serial.println();
            ow.reset();
        }
        delay(400);
    }
}

void OW_Bandit_lib::clearMemory() {
    Serial.println("About to clear memory... Proceed? y/n");
    while(true) {

        if (Serial.available() > 0) {
            char inByte = Serial.read();
            if (inByte == 'Y' || inByte == 'y') {
                Serial.println("Cleaning up...");
                for(int i = 0; i < EE24C32_SIZE; i++) {
                    eeprom.write(i, 0xFF);
                    if (i % 128 == 0)
                        Serial.print("#");
                }
                Serial.println();
                EEPROM.put(MEMORY_ADDRESS_CELL, 0);
                Serial.println("Memory cleared!");

                return;

            } else if (inByte == 'N' || inByte == 'n' || inByte == 'M' || inByte == 'm') {
                return;
            } else {
                Serial.println((String) "Invalid command [" + inByte + "]; Press 'M' to get back.");
            }
        }
    }
}

OW_Bandit_lib OW_Bandit = OW_Bandit_lib();