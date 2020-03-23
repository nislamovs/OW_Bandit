#ifndef _HC06_H
#define _HC06_H

#include "../constants/constants.h"

class HC06 {

public:
    HC06();
    void preset();

private:

    void switchToATMode();
    void switchToPairingMode();
    void setBaudrate();
    void powerOn();
    void powerOff();
    void restart();
    void setKeyPin();
    void clearKeyPin();
};

#endif