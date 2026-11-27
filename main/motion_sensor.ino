#include "config.h"

void setupMotionSensor() {
  pinMode(PIN_PIR, INPUT);
}

bool readMotion() {
  return digitalRead(PIN_PIR) == HIGH;
}
