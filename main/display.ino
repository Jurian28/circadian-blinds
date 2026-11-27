#include "config.h"
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 oled(128, 64, &Wire, -1);

void setupDisplay() {
  Wire.begin();

  oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  oled.setTextColor(SSD1306_WHITE);

  oled.clearDisplay();
}

void displayLines(const String line1, const String line2, const String line3, const String line4) {   // step message on OLED + Serial
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0, 0);   oled.println(line1);
  oled.setCursor(0, 16);  oled.println(line2);
  oled.setCursor(0, 32);  oled.println(line3);
  oled.setCursor(0, 48);  oled.println(line4);
  oled.display();
}
