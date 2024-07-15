#ifndef WEB_SERVER_SETUP_H
#define WEB_SERVER_SETUP_H

#include "TCPServerHandler.h"
#include <ESPAsyncWebServer.h>

class WebServerSetup {
  public:
    WebServerSetup();
    void begin(TCPServerHandler *tcpHandler);
    void logRequest(AsyncWebServerRequest *request);
    void notFound(AsyncWebServerRequest *request);
    AsyncWebServer server;

  private:
    TCPServerHandler *tcpServerHandler;
};

#endif // WEB_SERVER_SETUP_H
