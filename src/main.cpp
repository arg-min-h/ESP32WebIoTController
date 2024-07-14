#include "ota_update.h"
#include "tcp_server.h" // TCPサーバーのヘッダーをインクルード
#include "web_server.h"
#include "websocket.h"
#include "wifi_setup.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>

void setup() {
    Serial.begin(115200);
    delay(1000); // シリアル通信の初期化待ち時間

    // SPIFFSをマウント
    if (!SPIFFS.begin(true)) {
        String logMessage = "An Error has occurred while mounting SPIFFS\r\n";
        Serial.print(logMessage);
        sendTcpLog(logMessage.c_str());
        return;
    }

    // WiFiのセットアップ
    setupWiFi();

    // TCPサーバーのセットアップ
    setupTcpServer();

    // OTAのセットアップ
    setupOTA();

    // Webサーバーのセットアップ
    setupWebServer();

    // WebSocketのセットアップ
    setupWebSocket();

    String logMessage = "Setup completed\r\n";
    Serial.print(logMessage);
    sendTcpLog(logMessage.c_str());
}

void loop() {
    ArduinoOTA.handle(); // OTAの処理
    ws.cleanupClients(); // WebSocketのクライアントをクリーンアップ
    handleTcpClient();   // TCPクライアントの処理
}
