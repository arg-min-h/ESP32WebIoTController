#include "MainController.h"
#include "ColorPreferences.h"

MainController::MainController() : ledController() {}

void MainController::setup() {
    Serial.begin(115200);
    delay(1000);

    if (!spiffsSetup.begin()) {
        return;
    }

    wifiSetup.begin(&tcpServerHandler);
    tcpServerHandler.begin();
    otaUpdate.begin(&tcpServerHandler);
    webServerSetup.begin(&tcpServerHandler);
    webSocketHandler.begin(&webServerSetup.server, &tcpServerHandler);
    ledController.begin();

    ColorPreferences colorPreferences;
    int r, g, b;
    float speed;
    float redCorrection, greenCorrection, blueCorrection;
    colorPreferences.getColor(r, g, b);
    speed = colorPreferences.getSpeed();
    colorPreferences.getBrightnessCorrection(redCorrection, greenCorrection,
                                             blueCorrection);
    ledController.setColor(r, g, b, speed);
    ledController.setBrightnessCorrection(redCorrection, greenCorrection,
                                          blueCorrection);

    String logMessage = "Setup completed\r\n";
    Serial.print(logMessage);
    tcpServerHandler.sendLog(logMessage.c_str());
}

void MainController::loop() {
    otaUpdate.handle();
    webSocketHandler.cleanupClients();
    tcpServerHandler.handleClient();
    ledController.update();
}

void MainController::setColor(int r, int g, int b, float speed) {
    ledController.setColor(r, g, b, speed);
}

void MainController::setBrightnessCorrection(float redMax, float greenMax,
                                             float blueMax) {
    ledController.setBrightnessCorrection(redMax, greenMax, blueMax);
}
