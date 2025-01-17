#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "LEDPWMController.h"
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
    void setColor(int r, int g, int b, float speed);
    void
    setBrightnessCorrection(float redCorrection, float greenCorrection,
                            float blueCorrection); // 輝度補正の設定メソッド

  private:
    WiFiSetup wifiSetup;
    SPIFFSSetup spiffsSetup;
    WebServerSetup webServerSetup;
    OTAUpdate otaUpdate;
    WebSocketHandler webSocketHandler;
    TCPServerHandler tcpServerHandler;
    LEDPWMController ledController; // 新しいメンバー変数
};

#endif // MAIN_CONTROLLER_H
