#ifndef OW_BANDIT_LIB_H
#define OW_BANDIT_LIB_H

#include <inttypes.h>
#include "../OneWire/OneWire.h"
#include "../MAX17043/MAX17043.h"
#include "../OneWireSlave/OneWireSlave.h"
#include "../EE24C32/EE24C32.h"
#include "../EEPROM/src/EEPROM.h"
#include "../OW_Bandit.h"

#define IBUTTON_KEY_LENGTH 8
#define ONE_WIRE_HOST 12
#define ONE_WIRE_SLAVE 11
#define BUZZER 10
#define EEPROM_ADDRESS 0x50
#define MEMORY_ADDRESS_CELL 0    //cell, where we store first free eeprom cell address for storing iButton keys

#define TYPE_KEY_DS1990     0
#define TYPE_KEY_RW1990_1   1
#define TYPE_KEY_RW1990_2   2
#define TYPE_KEY_TM2004     3
#define TYPE_KEY_UNKNOWN    4

class OW_Bandit_lib {

    public:
        OW_Bandit_lib();
        void begin();
        void displayMenu();
        void displaySystemStatus();
        void readIButton(boolean saveToMemory, boolean overwrite);
        void soundBeacon();
        void clearMemory();
        void dumpKeys();
        void emulateIButtonManual();
        void emulateIButtonMemory();
        void calculateCRC();
        void manualAddIButton(boolean overwrite);
        void showMemory();
        void programIButtonManual();
        void programIButtonFromMemory();
        void cloneIButton();
        int identifyKeyBlank();

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
        int getPreferedMemPos();
        unsigned char* hexstr_to_char(String hexstr);
        boolean isValidKey(String key);
        boolean isDigitOnly(String strVal);
        void displayMemValues(int pageSize, int pageNumber);

        void writeKey(byte *data);
        void timeSlot(unsigned char data);
        boolean isEqualKeys(unsigned char *expectedKey, unsigned char *actualKey);
        void programKey(String newKey);
};

extern OW_Bandit_lib OW_BANDIT;

#endif
