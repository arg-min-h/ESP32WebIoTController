#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "OTAUpdate.h"
#include "SPIFFSSetup.h"
#include "TCPServerHandler.h"
#include "WebServerSetup.h"
#include "WebSocketHandler.h"
#include "WiFiSetup.h"

class MainController {
  public:
    MainController();
    void setup();
    void loop();

  private:
    WiFiSetup wifiSetup;
    SPIFFSSetup spiffsSetup;
    WebServerSetup webServerSetup;
    OTAUpdate otaUpdate;
    WebSocketHandler webSocketHandler;
    TCPServerHandler tcpServerHandler;
};

#endif // MAIN_CONTROLLER_H
