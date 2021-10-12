#include <ESPAsyncTCP.h>
#include <fauxmoESP.h>

fauxmoESP fauxmo;

struct wifiInformation {
  String ssid;
  String passwd;
};

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