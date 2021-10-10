#include <ESP8266WebServer.h>

namespace esp8266fileserver
{
    class ESP8266FileServer
    {
    public:
        ESP8266FileServer(ESP8266WebServer *server);
        void serveFromFolder(const char *folder);

    private:
        ESP8266WebServer *_server;
    };
};