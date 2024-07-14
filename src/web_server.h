#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESPAsyncWebServer.h>

extern AsyncWebServer server; // serverの外部宣言

void setupWebServer();
void logRequest(AsyncWebServerRequest *request);
void notFound(AsyncWebServerRequest *request);

#endif // WEB_SERVER_H
