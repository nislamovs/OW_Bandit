#include "HC06.h"

HC06::HC06() {
    if (DEBUG_MODE) { INFO(); }
    pinMode(HC_06_POWER, OUTPUT);
    pinMode(HC_06_KEY, OUTPUT);
}

void HC06::preset() {
    if (DEBUG_MODE) { INFO(); }

    int status;
    EEPROM.get(HC_06_STATUS_CELL, status);
    if (status != OK) {
        switchToATMode();
        setBaudrate();
        setName(DEVICE_NAME);
        EEPROM.put(HC_06_STATUS_CELL, OK);
        delay(10);
    }

    switchToPairingMode();
}

void HC06::switchToATMode() {
    if (DEBUG_MODE) { INFO(); }

    powerOff();
    delay(100);
    setKeyPin();
    delay(10);
    powerOn();
    delay(3000);
};

void HC06::switchToPairingMode(){
    if (DEBUG_MODE) { INFO(); }

    powerOff();
    delay(100);
    clearKeyPin();
    delay(10);
    powerOn();
    delay(300);
};

void HC06::setName(String newName){
    if (DEBUG_MODE) { INFO(); }

    Serial.begin(38400);

    Serial.print("AT+NAME=");
    Serial.print(newName);
    Serial.print(NEW_LINE);
    delay(100);
    Serial.print("AT+NAME?");
    Serial.print(NEW_LINE);
    delay(100);

    delay(200);
    Serial.end();
};

void HC06::setBaudrate(){
    if (DEBUG_MODE) { INFO(); }

    Serial.begin(38400);

    Serial.print("AT+UART=115200,0,0");
    Serial.print(NEW_LINE);
    delay(100);
    Serial.print("AT+UART?");
    Serial.print(NEW_LINE);
    delay(100);

    delay(200);
    Serial.end();
};

void HC06::clearKeyPin(){
    if (DEBUG_MODE) { INFO(); }

    //necessary to switch to pairing mode
    digitalWrite(HC_06_KEY, LOW);
}

void HC06::setKeyPin(){
    if (DEBUG_MODE) { INFO(); }

    //necessary to switch to AT mode
    digitalWrite(HC_06_KEY, HIGH);
}

void HC06::restart(){
    if (DEBUG_MODE) { INFO(); }

    powerOff();
    delay(100);
    powerOn();
}

void HC06::powerOn(){
    if (DEBUG_MODE) { INFO(); }
    digitalWrite(HC_06_POWER, HIGH);
};

void HC06::powerOff(){
    if (DEBUG_MODE) { INFO(); }
    digitalWrite(HC_06_POWER, LOW);
};