#ifndef ARDUINOEXAMPLE_OW_BANDIT_H
#define ARDUINOEXAMPLE_OW_BANDIT_H

#define DEBUG_MODE false

#ifdef DEBUG_MODE
    #define INFO(...) Serial.println((String)"DEBUG : [ " + __FILE__ + " - " + __LINE__ + " - " + __func__ + " - " + __VA_ARGS__ + " ]")
#endif


#endif
