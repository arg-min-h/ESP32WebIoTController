#include "WebSocketHandler.h"
#include "ColorPreferences.h"
#include "LEDConfig.h"
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

        // 接続時に現在の色、速度、輝度補正値を送信
        ColorPreferences colorPreferences;
        int r, g, b;
        float speed;
        float redCorrection, greenCorrection, blueCorrection;
        colorPreferences.getColor(r, g, b);
        speed = colorPreferences.getSpeed();
        colorPreferences.getBrightnessCorrection(redCorrection, greenCorrection,
                                                 blueCorrection);

        DynamicJsonDocument doc(256);
        doc["type"] = "initial";
        doc["data"]["r"] = r;
        doc["data"]["g"] = g;
        doc["data"]["b"] = b;
        doc["data"]["speed"] = speed;
        doc["data"]["redCorrection"] = redCorrection;
        doc["data"]["greenCorrection"] = greenCorrection;
        doc["data"]["blueCorrection"] = blueCorrection;

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
            String msg = String((char *)data).substring(0, len);
            String logMessage = "Received message: " + msg + "\r\n";
            Serial.print(logMessage);
            tcpServerHandler->sendLog(logMessage.c_str());

            DynamicJsonDocument doc(1024);
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
                    colorPreferences.saveSpeed(speed);

                    // LEDの色を設定
                    mainController.setColor(r, g, b, speed);
                } else if (type == "brightnessCorrection") {
                    JsonObject data = doc["data"];
                    float redMax = data["redCorrection"];
                    float greenMax = data["greenCorrection"];
                    float blueMax = data["blueCorrection"];
                    logMessage =
                        "Brightness Correction - Red: " + String(redMax) +
                        ", Green: " + String(greenMax) +
                        ", Blue: " + String(blueMax) + "\r\n";
                    Serial.print(logMessage);
                    tcpServerHandler->sendLog(logMessage.c_str());

                    // 輝度補正を保存
                    ColorPreferences colorPreferences;
                    colorPreferences.saveBrightnessCorrection(redMax, greenMax,
                                                              blueMax);

                    // 輝度補正を設定
                    mainController.setBrightnessCorrection(redMax, greenMax,
                                                           blueMax);

                    // 補正値を更新した後に現在の色を再設定
                    int r, g, b;
                    float speed = colorPreferences.getSpeed();
                    colorPreferences.getColor(r, g, b);
                    mainController.setColor(r, g, b, speed);
                }
            }
        }
    }
}
