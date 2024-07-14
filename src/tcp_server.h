#ifndef TCP_SERVER_H
#define TCP_SERVER_H

void setupTcpServer();
void sendTcpLog(const char *logMessage);
void handleTcpClient(); // 追加: handleTcpClient のプロトタイプ宣言

#endif // TCP_SERVER_H
