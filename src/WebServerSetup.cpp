#include "WebServerSetup.h"
#include "ColorPreferences.h"
#include "SPIFFS.h"
#include <esp_system.h> // ESP32のリセット関数を使用するために必要

const char *PARAM_MESSAGE = "message";

WebServerSetup::WebServerSetup() : server(80), tcpServerHandler(nullptr) {}

void WebServerSetup::begin(TCPServerHandler *tcpHandler) {
    tcpServerHandler = tcpHandler;

    // ルートURL（"/"）にアクセスがあった場合、index.htmlを提供
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        logRequest(request); // リクエストのログを記録
        request->send(SPIFFS, "/index.html", String(), false);
    });

    // CSSファイルを提供
    server.on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/css/style.css", "text/css");
    });

    // JavaScriptファイル（websocket.js）を提供
    server.on("/js/websocket.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/websocket.js", "application/javascript");
    });

    // JavaScriptファイル（switch-theme.js）を提供
    server.on("/js/switch-theme.js", HTTP_GET,
              [](AsyncWebServerRequest *request) {
                  request->send(SPIFFS, "/js/switch-theme.js",
                                "application/javascript");
              });

    // JavaScriptファイル（theme-setup.js）を提供
    server.on("/js/theme-setup.js", HTTP_GET,
              [](AsyncWebServerRequest *request) {
                  request->send(SPIFFS, "/js/theme-setup.js",
                                "application/javascript");
              });

    // JavaScriptファイル（color-setup.js）を提供
    server.on("/js/color-setup.js", HTTP_GET,
              [](AsyncWebServerRequest *request) {
                  request->send(SPIFFS, "/js/color-setup.js",
                                "application/javascript");
              });

    // JavaScriptファイル（service-worker.js）を提供
    server.on("/js/service-worker.js", HTTP_GET,
              [](AsyncWebServerRequest *request) {
                  request->send(SPIFFS, "/js/service-worker.js",
                                "application/javascript");
              });

    // favicon.icoを提供
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.ico", "image/x-icon");
    });

    // manifest.jsonを提供（PWA対応のため）
    server.on("/manifest.json", HTTP_GET,
              [this](AsyncWebServerRequest *request) {
                  logRequest(request); // リクエストのログを記録
                  request->send(SPIFFS, "/manifest.json", "application/json");
              });

    // 色値を取得するエンドポイント
    server.on("/color", HTTP_GET, [this](AsyncWebServerRequest *request) {
        logRequest(request); // リクエストのログを記録
        ColorPreferences colorPreferences;
        int r, g, b;
        colorPreferences.getColor(r, g, b);

        // 色値をJSON形式でレスポンス
        String json = "{\"r\":" + String(r) + ",\"g\":" + String(g) +
                      ",\"b\":" + String(b) + "}";
        request->send(200, "application/json", json);
    });

    // GETリクエストのハンドラ
    server.on("/get", HTTP_GET, [this](AsyncWebServerRequest *request) {
        logRequest(request); // リクエストのログを記録
        String message;
        // "message" パラメータがある場合、その値を取得
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        // レスポンスを送信
        request->send(200, "text/plain", "Hello, GET: " + message);
    });

    // POSTリクエストのハンドラ
    server.on("/post", HTTP_POST, [this](AsyncWebServerRequest *request) {
        logRequest(request); // リクエストのログを記録
        String message;
        // "message" パラメータがある場合、その値を取得
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        // レスポンスを送信
        request->send(200, "text/plain", "Hello, POST: " + message);
    });

    // リセットエンドポイント
    server.on("/reset", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String logMessage = "Received reset command, rebooting...\r\n";
        Serial.print(logMessage);
        tcpServerHandler->sendLog(logMessage.c_str());
        request->send(200, "text/html",
                      "<html><body><script>setTimeout(function() { "
                      "window.location.href = '/'; }, 5000);</script><p>ESP32 "
                      "is resetting...</p></body></html>");
        delay(1000);
        tcpServerHandler->sendLog("Connection closing for reset...\r\n");
        delay(500);
        tcpServerHandler->client.stop(); // クライアントを明示的に切断
        delay(500);
        ESP.restart();
    });

    // 未処理のリクエストに対する404ハンドラ
    server.onNotFound([this](AsyncWebServerRequest *request) {
        notFound(request); // リクエストのログを記録
    });

    // Webサーバーを開始
    server.begin();
}

// リクエストのログを記録する関数
void WebServerSetup::logRequest(AsyncWebServerRequest *request) {
    String logMessage = "HTTP " + String(request->methodToString()) +
                        " request to " + request->url();
    Serial.println(logMessage);
    tcpServerHandler->sendLog((logMessage + "\r\n").c_str());
}

// 404エラーハンドラ
void WebServerSetup::notFound(AsyncWebServerRequest *request) {
    String logMessage = "NOT FOUND: " + request->url();
    Serial.println(logMessage);
    tcpServerHandler->sendLog((logMessage + "\r\n").c_str());
    request->send(404, "text/plain", "Not found");
}
