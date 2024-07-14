#include "ota_update.h"
#include "web_server.h"
#include "websocket.h"
#include "wifi_setup.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>

void setup() {
    Serial.begin(115200);

    // SPIFFSをマウント
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // WiFiのセットアップ
    setupWiFi();

    // OTAのセットアップ
    setupOTA();

    // Webサーバーのセットアップ
    setupWebServer();

    // WebSocketのセットアップ
    setupWebSocket();

    Serial.println("Setup completed");
}

void loop() {
    ArduinoOTA.handle(); // OTAの処理
    ws.cleanupClients(); // WebSocketのクライアントをクリーンアップ
}
