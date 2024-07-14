#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <WiFi.h>

extern WiFiClient tcpClient; // 外部宣言

void setupTcpServer();
void sendTcpLog(const char *logMessage);
void handleTcpClient();

#endif // TCP_SERVER_H
