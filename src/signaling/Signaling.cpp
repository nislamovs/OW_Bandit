#include "Signaling.h"

Signaling::Signaling() {
    pinMode(VIBROMOTOR, OUTPUT);
}

void Signaling::statusOK() {
    if (DEBUG_MODE) { INFO(); }
    for (int i = 0; i < 2; i++) makeBeep(50, 1000);
}

void Signaling::statusError() {
    if (DEBUG_MODE) { INFO(); }
    makeBeep(700, 200);
}

void Signaling::devicePoweredUp() {
    if (DEBUG_MODE) { INFO(); }
    for (int i = 0; i < 3; i++) makeBeep(50, 1000);
}

void Signaling::deviceSetUp() {
    if (DEBUG_MODE) { INFO(); }
    makeBeep(600, 1000);
}


void Signaling::makeBeep(unsigned long duration, unsigned long freq) {
    if (DEBUG_MODE) { INFO(); }
    tone(BUZZER, freq);
    delay(duration);
    noTone(BUZZER);
    delay(duration);
}

void Signaling::enableVibro(unsigned long duration) {
    if (DEBUG_MODE) { INFO(); }
    digitalWrite(VIBROMOTOR, HIGH);
    delay(duration);
    digitalWrite(VIBROMOTOR, LOW);
    delay(duration);
}

void Signaling::shortVibro() {
    if (DEBUG_MODE) { INFO(); }
    digitalWrite(VIBROMOTOR, HIGH);
    delay(150);
    digitalWrite(VIBROMOTOR, LOW);
    delay(50);
}

void Signaling::enableBeacon() {
    if (DEBUG_MODE) { INFO(); }
    enableVibro(1000);
    for (int i = 0; i < 5; i++) makeBeep(50, 1000);
    makeBeep(1000, 1000);
}
