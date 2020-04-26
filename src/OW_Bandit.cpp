#include "OW_Bandit.h"

OW_Bandit_lib OW_BANDIT;

void setup() {
    if(DEBUG_MODE) Serial.println("Debug mode ON");
    Wire.begin();
    OW_BANDIT.begin();
    delay(1000);

    OW_BANDIT.displayMenu();
}

void loop() {

    if (Serial.available() > 0) {
        char inByte = Serial.read();
        switch (inByte) {
            case 'M':       //Menu
            case 'm':
            case 'H':       //Help
            case 'h':
            case '?':
                OW_BANDIT.displayMenu();
                break;

            case '0':
                Serial.println();
                Serial.println("System status:");
                OW_BANDIT.displaySystemStatus();
                break;

            case '1':
                Serial.println();
                Serial.println("Read iButton:");
                OW_BANDIT.readIButton(false, false);
                break;

            case '2':
                Serial.println();
                Serial.println("Read iButton and save [append]:");
                OW_BANDIT.readIButton(true, false);
                break;

            case '3':
                Serial.println();
                Serial.println("Read iButton and save [overwrite]:");
                OW_BANDIT.readIButton(true, true);
                break;

            case '4':
                Serial.println();
                Serial.println("Dump all iButton keys:");
                OW_BANDIT.dumpKeys();
                break;

            case '5':
                Serial.println();
                Serial.println("Emulate iButton [interactive]:");
                OW_BANDIT.emulateIButtonManual();
                break;

            case '6':
                Serial.println();
                Serial.println("Emulate iButton [from memory]:");
                OW_BANDIT.emulateIButtonMemory();
                break;

//TODO rewrite append and overwrite - should be one function for all

            case '7':
                Serial.println();
                Serial.println("Manual write to memory [append]:");
                OW_BANDIT.manualAddIButton(false);
                break;

            case '8':
                Serial.println();
                Serial.println("Manual write to memory [overwrite]:");
                OW_BANDIT.manualAddIButton(true);
                break;

            case '9':
                Serial.println();
                Serial.println("View memory content:");
                OW_BANDIT.showMemory();
                break;

            case 'A':
            case 'a':
                Serial.println();
                Serial.println("Sound beacon:");
                OW_BANDIT.soundBeacon();
                break;

            case 'B':
            case 'b':
                Serial.println();
                Serial.println("Clear memory:");
                OW_BANDIT.clearMemory();
                break;

            case 'C':
            case 'c':
                Serial.println();
                Serial.println("Calculate CRC for key:");
                OW_BANDIT.calculateCRC();
                break;

            case 'D':
            case 'd':
                Serial.println();
                Serial.println("Write iButton [interactive]:");
                OW_BANDIT.programIButtonManual();
                break;

            case 'E':
            case 'e':
                Serial.println();
                Serial.println("Write iButton [from memory]:");
                OW_BANDIT.programIButtonFromMemory();
                break;

            case 'F':
            case 'f':
                Serial.println();
                Serial.println("Clone iButton:");
                OW_BANDIT.cloneIButton();
                break;

            case 'G':
            case 'g':
                Serial.println();
                Serial.println("Identify key blank type");
                Serial.println("[RM1990.1, RM1990.2, TM2004]: ");
                OW_BANDIT.identifyKeyBlank();
                break;

            default:
                Serial.println();
                Serial.println((String)"Unknown command: [" + (String)inByte + "] ");
                break;
        }
        delay(1000);
        OW_BANDIT.displayMenu();
    }
}