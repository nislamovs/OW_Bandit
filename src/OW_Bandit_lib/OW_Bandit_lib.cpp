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
{}

void OW_Bandit_lib::begin() {
    batteryMonitor.reset();
    batteryMonitor.quickStart();
    eeprom.begin(&Wire);
    updateBatteryStatus();
    updateMemoryStatus();
}

void OW_Bandit_lib::displayMenu() {
    Serial.println("#######################################");
    Serial.println("#-------Please select function:-------#");
    Serial.println("#######################################");
    Serial.println();
    Serial.println(" [0] - Check battery status");
    Serial.println(" [1] - Read iButton");
    Serial.println(" [2] - Read iButton and save in memory");
    Serial.println(" [3] - Dump all iButton codes");
    Serial.println(" [4] - Emulate iButton (manual enter)");
    Serial.println(" [5] - Emulate iButton (from memory)");
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

    Serial.println();

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

void OW_Bandit_lib::dumpKeys() {

    int curPos = getCurrentMemPos();
    int keyCount = curPos / IBUTTON_KEY_LENGTH;
    Serial.println((String)"About to dump " + keyCount + " keys... Proceed? y/n");

    while(true) {

        if (Serial.available() > 0) {
            char inByte = Serial.read();
            if (inByte == 'Y' || inByte == 'y') {
                Serial.println("Dumping...");

                for(int i = 0; i < curPos; i++) {
                    int curKey = i / IBUTTON_KEY_LENGTH;
                    if (i % IBUTTON_KEY_LENGTH == 0) {
                        Serial.println();
                        Serial.print(" [");
                        curKey < 10 ? Serial.print("00") : curKey < 100 ? Serial.print("0") : Serial.print("");
                        Serial.print((String)curKey  + "]   ");
                    }
                    char buffer[2];
                    sprintf(buffer, "%02X", eeprom.read(i));
                    Serial.print(buffer);
                }
                Serial.println((String)"\n\nAll " + keyCount + " keys dumped successfully!\n");

                return;

            } else if (inByte == 'N' || inByte == 'n' || inByte == 'M' || inByte == 'm') {
                return;
            } else {
                Serial.println((String) "Invalid command [" + inByte + "]; Press 'M' to get back.");
            }
        }
    }
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
    uint16_t used = getCurrentMemPos();
//    Serial.println((String)"Debug used >>>" + used);
    usedMemory = used / IBUTTON_KEY_LENGTH;
//    Serial.println((String)"Debug used >>>" + usedMemory);

    totalMemory = EE24C32_SIZE / IBUTTON_KEY_LENGTH;
    availableMemory = totalMemory - usedMemory;
}

void OW_Bandit_lib::emulateIButtonManual() {

    Serial.println("Press 'M' to get back.");
    Serial.println();
    Serial.println("Please type iButton key You want to emulate:");
    while(true) {
        String inBytes = "";
        if (Serial.available() > 0) {
            inBytes = Serial.readString();
        }

        if (inBytes.length() == 1) {
            //Processing command:
            if (inBytes.charAt(0) != 'M' && inBytes.charAt(0) != 'm') {
                Serial.println("Invalid command [" + inBytes + "]; Press 'M' to get back.");
                Serial.println();
                Serial.println("Please type iButton key You want to emulate:");
            } else {
                Serial.println("Exiting...");
                return;
            }

        } else if (inBytes.length() == IBUTTON_KEY_LENGTH * 2) {
            //Processing key:
            Serial.println((String) "Emulating key [" + inBytes + "]...");

            for (int i = 0; i < IBUTTON_KEY_LENGTH * 2; i++) {
                char buffer[2];
                sprintf(buffer, "%02X", inBytes.charAt(i));
            }

            ows.init(hexstr_to_char(inBytes));
            ows.waitForRequest(false);

            Serial.print("Stopped.");
            Serial.println();
            Serial.println();
            Serial.println("Press 'M' to get back.");
            Serial.println();
            Serial.println("Please type iButton key You want to emulate:");

        } else if (inBytes.length() > 1 && inBytes.length() != IBUTTON_KEY_LENGTH * 2) {
            //Processing invalid key:

            Serial.println((String) "Invalid key [" + inBytes + "]; Press 'M' to get back.");
        }
        delay(400);
    }
}

void OW_Bandit_lib::emulateIButtonMemory() {
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
            Serial.println();

            if (saveToMemory) {
                int address = getCurrentMemPos();
                if(EE24C32_SIZE - address < 2) {
                    Serial.println("Memory is full.");
                    break;
                }

//                Serial.println();                                      //
//                Serial.println((String)"memAddr before: " + address);  //

                eeprom.write(address, key, IBUTTON_KEY_LENGTH);
                address += IBUTTON_KEY_LENGTH;

//                Serial.println((String)"memAddr after: " + address);  //
//                Serial.println();                                     //

                EEPROM.put(MEMORY_ADDRESS_CELL, address);
                displayShortMemoryStatus();
            }

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

int OW_Bandit_lib::getCurrentMemPos() {
    int address = 0;
    EEPROM.get(MEMORY_ADDRESS_CELL, address);

    return address;
}

unsigned char * OW_Bandit_lib::hexstr_to_char(String hexstr) {

    unsigned char result[IBUTTON_KEY_LENGTH+1] = {0};
    int length = hexstr.length();
    if (length != IBUTTON_KEY_LENGTH * 2) {
        Serial.println("Key length is incorrect.");
        return NULL;
    }
    for (int n = 0; n < IBUTTON_KEY_LENGTH * 2; n += 2) {
        char buffer = 0x00;

        //First halfbyte
        if(hexstr.charAt(n) >= '0' && hexstr.charAt(n) <= '9')
            buffer = (hexstr.charAt(n) - '0') << 4;
        if(hexstr.charAt(n) >= 'A' && hexstr.charAt(n) <= 'F')
            buffer = (hexstr.charAt(n) - 'A' + 10) << 4;
        if(hexstr.charAt(n) >= 'a' && hexstr.charAt(n) <= 'f')
            buffer = (hexstr.charAt(n) - 'a' + 10) << 4;

        result[n/2] = buffer;

        //Second halfbyte
        if(hexstr.charAt(n+1) >= '0' && hexstr.charAt(n+1) <= '9')
            buffer = (hexstr.charAt(n+1) - '0');
        if(hexstr.charAt(n+1) >= 'A' && hexstr.charAt(n+1) <= 'F')
            buffer = (hexstr.charAt(n+1) - 'A' + 10);
        if(hexstr.charAt(n+1) >= 'a' && hexstr.charAt(n+1) <= 'f')
            buffer = (hexstr.charAt(n+1) - 'a' + 10);

        result[n/2] += buffer;
    }

    return result;
}

OW_Bandit_lib OW_Bandit = OW_Bandit_lib();