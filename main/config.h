#pragma once
#include <Arduino.h>

// Pins (GP numbers, same as the wiring guide)
constexpr int PIN_RAIN   = 27;
constexpr int PIN_PIR    = 16;
constexpr int PIN_LEDS   = 17;
constexpr int PIN_SERVO  = 18;
constexpr int PIN_BUTTON = 14;

// Light sensor (own I2C bus)
constexpr int     ALS_SDA  = 2;// ambient light sensor on its own I2C bus (Wire1)
constexpr int     ALS_SCL  = 3;
constexpr uint8_t ALS_ADDR = 0x39; // NOA1305 light sensor

constexpr uint8_t OLED_ADDR = 0x3C;

// Other settings
constexpr int NUM_LEDS = 10;
