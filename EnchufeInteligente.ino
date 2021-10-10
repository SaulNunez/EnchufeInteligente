#include <ESP8266WebServer-impl.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <Parsing-impl.h>
#include <Uri.h>

#include <AsyncPrinter.h>
#include <async_config.h>
#include <DebugPrintMacros.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncTCPbuffer.h>
#include <SyncClient.h>
#include <tcp_axtls.h>

#include <fauxmoESP.h>

#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

fauxmoESP fauxmo;

// Setup code necesary for setup mode
// We need to enable stuff to discover networks before creating a soft AP
// In which the user can give their network credential before normal usage
void wifiDeviceSetupSetup(){
  WiFi.mode(WIFI_STA);
  // Shut down old connection to another AP
  WiFi.disconnect();

  delay(100);

  int n = WiFi.scanNetworks();
  Serial.println(" network(s) found");
  
  for (int i = 0; i < n; i++)
  {
    Serial.println(WiFi.SSID(i));
  }
  
  WiFi.softAP("El poderoso enchufe inteligente :D", "1234");
  
}

struct wifiInformation {
  String ssid;
  String passwd;
};

void normalOperation(){
  //Encender wifi
  WiFi.mode(WIFI_STA);
  fauxmo.addDevice("Enchufe Uno");
  fauxmo.addDevice("Enfuche Dos");

  fauxmo.setPort(80);
  fauxmo.enable(true);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    });
}

void setup() {
}

void loop() {
  fauxmo.handle();
}
