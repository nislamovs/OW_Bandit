#include "OW_Bandit.h"

OW_Bandit_lib OW_BANDIT;

void setup() {
    if(DEBUG_MODE) {
        Serial.begin(115200);
        Serial.println("Debug mode ON");
    }
    Wire.begin();
    OW_BANDIT.begin();
    delay(1000);

    OW_BANDIT.displayMenu();
}

void loop() {

    if (Serial1.available() > 0) {
        char inByte = Serial1.read();
        switch (inByte) {
            case 'M':       //Menu
            case 'm':
            case 'H':       //Help
            case 'h':
            case '?':
                OW_BANDIT.displayMenu();
                break;

            case '0':
                Serial1.println();
                Serial1.println("System status:");
                OW_BANDIT.displaySystemStatus();
                break;

            case '1':
                Serial1.println();
                Serial1.println("Read iButton:");
                OW_BANDIT.readIButton(false, false);
                break;

            case '2':
                Serial1.println();
                Serial1.println("Read iButton and save [append]:");
                OW_BANDIT.readIButton(true, false);
                break;

            case '3':
                Serial1.println();
                Serial1.println("Read iButton and save [overwrite]:");
                OW_BANDIT.readIButton(true, true);
                break;

            case '4':
                Serial1.println();
                Serial1.println("Dump all iButton keys:");
                OW_BANDIT.dumpKeys();
                break;

            case '5':
                Serial1.println();
                Serial1.println("Emulate iButton [interactive]:");
                OW_BANDIT.emulateIButtonManual();
                break;

            case '6':
                Serial1.println();
                Serial1.println("Emulate iButton [from memory]:");
                OW_BANDIT.emulateIButtonMemory();
                break;

//TODO rewrite append and overwrite - should be one function for all

            case '7':
                Serial1.println();
                Serial1.println("Manual write to memory [append]:");
                OW_BANDIT.manualAddIButton(false);
                break;

            case '8':
                Serial1.println();
                Serial1.println("Manual write to memory [overwrite]:");
                OW_BANDIT.manualAddIButton(true);
                break;

            case '9':
                Serial1.println();
                Serial1.println("View memory content:");
                OW_BANDIT.showMemory();
                break;

            case 'A':
            case 'a':
                Serial1.println();
                Serial1.println("Sound beacon:");
                OW_BANDIT.soundBeacon();
                break;

            case 'B':
            case 'b':
                Serial1.println();
                Serial1.println("Clear memory:");
                OW_BANDIT.clearMemory();
                break;

            case 'C':
            case 'c':
                Serial1.println();
                Serial1.println("Calculate CRC for key:");
                OW_BANDIT.calculateCRC();
                break;

            case 'D':
            case 'd':
                Serial1.println();
                Serial1.println("Write iButton [interactive]:");
                OW_BANDIT.programIButtonManual();
                break;

            case 'E':
            case 'e':
                Serial1.println();
                Serial1.println("Write iButton [from memory]:");
                OW_BANDIT.programIButtonFromMemory();
                break;

            case 'F':
            case 'f':
                Serial1.println();
                Serial1.println("Clone iButton:");
                OW_BANDIT.cloneIButton();
                break;

            case 'G':
            case 'g':
                Serial1.println();
                Serial1.println("Identify key blank type");
                Serial1.println("[RM1990.1, RM1990.2, TM2004]: ");
                OW_BANDIT.identifyKeyBlank();
                break;

            case 'T':
            case 't':
                Serial1.println();
                Serial1.println("Testing New Memory");
                OW_BANDIT.testNewMem();
                break;

            default:
                Serial1.println();
                Serial1.println((String)"Unknown command: [" + (String)inByte + "] ");
                break;
        }
        delay(1000);
        OW_BANDIT.displayMenu();
    }
}