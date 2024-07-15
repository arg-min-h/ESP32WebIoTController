#ifndef TCP_SERVER_HANDLER_H
#define TCP_SERVER_HANDLER_H

#include <WiFi.h>

class TCPServerHandler {
  public:
    TCPServerHandler();
    void begin();
    void handleClient();
    void sendLog(const char *logMessage);
    WiFiClient client; // クライアントの宣言
  private:
    WiFiServer server;
};

extern TCPServerHandler tcpServerHandler; // グローバル変数の宣言

#endif // TCP_SERVER_HANDLER_H
