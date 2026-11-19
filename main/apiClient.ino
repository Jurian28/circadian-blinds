#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"


WiFiClient client;

void setupApiClient() {
  connectToWifi();
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


int getWakeupTime() {
  if(WiFi.status() != WL_CONNECTED ){
    return -1;
  }

  String url = "https://circadian-blinds.onrender.com/api/wakeup-time";

  HTTPClient http;
  http.begin(url.c_str());
  int httpResponseCode = http.GET();
  String response = http.getString();
  http.end();

  if (httpResponseCode != 200) {
    return -1;
  }
  return atoi(response.c_str()); //into int
}

String getBlindStatus() {
  if(WiFi.status() != WL_CONNECTED ){
    return "";
  }

  String url = "https://circadian-blinds.onrender.com/api/blind-status";

  HTTPClient http;
  http.begin(url.c_str());
  int httpResponseCode = http.GET();
  String response = http.getString();
  http.end();

  if (httpResponseCode != 200) {
    return "";
  }
  return response; 
}

int sendSensorDataToApi(float motor, float light, float rain) {
  if(WiFi.status() != WL_CONNECTED ){
    return -1;
  }

  String url = "https://circadian-blinds.onrender.com/api/sensors";
  String body = "{\"motor\":" + String(motor) +
    ",\"light\":" + String(light) +
    ",\"rain\":" + String(rain) + "}";

  HTTPClient http;
  http.begin(url.c_str());
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(body);

  http.end();

  if (httpResponseCode == 200) {
    return 0;
  } else {
    return -1;
  }
}