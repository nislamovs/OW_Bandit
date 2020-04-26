#include "OW_Bandit_lib.h"

// Initialize Class Variables //////////////////////////////////////////////////
MAX17043 OW_Bandit_lib::batteryMonitor;
HC06 OW_Bandit_lib::btComm;
Signaling OW_Bandit_lib::signaling;
OneWire OW_Bandit_lib::ow(ONE_WIRE_HOST);
OneWireSlave OW_Bandit_lib::ows(ONE_WIRE_SLAVE);
EE24C32 OW_Bandit_lib::eeprom(EEPROM_I2C_ADDRESS);

int OW_Bandit_lib::usedMemory;
int OW_Bandit_lib::availableMemory;
int OW_Bandit_lib::totalMemory;
float OW_Bandit_lib::cellVoltage;
float OW_Bandit_lib::stateOfCharge;

OW_Bandit_lib::OW_Bandit_lib() {

}

void OW_Bandit_lib::begin() {
    if (DEBUG_MODE) { INFO(); }
    signaling.devicePoweredUp();

    setupBatteryMonitor();
    setupSerialComm();
    setupMemory();

    signaling.deviceSetUp();
}

void OW_Bandit_lib::displayMenu() {
    if (DEBUG_MODE) { INFO(); }
    Serial.println();
    Serial.println("##########################################");
    Serial.println("#--------Please select function:---------#");
    Serial.println("##########################################");
    Serial.println();
    Serial.println(" [0] - Check system status");
    Serial.println(" [1] - Read iButton");
    Serial.println(" [2] - Read iButton and save [append]");
    Serial.println(" [3] - Read iButton and save [overwrite]");
    Serial.println(" [4] - Dump all iButton keys");
    Serial.println(" [5] - Emulate iButton [manual enter]");
    Serial.println(" [6] - Emulate iButton [from memory]");
    Serial.println(" [7] - Manual write to memory [append]");
    Serial.println(" [8] - Manual write to memory [overwrite]");
    Serial.println(" [9] - View memory content");
    Serial.println(" [A, a] - Sound beacon");
    Serial.println(" [B, b] - Clear memory");
    Serial.println(" [C, c] - Calculate CRC for key");
    Serial.println(" [D, d] - Write iButton [interactive]");
    Serial.println(" [E, e] - Write iButton [from memory]");
    Serial.println(" [F, f] - Clone iButton");
    Serial.println(" [G, g] - Identify key blank type");
    Serial.println();
    Serial.println(" [h, H, m, M, ?] - Back to main menu");
    Serial.println("==========================================");
    Serial.println(" Command could not be longer than 1 char  ");
    Serial.println("==========================================");

}

void OW_Bandit_lib::displaySystemStatus() {
    if (DEBUG_MODE) { INFO(); }
    Serial.println();

    updateBatteryStatus();
    updateMemoryStatus();

    Serial.println("================Battery===================");
    Serial.print  ("Voltage:                          ");
    Serial.print  (cellVoltage, 4);
    Serial.print  ("V");

    Serial.println();

    Serial.print  ("Charge:                           ");
    Serial.print  (stateOfCharge);
    Serial.print  ("%");

    Serial.println();

    Serial.println();

    Serial.println("=================Keys=====================");
    Serial.print  ("Used space:                     ");
    Serial.print  (usedMemory);
    Serial.print  (" keys");

    Serial.println();

    Serial.print  ("Available space:               ");
    Serial.print  (availableMemory);
    Serial.print  (" keys");

    Serial.println();

    Serial.print  ("Total space:                    ");
    Serial.print  (totalMemory);
    Serial.print  (" keys");

    Serial.println();

    Serial.println();

    Serial.println("=============System boot log==============");
    Serial.println("Battery monitor status :              " + getModuleStatus(MAX17043_RESET_STATUS_CELL));
    Serial.println("Bluetooth comm status  :              " + getModuleStatus(HC_06_STATUS_CELL));
    Serial.println("Memory status          :              " + getModuleStatus(EXTERNAL_MEMORY_STATUS_CELL));
    Serial.println("Memory position cursor :              " + String(getCurrentMemPos()));
    Serial.println();
    Serial.println("Version                :      " + String(VERSION));
    delay(400);
}

void OW_Bandit_lib::showMemory() {
    if (DEBUG_MODE) { INFO(); }

    int pageNumber = 0;
    int pageSize = 16;
    int curPos =  getCurrentMemPos();

    if (curPos == 0) {
        Serial.println("Memory is empty! Exiting...");
        Serial.println();
        delay(400);
        return;
    } else {
        Serial.println("Press 'M' to get back, 'F' to next mem page, 'B' to prev mem page");
        Serial.println();
        displayMemValues(pageSize, pageNumber);

        while (true) {
            String inBytes = "";
            if (Serial.available() > 0) {
                inBytes = Serial.readString();

                if (inBytes.length() == 1) {
                    //Processing command:
                    if (inBytes.charAt(0) == 'M' || inBytes.charAt(0) == 'm') {
                        Serial.println("Exiting...");
                        return;
                    } else if (inBytes.charAt(0) == 'B' || inBytes.charAt(0) == 'b') {
                        if (pageNumber > 0) pageNumber--;
                        displayMemValues(pageSize, pageNumber);

                    } else if (inBytes.charAt(0) == 'F' || inBytes.charAt(0) == 'f') {
                        if (pageNumber < ((curPos / IBUTTON_KEY_LENGTH) / pageSize)) pageNumber++;
                        displayMemValues(pageSize, pageNumber);

                    } else {
                        Serial.println("Invalid command [" + inBytes + "]; Press 'M' to get back.");
                        Serial.println();
                    }

                } else {
                    //Processing invalid command:

                    Serial.println((String) "Invalid command [" + inBytes + "]; Press 'M' to get back.");
                }
            }
            delay(400);
        }
    }
}

void OW_Bandit_lib::programIButtonManual() {
    if (DEBUG_MODE) { INFO(); }

    Serial.println("Press 'M' to get back.");
    Serial.println();
    Serial.println("Please type iButton key You want to program:");
    while (true) {

        String inBytes = "";
        if (Serial.available() > 0) {
            inBytes = Serial.readString();
        }

        if (inBytes.length() == 1) {
            //Processing command:
            if (inBytes.charAt(0) != 'M' && inBytes.charAt(0) != 'm') {
                Serial.println("Invalid command [" + inBytes + "]; Press 'M' to get back.");
                Serial.println();
                Serial.println("Attach key blank to iButton socket:");
                Serial.println("Please type iButton key You want to program:");
            } else {
                Serial.println("Exiting...");
                return;
            }

        } else if (inBytes.length() == IBUTTON_KEY_LENGTH * 2) {
            //Processing key:
            Serial.println((String) "Programming key [" + inBytes + "] :");
            writeKey(hexstr_to_char(inBytes));

            //validation here:
            unsigned char actualKey[IBUTTON_KEY_LENGTH] = {0};

            if (!ow.search(actualKey)) {
                ow.reset_search();
                delay(200);
            }

            isEqualKeys(hexstr_to_char(inBytes), actualKey) ? Serial.print(" Done!") : Serial.print(" Error!");
            Serial.println();
            Serial.println();
            Serial.println("Press 'M' to get back.");
            Serial.println();
            Serial.println("Attach key blank to iButton socket:");
            Serial.println("Please type iButton key You want to program:");

        } else if (inBytes.length() > 1 && inBytes.length() != IBUTTON_KEY_LENGTH * 2) {
            //Processing invalid key:

            Serial.println((String) "Invalid key [" + inBytes + "]; Press 'M' to get back.");
        }
        delay(400);
    }
}

boolean OW_Bandit_lib::isEqualKeys(unsigned char *expectedKey, unsigned char *actualKey) {
    for(int n = 0; n < IBUTTON_KEY_LENGTH; n++) {
        if(expectedKey[n] != actualKey[n])
            return false;
    }

    return true;
}

void OW_Bandit_lib::writeKey(byte *data) {
    if (DEBUG_MODE) { INFO(); }

    ow.skip();              // send reset
    ow.reset();
    ow.write(0xD1);         // send 0xD1
    timeSlot(0);            // send logical 0

    ow.skip();
    ow.reset();
    ow.write(0xD5);

    for (byte x = 0; x < 8; x++) {
        for (int data_bit = 0; data_bit < 8; data_bit++) {
            data[x] & 1 ? timeSlot(0) : timeSlot(1);
            data[x] = data[x] >> 1;
        }
        Serial.print("##");
    }

    ow.reset();
    ow.write(0xD1);         // send 0xD1
    timeSlot(1);            // send logical 0

}

void OW_Bandit_lib::programIButtonFromMemory(){
    if (DEBUG_MODE) { INFO(); }

    int pageNumber = 0;
    int pageSize = 16;
    int curPos =  getCurrentMemPos();

    Serial.println("Please type memory cell number with key You want to program, [e.g. 005]:");
    Serial.println("Attach key blank to iButton socket:");
    Serial.println("Press 'M' to get back, 'F' to next mem page, 'B' to prev mem page");
    Serial.println();

    displayMemValues(pageSize, pageNumber);
    while(true) {
        String inBytes = "";
        if (Serial.available() > 0) {
            inBytes = Serial.readString();

            if (inBytes.length() == 1) {
                //Processing command:
                if (inBytes.charAt(0) == 'M' || inBytes.charAt(0) == 'm') {
                    Serial.println("Exiting...");
                    return;
                } else if (inBytes.charAt(0) == 'B' || inBytes.charAt(0) == 'b') {
                    if (pageNumber > 0) pageNumber--;
                    displayMemValues(pageSize, pageNumber);

                } else if (inBytes.charAt(0) == 'F' || inBytes.charAt(0) == 'f') {
                    if (pageNumber < ((curPos / IBUTTON_KEY_LENGTH) / pageSize)) pageNumber++;
                    displayMemValues(pageSize, pageNumber);

                } else {
                    Serial.println("Invalid command [" + inBytes + "]; Press 'M' to get back.");
                    Serial.println();
                }

            } else if (inBytes.length() == 3) {
                int cellNumber = 0;
                int curPos = getCurrentMemPos();

                if (!isDigitOnly(inBytes)) {
                    Serial.println((String) "Invalid cell number [" + inBytes + "]; Press 'M' to get back.");
                } else {
                    cellNumber = atoi(inBytes.c_str());
                    if (cellNumber < 0 || cellNumber >= (curPos / IBUTTON_KEY_LENGTH)) {
                        Serial.println((String) "Cell number [" + inBytes + "] is out of range; Press 'M' to get back.");
                    } else {
                        //Processing key:
                        String key = "";
                        for (int n = cellNumber * IBUTTON_KEY_LENGTH; n < (cellNumber + 1) * IBUTTON_KEY_LENGTH; n++) {
                            char buffer[2] = {0};
                            sprintf(buffer, "%02X", eeprom.read(n));

                            key = (String)(key + buffer);
                        }

                        Serial.print((String) "Programming key No. " + cellNumber + " [" + key + "] : ");
                        writeKey(hexstr_to_char(key));

                        //validation here:
                        unsigned char actualKey[IBUTTON_KEY_LENGTH] = {0};

                        if (!ow.search(actualKey)) {
                            ow.reset_search();
                            delay(200);
                        }

                        isEqualKeys(hexstr_to_char(key), actualKey) ? Serial.print(" Done!") : Serial.print(" Error!");
                        Serial.println();
                    }
                }

                Serial.println();
                Serial.println();
                Serial.println("Press 'M' to get back.");
                Serial.println();
                Serial.println("Attach key blank to iButton socket:");
                Serial.println("Please type memory cell number with key You want to program, [e.g. 005]:");

            } else if (inBytes.length() != 1 && inBytes.length() != 3) {
                //Processing invalid command:

                Serial.println((String) "Invalid command [" + inBytes + "]; Press 'M' to get back.");
            }
        }
        delay(400);
    }

}

void OW_Bandit_lib::cloneIButton(){

    if (DEBUG_MODE) { INFO(); }

    Serial.println("Press 'M' to get back.");
    Serial.println();
    Serial.println("Please attach iButton You want to clone:");
    while (true) {

        String inBytes = "";
        if (Serial.available() > 0) {
            inBytes = Serial.readString();
        }

        if (inBytes.length() == 1) {
            //Processing command:
            if (inBytes.charAt(0) != 'M' && inBytes.charAt(0) != 'm') {
                Serial.println("Invalid command [" + inBytes + "]; Press 'M' to get back.");
                Serial.println();
                Serial.println("Please attach iButton You want to clone:");
            } else {
                Serial.println("Exiting...");
                return;
            }
        }

        while (true) {

            byte key[IBUTTON_KEY_LENGTH] = {0};
            String newKey = "";

            if (!ow.search(key)) {
                ow.reset_search();
                delay(200);
                break;
            }

            if (OneWire::crc8(key, 7) != key[7]) {
                Serial.println();
                Serial.println("CRC is not valid!");
//                break;
            }

            Serial.println();

            for (int i = 0; i < IBUTTON_KEY_LENGTH; i++) {
                char buffer[2] = {0};
                sprintf(buffer, "%02X", key[i]);
                newKey = (String)(newKey + buffer);
            }
            Serial.println((String)"Recorded new key : [" + newKey + "]");
            ow.reset();
            programKey(newKey);
            Serial.println();
            Serial.println();
            Serial.println("Press 'M' to get back.");
            Serial.println();
            Serial.println("Please attach iButton You want to clone:");
            break;
        }
        delay(400);

    }
}

void OW_Bandit_lib::programKey(String newKey) {

    Serial.println();
    Serial.println("Please detach iButton and attach blank key:");
    unsigned char key[IBUTTON_KEY_LENGTH] = {0};
    delay(1500);
    //wait for detaching original key and attaching blank ibutton

    while (true) {

        if (Serial.available() > 0) {
            char inByte = Serial.read();
            if (inByte != 'M' && inByte != 'm') {
                Serial.println((String) "Invalid command [" + inByte + "]; Press 'M' to get back.");
                Serial.println();
                Serial.println("Please attach blank iButton :");
            } else {
                return;
            }
        }

        while (true) {

            unsigned char actualKey[IBUTTON_KEY_LENGTH] = {0};

            if (!ow.search(key)) {
                ow.reset_search();
                delay(200);
                break;
            }

            //Processing key:
            Serial.println((String) "Programming key [" + newKey + "] :  ");
            writeKey(hexstr_to_char(newKey));
            delay(200);
            //validation here:
            while (!ow.search(actualKey)) {
                ow.reset_search();
                delay(200);
            }

            isEqualKeys(hexstr_to_char(newKey), actualKey) ? Serial.print(" Done!") : Serial.print(" Error!");
            Serial.println();
            Serial.println();
            Serial.println("Press 'M' to get back.");
            Serial.println();
            Serial.println("Please attach next blank iButton :");
            break;

        }
        delay(1500);
    }
}

void OW_Bandit_lib::displayShortMemoryStatus() {
    if (DEBUG_MODE) { INFO(); }
    updateMemoryStatus();
    Serial.print((String)"   [" + usedMemory + "/" + totalMemory + "]");
}

int OW_Bandit_lib::identifyKeyBlank() {
    if (DEBUG_MODE) { INFO(); }
    Serial.print("Not implemented! Exiting...");
    delay(1000);
}

void OW_Bandit_lib::timeSlot(unsigned char data) {
    digitalWrite(ONE_WIRE_HOST, LOW);
    pinMode(ONE_WIRE_HOST, OUTPUT);
    data == 1 ? delayMicroseconds(6) : delayMicroseconds(60);
    pinMode(ONE_WIRE_HOST, INPUT);
    digitalWrite(ONE_WIRE_HOST, HIGH);
    delay(10);
}

void OW_Bandit_lib::dumpKeys() {
    if (DEBUG_MODE) { INFO(); }

    int curPos = getCurrentMemPos();
    int keyCount = curPos / IBUTTON_KEY_LENGTH;
    Serial.println((String)"About to dump " + keyCount + " keys... Proceed? y/n");

    while(true) {

        if (Serial.available() > 0) {
            char inByte = Serial.read();
            if (inByte == 'Y' || inByte == 'y') {
                Serial.println("Dumping...");

                displayMemValues(EE24C32_SIZE / IBUTTON_KEY_LENGTH, 0);
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

void OW_Bandit_lib::displayMemValues(int pageSize, int pageNumber) {
    if (DEBUG_MODE) { INFO(); }

    int curPos = getCurrentMemPos();
    int startPos = IBUTTON_KEY_LENGTH * pageSize * pageNumber;
    int endPos = ((startPos + pageSize * IBUTTON_KEY_LENGTH) > curPos) ? curPos : (startPos + pageSize * IBUTTON_KEY_LENGTH);

    for(int i = startPos; i < endPos; i++) {
        int curKey = i / IBUTTON_KEY_LENGTH;
        if (i % IBUTTON_KEY_LENGTH == 0) {
            Serial.println();
            Serial.print(" [");
            curKey < 10 ? Serial.print("00") : curKey < 100 ? Serial.print("0") : Serial.print("");
            Serial.print((String)curKey  + "]   ");
        }
        char buffer[2] = {0};
        sprintf(buffer, "%02X", eeprom.read(i));
        Serial.print(buffer);
    }
    Serial.println();
}

void OW_Bandit_lib::soundBeacon() {
    if (DEBUG_MODE) { INFO(); }
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
        signaling.enableBeacon();
    }
}

void OW_Bandit_lib::updateBatteryStatus() {
    if (DEBUG_MODE) { INFO(); }
    cellVoltage = batteryMonitor.getVCell();
    stateOfCharge = batteryMonitor.getSoC();
}

void OW_Bandit_lib::setupMemory() {
    if (DEBUG_MODE) { INFO(); }
    int status;
    EEPROM.get(EXTERNAL_MEMORY_STATUS_CELL, status);

    eeprom.begin(&Wire);

    if (status != OK) {
        for (int i = 0; i < EE24C32_SIZE; i++) {
            eeprom.write(i, 0xFF);
        }
        delay(10);
        EEPROM.put(MEMORY_ADDRESS_CELL, 0);
        delay(10);
        EEPROM.put(EXTERNAL_MEMORY_STATUS_CELL, OK);
        delay(10);
    }
    updateMemoryStatus();
}

void OW_Bandit_lib::setupSerialComm() {
    if (DEBUG_MODE) { INFO(); }
    btComm.preset();
    Serial.begin(115200);
}

String OW_Bandit_lib::getModuleStatus(int cellAddress) {
    if (DEBUG_MODE) { INFO(); }

    int status = NOK;
    EEPROM.get(cellAddress, status);

    return status == NOK ? "NOK" : "OK";
}

void OW_Bandit_lib::setupBatteryMonitor() {
    if (DEBUG_MODE) { INFO(); }
    batteryMonitor.reset();
    batteryMonitor.quickStart();
    updateBatteryStatus();
    EEPROM.put(MAX17043_RESET_STATUS_CELL, OK);
}

void OW_Bandit_lib::updateMemoryStatus() {
    if (DEBUG_MODE) { INFO(); }

    uint16_t used = getCurrentMemPos();
    usedMemory = used / IBUTTON_KEY_LENGTH;
    totalMemory = EE24C32_SIZE / IBUTTON_KEY_LENGTH;
    availableMemory = totalMemory - usedMemory;
}

void OW_Bandit_lib::emulateIButtonManual() {
    if (DEBUG_MODE) { INFO(); }

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

            ows.init(hexstr_to_char(inBytes));
            ows.waitForRequest(true);

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
    if (DEBUG_MODE) { INFO(); }

    int pageNumber = 0;
    int pageSize = 16;
    int curPos =  getCurrentMemPos();

    Serial.println("Please type memory cell number with key You want to emulate, [e.g. 005]:");
    Serial.println("Press 'M' to get back, 'F' to next mem page, 'B' to prev mem page");
    Serial.println();

    displayMemValues(pageSize, pageNumber);
    while(true) {
        String inBytes = "";
        if (Serial.available() > 0) {
            inBytes = Serial.readString();

            if (inBytes.length() == 1) {
                //Processing command:
                if (inBytes.charAt(0) == 'M' || inBytes.charAt(0) == 'm') {
                    Serial.println("Exiting...");
                    return;
                } else if (inBytes.charAt(0) == 'B' || inBytes.charAt(0) == 'b') {
                    if (pageNumber > 0) pageNumber--;
                    displayMemValues(pageSize, pageNumber);

                } else if (inBytes.charAt(0) == 'F' || inBytes.charAt(0) == 'f') {
                    if (pageNumber < ((curPos / IBUTTON_KEY_LENGTH) / pageSize)) pageNumber++;
                    displayMemValues(pageSize, pageNumber);

                } else {
                    Serial.println("Invalid command [" + inBytes + "]; Press 'M' to get back.");
                    Serial.println();
                }

            } else if (inBytes.length() == 3) {
                int cellNumber = 0;
                int curPos = getCurrentMemPos();

                if (!isDigitOnly(inBytes)) {
                    Serial.println((String) "Invalid cell number [" + inBytes + "]; Press 'M' to get back.");
                } else {
                    cellNumber = atoi(inBytes.c_str());
                    if (cellNumber < 0 || cellNumber >= (curPos / IBUTTON_KEY_LENGTH)) {
                        Serial.println((String) "Cell number [" + inBytes + "] is out of range; Press 'M' to get back.");
                    } else {
                        //Processing key:
                        String key = "";
                        for (int n = cellNumber * IBUTTON_KEY_LENGTH; n < (cellNumber + 1) * IBUTTON_KEY_LENGTH; n++) {
                            char buffer[2] = {0};
                            sprintf(buffer, "%02X", eeprom.read(n));

                            key = (String)(key + buffer);
                        }

                        Serial.println((String) "Emulating key No. " + cellNumber + " [" + key + "]...");
                        Serial.println("Press 'M' to get back.");
                        Serial.println();
                        ows.init(hexstr_to_char(key));
                        ows.waitForRequest(true);
                        Serial.print("Stopped.");
                    }
                }

                Serial.println();
                Serial.println("Please type memory cell number with key You want to emulate, [e.g. 005]:");
                Serial.println("Press 'M' to get back, 'F' to next mem page, 'B' to next mem page");
                Serial.println();

            } else if (inBytes.length() != 1 && inBytes.length() != 3) {
                //Processing invalid command:

                Serial.println((String) "Invalid command [" + inBytes + "]; Press 'M' to get back.");
            }
        }
        delay(400);
    }
}

void OW_Bandit_lib::calculateCRC() {
    if (DEBUG_MODE) { INFO(); }
    Serial.println("Press 'M' to get back.");
    Serial.println();
    Serial.println("Please type iButton key You want to calculate CRC for:");
    while(true) {
        String inBytes = "";
        String key = "";
        if (Serial.available() > 0) {
            inBytes = Serial.readString();
        }

        if (inBytes.length() == 1) {
            //Processing command:
            if (inBytes.charAt(0) != 'M' && inBytes.charAt(0) != 'm') {
                Serial.println();
                Serial.println("Invalid command [" + inBytes + "]; Press 'M' to get back.");
                Serial.println();
                Serial.println("Please type iButton key You want to calculate CRC for:");
            } else {
                Serial.println("Exiting...");
                return;
            }
        } else if (inBytes.length() > 1) {
            key = inBytes.substring(0, (IBUTTON_KEY_LENGTH - 1) * 2);

            if (isValidKey(key)) {
                //Processing key (WITH OR WITHOUT CRC):
                Serial.println();

                Serial.print((String) "Calculating CRC for key [" + key + "]... ");

                Serial.print("CRC : ");
                Serial.print(OneWire::crc8(hexstr_to_char(key), IBUTTON_KEY_LENGTH - 1), HEX);

                Serial.println();
                Serial.println("Press 'M' to get back.");
                Serial.println();
                Serial.println("Please type iButton key You want to calculate CRC for:");

            } else {
                //Processing invalid key:

                Serial.println((String) "Invalid key [" + inBytes + "]; Press 'M' to get back.");
            }
        }
        delay(400);
    }
}

boolean OW_Bandit_lib::isValidKey(String key) {
    if (DEBUG_MODE) { INFO(); }

    if (    key.length() != (IBUTTON_KEY_LENGTH - 2) * 2
         && key.length() != IBUTTON_KEY_LENGTH * 2 )
        return false;

    for (int n = 0; n < key.length(); n++) {
        if(     (key.charAt(n) >= '0' && key.charAt(n) <= '9')
            ||  (key.charAt(n) >= 'A' && key.charAt(n) <= 'F')
            ||  (key.charAt(n) >= 'a' && key.charAt(n) <= 'f')) {
            continue;
        } else {
            return false;
        }
    }

    return true;
}

void OW_Bandit_lib::manualAddIButton(boolean overwrite) {
    if (DEBUG_MODE) { INFO(); }

    Serial.println();
    Serial.println("Type key value (6 or 8 bytes).");
    Serial.println("Press 'M' to get back.");
    while(true) {
        String input = "";
        char key[IBUTTON_KEY_LENGTH + 1] = {0};
        if (Serial.available() > 0) {
            input = Serial.readString();
            if (input.length() == 1 && input.charAt(0) == 'M' || input.charAt(0) == 'm') {
                Serial.println("Exiting...");
                return;
            } else if (!isValidKey(input)) {
                Serial.println("Key length doesn't fit into requirements. Exiting...");
                return;
            }

            if (input.length() == (IBUTTON_KEY_LENGTH - 2) * 2) {                                   //short key
                strcpy(key, (char *)hexstr_to_char(DALLAS_IBUTTON_DEVICE_ID + input));
                Serial.println();
                Serial.print("Calculating CRC...");
                key[IBUTTON_KEY_LENGTH - 1] = OneWire::crc8((uint8_t *)key, IBUTTON_KEY_LENGTH - 1);
                Serial.print("Done!");
                Serial.println();
            } else {                                                                                // key with device id and crc
                strcpy(key, (char *)hexstr_to_char(input)) ;
            }

            if (overwrite) {
                updateMemoryStatus();
                if (usedMemory == 0) {
                    Serial.println();
                    Serial.println("You have no keys in memory. Switching to appending mode.");
                }
            }

            int address = usedMemory > 0 && overwrite ? getPreferedMemPos() : getCurrentMemPos();
            if (address == -1)
                return;

            if (usedMemory == 0 || !overwrite) {
                if (EE24C32_SIZE - address < 2) {
                    Serial.println("Memory is full.");
                    return;
                }
            }

            eeprom.write(address, (uint8_t *)key, IBUTTON_KEY_LENGTH);

            for (int i = address; i < address + IBUTTON_KEY_LENGTH; i++) {
                char buffer[2] = {0};
                sprintf(buffer, "%02X", eeprom.read(i));
                Serial.print(buffer);
            }

            if (usedMemory == 0 || !overwrite) {
                address += IBUTTON_KEY_LENGTH;
                EEPROM.put(MEMORY_ADDRESS_CELL, address);
                displayShortMemoryStatus();
            }

            Serial.println();
            Serial.println("Done! Exiting...");
            delay(400);
            return;
        }
    }

}

void OW_Bandit_lib::readIButton(boolean saveToMemory, boolean overwrite) {
    if (DEBUG_MODE) { INFO(); }

    Serial.println("Press 'M' to get back.");
    while (true) {

        //todo
        //put into separate func

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
                delay(200);
                break;
            }

            if (OneWire::crc8(key, 7) != key[7]) {
                Serial.println();
                Serial.println("CRC is not valid!");
//                break;
            }

//            if (key[0] != 0x01) {
//                Serial.println("Device is not a DS1990A family device.");

//                Uncomment this if You want to deal only with Dallas iButtons
//                break;
//            }

            if (saveToMemory && overwrite) {
                updateMemoryStatus();
                if (usedMemory == 0) {
                    Serial.println();
                    Serial.println("You have no keys in memory. Switching to appending mode.");
                }
            }

            Serial.println();

            for (int i = 0; i < IBUTTON_KEY_LENGTH; i++) {
                char buffer[2] = {0};
                sprintf(buffer, "%02X", key[i]);
                Serial.print(buffer);
            }

            if (saveToMemory) {

                int address = usedMemory > 0 && overwrite ? getPreferedMemPos() : getCurrentMemPos();

                if (address == -1)
                    return;

                if (usedMemory == 0 || !overwrite) {
                    if (EE24C32_SIZE - address < 2) {
                        Serial.println("Memory is full.");
                        break;
                    }
                }

                eeprom.write(address, key, IBUTTON_KEY_LENGTH);

                if (usedMemory == 0 || !overwrite) {
                    address += IBUTTON_KEY_LENGTH;

                    EEPROM.put(MEMORY_ADDRESS_CELL, address);
                    signaling.shortVibro();
                    displayShortMemoryStatus();
                }
            }

            ow.reset();

            if(overwrite) {
                Serial.println();
                Serial.println("Done! Exiting...");
                delay(400);
                return;
            }
        }
        delay(400);
    }
}

int OW_Bandit_lib::getPreferedMemPos() {
    if (DEBUG_MODE) { INFO(); }
    Serial.println();
    Serial.println((String)"Put memory cell number (0 - " + (usedMemory - 1) + "). Cell data will be overwritten.");
    while(true) {
        if (Serial.available() > 0) {
            String input = Serial.readString();
            if (input.length() == 1 && input.charAt(0) == 'M' || input.charAt(0) == 'm') {
                Serial.println("Exiting...");
                return -1;
            } else if (!isDigitOnly(input)) {
                Serial.println((String) "Invalid number [" + input + "]; Press 'M' to get back.");
            } else if (input.toInt() < 0 || input.toInt() >= usedMemory) {
                Serial.println((String) "Cell number [" + input + "] is out of range; Press 'M' to get back.");
            } else {
                return input.toInt() * IBUTTON_KEY_LENGTH;
            }
        }
    }
}

boolean OW_Bandit_lib::isDigitOnly(String strVal) {
    if (DEBUG_MODE) { INFO(); }
    for (int n = 0; n < strVal.length(); n++) {
        if (strVal.charAt(n) < '0' || strVal.charAt(n) > '9')
            return false;
    }

    return true;
}

void OW_Bandit_lib::clearMemory() {
    if (DEBUG_MODE) { INFO(); }
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
    if (DEBUG_MODE) { INFO(); }
    int address = 0;
    EEPROM.get(MEMORY_ADDRESS_CELL, address);

    return address;
}

unsigned char * OW_Bandit_lib::hexstr_to_char(String hexstr) {
    if (DEBUG_MODE) { INFO(); }
    unsigned char * result = (unsigned char *) malloc(sizeof(unsigned char) * IBUTTON_KEY_LENGTH + 1);
//    unsigned char result[IBUTTON_KEY_LENGTH+1] = {0};
    int length = hexstr.length();
//    if (length != IBUTTON_KEY_LENGTH * 2) {
//        Serial.println("Key length is incorrect.");
//        return NULL;
//    }
    for (int n = 0; n < length; n += 2) {
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

    result[length] = '\0';

    return result;
}

OW_Bandit_lib OW_Bandit = OW_Bandit_lib();