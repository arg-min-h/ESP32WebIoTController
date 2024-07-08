#include "credentials.h" // SSIDとパスワードを含むファイル
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncWebSocket.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <SPIFFS.h>
#include <WiFi.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

Preferences preferences;

const char *PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request) {
    Serial.printf("NOT FOUND: %s\n", request->url().c_str());
    request->send(404, "text/plain", "Not found");
}

void logRequest(AsyncWebServerRequest *request) {
    Serial.printf("HTTP %s request to %s\n", request->methodToString(),
                  request->url().c_str());
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
            const size_t capacity = JSON_OBJECT_SIZE(3) + 30;
            DynamicJsonDocument doc(capacity);
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
                    preferences.begin("color", false);
                    preferences.putInt("r", r);
                    preferences.putInt("g", g);
                    preferences.putInt("b", b);
                    preferences.end();
                }
            }
        }
    }
}

void setup() {
    Serial.begin(115200);

    // SPIFFSをマウント
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // 固定IPアドレスの設定（必要に応じて）
    IPAddress local_IP(192, 168, 0, 184);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress primaryDNS(8, 8, 8, 8); // オプション: プライマリDNSサーバー
    IPAddress secondaryDNS(8, 8, 4, 4); // オプション: セカンダリDNSサーバー

    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("STA Failed to configure");
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // WebSocketサーバーを設定
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    // ルートURLでindex.htmlを提供
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        logRequest(request);
        request->send(SPIFFS, "/index.html", String(), false);
    });

    server.on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/css/style.css", "text/css");
    });

    server.on("/js/websocket.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/websocket.js", "application/javascript");
    });

    server.on("/js/switch-theme.js", HTTP_GET,
              [](AsyncWebServerRequest *request) {
                  request->send(SPIFFS, "/js/switch-theme.js",
                                "application/javascript");
              });

    // favicon.icoを提供
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.ico", "image/x-icon");
    });

    // manifest.jsonを提供（PWA対応のため）
    server.on("/manifest.json", HTTP_GET, [](AsyncWebServerRequest *request) {
        logRequest(request);
        request->send(SPIFFS, "/manifest.json", "application/json");
    });

    // 色値を取得するエンドポイント
    server.on("/color", HTTP_GET, [](AsyncWebServerRequest *request) {
        logRequest(request);
        preferences.begin("color", true);
        int r = preferences.getInt("r", 0);
        int g = preferences.getInt("g", 0);
        int b = preferences.getInt("b", 0);
        preferences.end();

        String json = "{\"r\":" + String(r) + ",\"g\":" + String(g) +
                      ",\"b\":" + String(b) + "}";
        request->send(200, "application/json", json);
    });

    // GETリクエストハンドラ
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
        logRequest(request);
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });

    // POSTリクエストハンドラ
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request) {
        logRequest(request);
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
    ws.cleanupClients(); // WebSocketのクライアントをクリーンアップ
}
