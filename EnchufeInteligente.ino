#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <fauxmoESP.h>
#include <ESP8266WiFi.h>
#include "LittleFS.h"
AsyncWebServer server(80);

#include <fauxmoESP.h>
fauxmoESP fauxmo;

// Setup code necesary for setup mode
// We need to enable stuff to discover networks before creating a soft AP
// In which the user can give their network credential before normal usage
void wifiDeviceSetupSetup(){
  WiFi.mode(WIFI_STA);
  // Shut down old connection to another AP
  WiFi.disconnect();

  delay(100);
  
  WiFi.softAP("El poderoso enchufe inteligente :D", "1234");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/wifi_info.html", String(), false);
  });
  server.begin();
  Serial.println("HTTP server started");
}

void normalOperationSetup(){
  //Encender wifi
  WiFi.mode(WIFI_STA);
  fauxmo.addDevice("Enchufe Uno");

  fauxmo.setPort(80);
  fauxmo.enable(true);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    });
}

void normalOperationLoop(){
    fauxmo.handle();
}

void setup() {
if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
}

void loop() {
  
}