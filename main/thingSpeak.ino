#include <ThingSpeak.h>
#include <WiFi.h>
#include "secrets.h"

WiFiClient client;

void setupThingSpeak() {
  connectToWifi();
  connectToThingSpeak();
}

void connectToWifi() {
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());
}
void connectToThingSpeak() {
  ThingSpeak.begin(client);
}

void setThingSpeakValue(int num) {
  ThingSpeak.setField(1, num);
}

void sendToThingSpeak() {
  ThingSpeak.writeFields(THING_SPEAK_CHANNEL_ID, THING_SPEAK_API_KEY);
}
