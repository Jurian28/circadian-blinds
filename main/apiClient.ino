#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"
#include <iostream>
#include <string> 


WiFiClientSecure client;


void setupApiClient() {
  connectToWifi();
  while (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
  client.setInsecure();

}

void connectToWifi() {
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    i++;
    if (i>20) {
      break;
    }
    Serial.print(".");
  }

  Serial.println(WiFi.localIP());
}


String getWakeupTime() {
  if(WiFi.status() != WL_CONNECTED ){
    return "";
  }

  String url = "https://circadian-blinds.onrender.com/api/wakeup-time";


  HTTPClient http;
  http.begin(client, url);
  int httpResponseCode = http.GET();
  String response = http.getString();
  http.end();

  Serial.println("httpResponseCode: [" + String(httpResponseCode) + "]");  
  // Serial.println("response = " + response);
  if (httpResponseCode != 200) {
    return "";
  }
  return response;
}

String getBlindStatus() {
  if(WiFi.status() != WL_CONNECTED ){
    return "";
  }

  String url = "https://circadian-blinds.onrender.com/api/blind-status";

  HTTPClient http;
  http.begin(client, url);
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
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(body);

  http.end();

  if (httpResponseCode == 200) {
    return 0;
  } else {
    return -1;
  }
}
