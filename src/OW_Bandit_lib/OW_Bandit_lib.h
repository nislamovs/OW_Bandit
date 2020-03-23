#ifndef OW_BANDIT_LIB_H
#define OW_BANDIT_LIB_H

#include "../OW_Bandit.h"

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

        void makeBeep(unsigned long duration, unsigned long freq);

    private:
        static HC06 btComm;
        static MAX17043 batteryMonitor;
        static OneWire ow;
        static OneWireSlave ows;
        static EE24C32 eeprom;
        static int usedMemory;
        static int availableMemory;
        static int totalMemory;
        static float cellVoltage;
        static float stateOfCharge;

        void setupSerialComm();
        void updateMemoryStatus();
        void updateBatteryStatus();
        void displayShortMemoryStatus();

        void enableVibro(unsigned long duration, unsigned long freq);
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
