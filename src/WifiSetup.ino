#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

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
    request->send(LittleFS, "/wifi_info.html", String(), false, processor);
  });
  server.begin();
  Serial.println("HTTP server started");
}
void wifiDeviceSetupLoop(){
    server.handleClient();
}