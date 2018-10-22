#ifndef ARDUINOEXAMPLE_OW_BANDIT_LIB_H
#define ARDUINOEXAMPLE_OW_BANDIT_LIB_H

#include <inttypes.h>
#include "../OneWire/OneWire.h"
#include "../MAX17043/MAX17043.h"
#include "../OneWireSlave/OneWireSlave.h"
#include "../EE24C32/EE24C32.h"
#include "../EEPROM/src/EEPROM.h"

#define IBUTTON_KEY_LENGTH 8
#define ONE_WIRE_HOST 12
#define ONE_WIRE_SLAVE 11
#define BUZZER 10
#define EEPROM_ADDRESS 0x50
#define MEMORY_ADDRESS_CELL 0    //cell, where we store first free eeprom cell address for storing iButton keys

class OW_Bandit_lib {

    public:
        OW_Bandit_lib();
        void begin();
        void displayMenu();
        void displaySystemStatus();
        void readIButton(boolean saveToMemory);
        void emulateIButton();
        void soundBeacon();
        void clearMemory();
        void dumpKeys();

    private:
        static MAX17043 batteryMonitor;
        static OneWire ow;
        static OneWireSlave ows;
        static EE24C32 eeprom;
        static int usedMemory;
        static int availableMemory;
        static int totalMemory;
        static float cellVoltage;
        static float stateOfCharge;

        void updateMemoryStatus();
        void updateBatteryStatus();
        void displayShortMemoryStatus();
        void makeBeep(unsigned long duration, unsigned long freq);
        int getCurrentMemPos();
};

extern OW_Bandit_lib OW_BANDIT;

#endif
