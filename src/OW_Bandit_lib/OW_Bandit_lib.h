#ifndef ARDUINOEXAMPLE_OW_BANDIT_LIB_H
#define ARDUINOEXAMPLE_OW_BANDIT_LIB_H

#include <inttypes.h>
//#include "OW_Bandit_lib.h"
#include "../OneWire/OneWire.h"
#include "../MAX17043/MAX17043.h"
#include "../OneWireSlave/OneWireSlave.h"
#include "../EE24C32/EE24C32.h"

#define IBUTTON_KEY_LENGTH 8
#define ONE_WIRE_HOST 12
#define ONE_WIRE_SLAVE 11
#define BUZZER 10
#define EEPROM_ADDRESS 0x50

class OW_Bandit_lib {


public:
    OW_Bandit_lib();
    void begin();
    void displayMenu();
    void getBatteryStatus();
    void readIButton(boolean saveToMemory);
    void emulateIButton();
    void soundBeacon();

private:
    static MAX17043 batteryMonitor;
    static OneWire ow;
    static OneWireSlave ows;
    static EE24C32 eeprom;
};

extern OW_Bandit_lib OW_BANDIT;

#endif
