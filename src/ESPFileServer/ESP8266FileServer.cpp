#include "ESP8266FileServer.h";
#include <ESP8266WebServer.h>

ESP8266FileServer(ESP8266WebServer *server){
    _server = server;
}

void ESP8266FileServer::serveFromFolder(const char *folder){

}