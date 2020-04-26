#ifndef OW_BANDIT_SIGNALING_H
#define OW_BANDIT_SIGNALING_H

#include "../constants/constants.h"

class Signaling {

public:
    Signaling();
    void makeBeep(unsigned long duration, unsigned long freq);
    void enableVibro(unsigned long duration);
    void enableBeacon();
    void devicePoweredUp();
    void deviceSetUp();
    void statusOK();
    void statusError();
    void shortVibro();

private:


};


#endif //OW_BANDIT_SIGNALING_H
