#include "credentials.h" // SSIDとパスワードを含むファイル
#include "tcp_server.h"  // TCPサーバーのヘッダーをインクルード
#include <WiFi.h>

void setupWiFi() {
    // 固定IPアドレスの設定（必要に応じて）
    IPAddress local_IP(192, 168, 0, 184);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress primaryDNS(8, 8, 8, 8); // オプション: プライマリDNSサーバー
    IPAddress secondaryDNS(8, 8, 4, 4); // オプション: セカンダリDNSサーバー

    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        String logMessage = "STA Failed to configure\r\n";
        Serial.print(logMessage);
        sendTcpLog(logMessage.c_str());
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        String logMessage = "WiFi Failed!\r\n";
        Serial.print(logMessage);
        sendTcpLog(logMessage.c_str());
        return;
    }

    String logMessage = "IP Address: " + WiFi.localIP().toString() + "\r\n";
    Serial.print(logMessage);
    sendTcpLog(logMessage.c_str());
}
