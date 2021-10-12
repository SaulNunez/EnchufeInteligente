#include <ESP8266WebServer.h>
#include <ESPAsyncTCP.h>
#include <fauxmoESP.h>
#include <ESP8266WiFi.h>
#include "LittleFS.h"

void setup() {
if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
}

void loop() {
  
}
