#include "WebSocketHandler.h"
#include "ColorPreferences.h"
#include "LEDConfig.h" // LEDConfig.hをインクルードしてTRANSITION_TIMEを使用
#include "MainController.h"
#include <ArduinoJson.h>

extern MainController mainController; // MainControllerのインスタンスを取得

WebSocketHandler::WebSocketHandler() : ws("/ws"), tcpServerHandler(nullptr) {}

void WebSocketHandler::begin(AsyncWebServer *server,
                             TCPServerHandler *tcpHandler) {
    tcpServerHandler = tcpHandler;
    ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client,
                      AwsEventType type, void *arg, uint8_t *data, size_t len) {
        onWsEvent(server, client, type, arg, data, len);
    });
    server->addHandler(&ws); // WebSocketハンドラをWebサーバーに追加
}

void WebSocketHandler::cleanupClients() {
    ws.cleanupClients(); // クライアントのクリーンアップ処理
}
void WebSocketHandler::onWsEvent(AsyncWebSocket *server,
                                 AsyncWebSocketClient *client,
                                 AwsEventType type, void *arg, uint8_t *data,
                                 size_t len) {
    if (type == WS_EVT_CONNECT) {
        String logMessage = "WebSocket client connected from " +
                            client->remoteIP().toString() + "\r\n";
        Serial.print(logMessage);
        tcpServerHandler->sendLog(logMessage.c_str());

        // 接続時に現在の色と速度を送信
        ColorPreferences colorPreferences;
        int r, g, b;
        float speed;
        colorPreferences.getColor(r, g, b);
        speed = colorPreferences.getSpeed();

        StaticJsonDocument<200> doc;
        doc["type"] = "initial";
        doc["data"]["r"] = r;
        doc["data"]["g"] = g;
        doc["data"]["b"] = b;
        doc["data"]["speed"] = speed;

        String message;
        serializeJson(doc, message);
        client->text(message);

    } else if (type == WS_EVT_DISCONNECT) {
        String logMessage = "Client disconnected\r\n";
        Serial.print(logMessage);
        tcpServerHandler->sendLog(logMessage.c_str());
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->opcode == WS_TEXT) {
            // UTF-8としてデコードする
            String msg = String((char *)data).substring(0, len);
            String logMessage = "Received message: " + msg + "\r\n";
            Serial.print(logMessage);
            tcpServerHandler->sendLog(logMessage.c_str());

            // ArduinoJsonのJsonDocumentを使用する
            const size_t capacity = 1024;
            StaticJsonDocument<capacity> doc;
            DeserializationError error = deserializeJson(doc, msg);
            if (error) {
                logMessage =
                    "deserializeJson() failed: " + String(error.c_str()) +
                    "\r\n";
                Serial.print(logMessage);
                tcpServerHandler->sendLog(logMessage.c_str());
                return;
            }

            if (doc.containsKey("type")) {
                String type = doc["type"];
                if (type == "color") {
                    JsonObject data = doc["data"];
                    int r = data["r"];
                    int g = data["g"];
                    int b = data["b"];
                    float speed = data["speed"];
                    logMessage = "RGB: (" + String(r) + ", " + String(g) +
                                 ", " + String(b) +
                                 "), Speed: " + String(speed) + "\r\n";
                    Serial.print(logMessage);
                    tcpServerHandler->sendLog(logMessage.c_str());

                    // 色値を保存
                    ColorPreferences colorPreferences;
                    colorPreferences.saveColor(r, g, b);
                    colorPreferences.saveSpeed(speed); // 速度を保存

                    // LEDの色を設定
                    mainController.setColor(r, g, b,
                                            speed); // 速度を追加して渡す
                }
            }
        }
    }
}
