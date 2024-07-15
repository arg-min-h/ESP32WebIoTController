#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include "TCPServerHandler.h"
#include <WiFi.h>

class WiFiSetup {
  public:
    WiFiSetup();
    void begin(TCPServerHandler *tcpHandler);

  private:
    void connectToWiFi();
    TCPServerHandler *tcpServerHandler;
};

#endif // WIFI_SETUP_H
