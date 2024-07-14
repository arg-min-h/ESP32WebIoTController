#include "web_server.h"
#include "color_preferences.h"
#include "tcp_server.h" // TCPサーバーのヘッダーをインクルード
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// Webサーバーのインスタンスを作成し、ポート80でリッスン
AsyncWebServer server(80);

// メッセージパラメータの定義
const char *PARAM_MESSAGE = "message";

// Webサーバーのセットアップ
void setupWebServer() {
    // ルートURL（"/"）にアクセスがあった場合、index.htmlを提供
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
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
    server.on("/manifest.json", HTTP_GET, [](AsyncWebServerRequest *request) {
        logRequest(request); // リクエストのログを記録
        request->send(SPIFFS, "/manifest.json", "application/json");
    });

    // 色値を取得するエンドポイント
    server.on("/color", HTTP_GET, [](AsyncWebServerRequest *request) {
        logRequest(request); // リクエストのログを記録
        preferences.begin("color",
                          true); // "color" 名前空間でプリファレンスを開く
        int r = preferences.getInt("r", 0);
        int g = preferences.getInt("g", 0);
        int b = preferences.getInt("b", 0);
        preferences.end();

        // 色値をJSON形式でレスポンス
        String json = "{\"r\":" + String(r) + ",\"g\":" + String(g) +
                      ",\"b\":" + String(b) + "}";
        request->send(200, "application/json", json);
    });

    // GETリクエストのハンドラ
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
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
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request) {
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

    // 未処理のリクエストに対する404ハンドラ
    server.onNotFound(notFound);

    // Webサーバーを開始
    server.begin();
}

// リクエストのログを記録する関数
void logRequest(AsyncWebServerRequest *request) {
    String logMessage = "HTTP " + String(request->methodToString()) +
                        " request to " + request->url();
    Serial.println(logMessage);
    sendTcpLog((logMessage + "\r\n").c_str());
}

// 404エラーハンドラ
void notFound(AsyncWebServerRequest *request) {
    String logMessage = "NOT FOUND: " + request->url();
    Serial.println(logMessage);
    sendTcpLog((logMessage + "\r\n").c_str());
    request->send(404, "text/plain", "Not found");
}
