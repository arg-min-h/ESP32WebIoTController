#include "websocket.h"
#include "color_preferences.h"
#include "tcp_server.h" // sendTcpLog 関数をインクルード
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
        String logMessage = "WebSocket client connected from " +
                            client->remoteIP().toString() + "\r\n";
        Serial.print(logMessage);
        sendTcpLog(logMessage.c_str());
    } else if (type == WS_EVT_DISCONNECT) {
        String logMessage = "Client disconnected\r\n";
        Serial.print(logMessage);
        sendTcpLog(logMessage.c_str());
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->opcode == WS_TEXT) {
            String msg = String((char *)data).substring(0, len);
            String logMessage = "Received message: " + msg + "\r\n";
            Serial.print(logMessage);
            sendTcpLog(logMessage.c_str());

            // ArduinoJsonのJsonDocumentを使用する
            const size_t capacity = 1024;
            StaticJsonDocument<capacity> doc;
            DeserializationError error = deserializeJson(doc, msg);
            if (error) {
                logMessage =
                    "deserializeJson() failed: " + String(error.c_str()) +
                    "\r\n";
                Serial.print(logMessage);
                sendTcpLog(logMessage.c_str());
                return;
            }
            if (doc.containsKey("type")) {
                String type = doc["type"];
                if (type == "color") {
                    JsonObject data = doc["data"];
                    int r = data["r"];
                    int g = data["g"];
                    int b = data["b"];
                    logMessage = "RGB: (" + String(r) + ", " + String(g) +
                                 ", " + String(b) + ")\r\n";
                    Serial.print(logMessage);
                    sendTcpLog(logMessage.c_str());

                    // 色値を保存
                    saveColorPreferences(r, g, b);
                }
            }
        }
    }
}
