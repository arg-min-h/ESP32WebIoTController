#include "MainController.h"

MainController::MainController()
    : ledController(25, 26, 27) {} // ここで使用するピン番号を指定

void MainController::setup() {
    Serial.begin(115200);
    delay(1000); // シリアル通信の初期化待ち時間

    // SPIFFSのセットアップ
    if (!spiffsSetup.begin()) {
        return;
    }

    // WiFiのセットアップ
    wifiSetup.begin(&tcpServerHandler);

    // TCPサーバーのセットアップ
    tcpServerHandler.begin();

    // OTAのセットアップ
    otaUpdate.begin(&tcpServerHandler);

    // Webサーバーのセットアップ
    webServerSetup.begin(&tcpServerHandler);

    // WebSocketのセットアップ
    webSocketHandler.begin(&webServerSetup.server, &tcpServerHandler);

    // LEDコントローラの初期化
    ledController.begin();

    String logMessage = "Setup completed\r\n";
    Serial.print(logMessage);
    tcpServerHandler.sendLog(logMessage.c_str());
}

void MainController::loop() {
    otaUpdate.handle(); // OTAの処理
    webSocketHandler
        .cleanupClients(); // WebSocketのクライアントをクリーンアップ
    tcpServerHandler.handleClient(); // TCPクライアントの処理
}

void MainController::setColor(int r, int g, int b) {
    ledController.setColor(r, g, b);
}
