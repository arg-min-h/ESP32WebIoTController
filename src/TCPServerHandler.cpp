#include "TCPServerHandler.h"

TCPServerHandler tcpServerHandler; // インスタンスの定義

TCPServerHandler::TCPServerHandler() : server(23) {}

void TCPServerHandler::begin() {
    server.begin();
    server.setNoDelay(true);
    Serial.println("TCP server started");
    sendLog("TCP server started\r\n");
}

void TCPServerHandler::handleClient() {
    if (!client || !client.connected()) {
        client = server.available();
        if (client) {
            Serial.println("New client connected");
            sendLog("New client connected\r\n");
        }
    }
    if (client) {
        while (client.available()) {
            char c = client.read();
            Serial.write(c);
        }
    }
}

void TCPServerHandler::sendLog(const char *logMessage) {
    if (client && client.connected()) {
        client.print(logMessage);
        Serial.print("Sent to client: ");
        Serial.print(logMessage);
    } else {
        Serial.println("No client connected");
    }
}
