#ifndef ARDUINOEXAMPLE_OW_BANDIT_LIB_H
#define ARDUINOEXAMPLE_OW_BANDIT_LIB_H

#include <inttypes.h>
#include "OW_Bandit_lib.h"

class OW_Bandit_lib {

    private:

    public:
//        OW_Bandit_lib();

        void displayMenu();

        void getBatteryStatus(MAX17043 batteryMonitor);

        void readIButton(OneWire ow);

};


#endif
