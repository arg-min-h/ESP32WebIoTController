#include "websocket.h"
#include "color_preferences.h"
#include "web_server.h" // serverの外部宣言を追加
#include <ArduinoJson.h>

AsyncWebSocket ws("/ws");

void setupWebSocket() {
    ws.onEvent(onWsEvent);
    extern AsyncWebServer server; // serverの外部宣言
    server.addHandler(&ws); // WebSocketハンドラをWebサーバーに追加
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket client connected from %s\n",
                      client->remoteIP().toString().c_str());
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.println("Client disconnected");
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->opcode == WS_TEXT) {
            String msg = (char *)data;
            Serial.printf("Received message: %s\n", msg.c_str());

            // ArduinoJsonのJsonDocumentを使用する
            const size_t capacity = 1024;
            StaticJsonDocument<capacity> doc;
            DeserializationError error = deserializeJson(doc, msg);
            if (error) {
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.c_str());
                return;
            }
            if (doc.containsKey("type")) {
                String type = doc["type"];
                if (type == "color") {
                    JsonObject data = doc["data"];
                    int r = data["r"];
                    int g = data["g"];
                    int b = data["b"];
                    Serial.printf("RGB: (%d, %d, %d)\n", r, g, b);

                    // 色値を保存
                    saveColorPreferences(r, g, b);
                }
            }
        }
    }
}
