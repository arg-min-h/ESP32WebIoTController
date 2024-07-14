#include "credentials.h" // SSIDとパスワードを含むファイル
#include <WiFi.h>

void setupWiFi() {
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
}
