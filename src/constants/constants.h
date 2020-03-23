#ifndef OW_BANDIT_CONSTANTS_H
#define OW_BANDIT_CONSTANTS_H

#include <Arduino.h>
#include <util/delay.h>
#include "Wire/Wire.h"

#include "../EEPROM/src/EEPROM.h"

#define VIBROMOTOR 13
#define ONE_WIRE_HOST 12
#define ONE_WIRE_SLAVE 11
#define BUZZER 10

#define HC_06_KEY 5
#define HC_06_POWER 2

#define IBUTTON_KEY_LENGTH 8
#define EEPROM_I2C_ADDRESS 0x50

#define OK 0xAA
#define NOK 0xDD


#define MAX17043_RESET_STATUS_CELL       0x00            //cell, where we store info about MAX17043 battery gauge reset status
//AA - OK
//DD - NOK

#define HC_06_STATUS_CELL                0x01            //cell, where we store info about HC-06 bluetooth module settings
//AA - OK
//DD - NOK

#define EXTERNAL_MEMORY_STATUS_CELL      0x02            //cell, where we store info about 24C32 external memory reset status
//AA - OK
//DD - NOK

#define MEMORY_ADDRESS_CELL              0x03            //cell, where we store first free eeprom cell address for storing iButton keys

#define TYPE_KEY_DS1990     0
#define TYPE_KEY_RW1990_1   1
#define TYPE_KEY_RW1990_2   2
#define TYPE_KEY_TM2004     3
#define TYPE_KEY_UNKNOWN    4

#define DEBUG_MODE false

#ifdef DEBUG_MODE
#define INFO(...) Serial.println((String)"DEBUG : [ " + __FILE__ + " - " + __LINE__ + " - " + __func__ + " - " + __VA_ARGS__ + " ]")
#endif

#endif //OW_BANDIT_CONSTANTS_H
