#include "config.h"

void setupButton() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
}

bool buttonPressed() {
  return digitalRead(PIN_BUTTON) == LOW;
}
