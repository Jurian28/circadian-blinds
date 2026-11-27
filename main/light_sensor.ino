#include "config.h"
#include "Wire.h"

void alsWrite(uint8_t reg, uint8_t value) {
  Wire1.beginTransmission(ALS_ADDR);
  Wire1.write(reg);
  Wire1.write(value);
  Wire1.endTransmission();
}

void setupLightSensor() {
  Wire1.setSDA(ALS_SDA);
  Wire1.setSCL(ALS_SCL);
  Wire1.begin();

  Wire1.beginTransmission(ALS_ADDR);
  if (Wire1.endTransmission() != 0) {
    Serial.println("Light sensor not responding");
  }

  alsWrite(0x00, 0x08); //power on
  delay(2);
  alsWrite(0x02, 0x02); // 200 ms integration time
}

long readLux() { // -1 if the sensor does not answer
  Wire1.beginTransmission(ALS_ADDR);
  Wire1.write(0x06);
  if (Wire1.endTransmission(false) != 0) return -1;
  if (Wire1.requestFrom(ALS_ADDR, (uint8_t)2) != 2) return -1;
  int lsb = Wire1.read();
  int msb = Wire1.read();
  return lround((msb * 256 + lsb) / 1.54);
}
