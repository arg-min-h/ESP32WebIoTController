#include "tcp_server.h"
#include <WiFi.h>

WiFiServer tcpServer(23); // ポート23でTCPサーバーを起動
WiFiClient tcpClient;

void setupTcpServer() {
    tcpServer.begin();
    tcpServer.setNoDelay(true);
    Serial.println("TCP server started");
}

void sendTcpLog(const char *logMessage) {
    if (tcpClient && tcpClient.connected()) {
        tcpClient.print(logMessage);
    }
}

void handleTcpClient() {
    if (!tcpClient || !tcpClient.connected()) {
        tcpClient = tcpServer.available();
    }
    if (tcpClient) {
        while (tcpClient.available()) {
            char c = tcpClient.read();
            Serial.write(c);
        }
    }
}
