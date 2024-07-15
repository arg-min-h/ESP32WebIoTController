#ifndef WEBSOCKET_HANDLER_H
#define WEBSOCKET_HANDLER_H

#include "TCPServerHandler.h"
#include <ESPAsyncWebServer.h>

class WebSocketHandler {
  public:
    WebSocketHandler();
    void begin(AsyncWebServer *server, TCPServerHandler *tcpHandler);
    void cleanupClients(); // クライアントのクリーンアップメソッドを追加
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                   AwsEventType type, void *arg, uint8_t *data, size_t len);

  private:
    AsyncWebSocket ws;
    TCPServerHandler *tcpServerHandler;
};

#endif // WEBSOCKET_HANDLER_H
