#include "config.h"

void setupRainSensor() {
  pinMode(PIN_RAIN, INPUT);
  analogReadResolution(12);
}

int readRain() {
  int percent = map(analogRead(PIN_RAIN), 0, 4095, 0, 100);
  return percent;
}
