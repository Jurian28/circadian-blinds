#include "config.h"

void setupServo() {
  analogWriteFreq(50); // servo: 50 Hz, 1 tick = 1 us
  analogWriteRange(20000);
}

void writeServo(int angle) { // 500 us = 0 deg, 2500 us = 180 deg
  analogWrite(PIN_SERVO, map(angle, 0, 180, 500, 2500));
}
