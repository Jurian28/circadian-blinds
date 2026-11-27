#include "config.h"
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel leds(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);

void setupLeds() {
  leds.begin();
  leds.setBrightness(80);
  leds.show();
}

void setLeds(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, leds.Color(r, g, b));
  }
  leds.show();
}
  