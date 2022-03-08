#ifndef OW_BANDIT_CONSTANTS_H
#define OW_BANDIT_CONSTANTS_H

#include <Arduino.h>
#include <util/delay.h>
#include "Wire/Wire.h"

#include "../EEPROM/src/EEPROM.h"
#include "../signaling/Signaling.h"

#define VERSION "ver. 1.0.0"

#define VIBROMOTOR 9
#define ONE_WIRE_HOST 12
#define ONE_WIRE_SLAVE 11
#define BUZZER 10

#define HC_06_KEY 5
#define HC_06_POWER 2

#define IBUTTON_KEY_LENGTH 8
#define KEY_EEPROM_I2C_ADDRESS 0x50
#define JOURNAL_EEPROM_I2C_ADDRESS 0x52

#define OK 0xAA
#define NOK 0xDD

#define DEVICE_NAME "OW_BANDIT"
#define NEW_LINE "\r\n"
#define DALLAS_IBUTTON_DEVICE_ID "01"

#define MAX17043_RESET_STATUS_CELL               0x00      //cell, where we store info about MAX17043 battery gauge reset status
//AA - OK
//DD - NOK

#define HC_06_STATUS_CELL                        0x02      //cell, where we store info about HC-06 bluetooth module settings
//AA - OK
//DD - NOK

#define EXTERNAL_KEY_MEMORY_STATUS_CELL          0x04      //cell, where we store info about 24C32 external key memory reset status [KEYS]
//AA - OK
//DD - NOK

#define MEMORY_ADDRESS_CELL                      0x06      //cell, where we store first free eeprom cell address for storing iButton keys

#define EXTERNAL_JOURNAL_MEMORY_STATUS_CELL      0x08      //cell, where we store info about AT24CM01 external journal memory reset status [JOURNAL]
//AA - OK
//DD - NOK

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
