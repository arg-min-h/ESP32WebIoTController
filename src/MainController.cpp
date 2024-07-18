#include "MainController.h"
#include "ColorPreferences.h" // ColorPreferencesをインクルード

MainController::MainController() : ledController() {}

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

    // 初期色を設定
    ColorPreferences colorPreferences;
    int r, g, b;
    colorPreferences.getColor(r, g, b);
    ledController.setColor(r, g, b); // 遷移なしで初期色を設定

    String logMessage = "Setup completed\r\n";
    Serial.print(logMessage);
    tcpServerHandler.sendLog(logMessage.c_str());
}

void MainController::loop() {
    otaUpdate.handle(); // OTAの処理
    webSocketHandler
        .cleanupClients(); // WebSocketのクライアントをクリーンアップ
    tcpServerHandler.handleClient(); // TCPクライアントの処理
    ledController.update();          // LEDの更新
}

void MainController::setColor(int r, int g, int b) {
    ledController.setColor(r, g, b);
}
