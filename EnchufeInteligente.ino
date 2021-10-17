#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <fauxmoESP.h>
#include <ESP8266WiFi.h>
#include "LittleFS.h"
AsyncWebServer server(80);

#include <fauxmoESP.h>
fauxmoESP fauxmo;

enum operationMode {
  askWifiCredentials,
  justPlug
};

// Setup code necesary for setup mode
// We need to enable stuff to discover networks before creating a soft AP
// In which the user can give their network credential before normal usage
void wifiDeviceSetupSetup(){
  WiFi.mode(WIFI_STA);
  // Shut down old connection to another AP
  WiFi.disconnect();

  Serial.println("Creating wifi network");

  delay(100);
  
  WiFi.softAP("El poderoso enchufe inteligente :D", "1234");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/wifi_info.html", String(), false);
  });
  server.on("/wifi_info", HTTP_POST, [](AsyncWebServerRequest *request){
    String ssid;
    String password;

    AsyncWebParameter* ssidParam = request->getParam("wifi");
    ssid = ssidParam->value().c_str();
    AsyncWebParameter* passwordParam = request->getParam("password");
    password = passwordParam->value().c_str();

    File f = LittleFS.open("/wifi.conf", "w");
    if(!f){
      return;
    }
    f.write(("ssid:" + ssid + "\n" + "password:" + password).c_str());
    f.close();
    ESP.restart();
  });
  server.begin();
  Serial.println("HTTP server started");
}

void normalOperationSetup(){
  //Encender wifi
  WiFi.mode(WIFI_STA);

  File file = LittleFS.open("/wifi.conf", "r");
  String wifiFileContents = file.readString();
  file.close();

  int newLineSeparator = wifiFileContents.indexOf('\n');
  String ssidFull = wifiFileContents.substring(0, newLineSeparator);
  String passwordFull = wifiFileContents.substring(newLineSeparator);

  String ssid = ssidFull.substring(5);
  String password = passwordFull.substring(9);

  WiFi.begin(ssid, password);

  fauxmo.addDevice("Enchufe Uno");

  fauxmo.setPort(80);
  fauxmo.enable(true);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    });
}

operationMode mode;

void setup() {
Serial.begin(115200);
Serial.setDebugOutput(true);
if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  if(LittleFS.exists("/wifi.conf")){
    mode = justPlug;
    normalOperationSetup();
  } else {
    mode = askWifiCredentials;
    wifiDeviceSetupSetup();
  }
}

void loop() {
  switch (mode)
  {
  case askWifiCredentials:
    fauxmo.handle();
    break;
  
  default:
    break;
  }
}
