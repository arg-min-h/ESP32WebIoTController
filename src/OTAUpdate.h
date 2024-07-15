#ifndef OTA_UPDATE_H
#define OTA_UPDATE_H

#include "TCPServerHandler.h"
#include <ArduinoOTA.h>

class OTAUpdate {
  public:
    OTAUpdate();
    void begin(TCPServerHandler *tcpHandler);
    void handle(); // handle メソッドの宣言
  private:
    TCPServerHandler *tcpServerHandler;
};

#endif // OTA_UPDATE_H
