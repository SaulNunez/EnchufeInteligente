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
  WiFi.softAP("IntelliPlug1");
  Serial.println("AP address:" + WiFi.softAPIP().toString());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/wifi_info.html", String(), false);
  });

  server.on("/available_networks", HTTP_GET, [](AsyncWebServerRequest *request){
    String returnObj = "{ \"wifiNetworks\": [";
    int networksFound = WiFi.scanNetworks();
    Serial.println("Networks found: " + networksFound);
    for(int i = 0; i < networksFound; i++){
      returnObj.concat("\"" + WiFi.SSID(i) + "\"");
      if(i != networksFound - 1){
        returnObj.concat(",");
      }
    }
    returnObj.concat("] }");
    Serial.println(returnObj);

    request->send(200, "application/json", returnObj);
  });

  server.on("/wifi_info", HTTP_POST, [](AsyncWebServerRequest *request){
    String ssid;
    String password;

    AsyncWebParameter* ssidParam = request->getParam("ssid");
    ssid = ssidParam->value().c_str();
    AsyncWebParameter* passwordParam = request->getParam("password");
    password = passwordParam->value().c_str();

    Serial.println(("ssid:" + ssid + "\n" + "password:" + password).c_str());

    File f = LittleFS.open("/wifi.conf", "w");
    if(!f){
      request->send(500);
      return;
    }
    Serial.println("Saving connection parameters...");
    f.write(("ssid:" + ssid + "\n" + "password:" + password).c_str());
    f.close();
    request->send(200);
    ESP.restart();
  });
  server.begin();
  Serial.println("HTTP server started");
}

const char* enchufeUno = "Enchufe Uno";
const char* enchufeDos = "Enchufe Dos";

#define RELAY_ENCHUFE_1 D1
#define RELAY_ENCHUFE_2 D2

void normalOperationSetup(){
  Serial.println("IoT mode");
  
  pinMode(RELAY_ENCHUFE_1, OUTPUT);
  pinMode(RELAY_ENCHUFE_2, OUTPUT);
  digitalWrite(RELAY_ENCHUFE_1, LOW);
  digitalWrite(RELAY_ENCHUFE_2, LOW);

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

  fauxmo.createServer(true);
  fauxmo.setPort(80);
  fauxmo.enable(true);

  fauxmo.addDevice(enchufeUno);
  fauxmo.addDevice(enchufeDos);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        if(strcmp(device_name,enchufeUno) == 0){
          digitalWrite(RELAY_ENCHUFE_1, state? HIGH: LOW);
        } else if(strcmp(device_name, enchufeDos) == 0){
          digitalWrite(RELAY_ENCHUFE_2, state? HIGH: LOW);
        }
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
  if(mode == justPlug){
    fauxmo.handle();
  } else if(mode == askWifiCredentials){
    
  }
}
