#include "tcp_server.h" // TCPサーバーのヘッダーをインクルード
#include <ArduinoOTA.h>

void setupOTA() {
    // OTA設定
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_SPIFFS
            type = "filesystem";
        }
        String logMessage = "Start updating " + type + "\r\n";
        Serial.print(logMessage);
        sendTcpLog(logMessage.c_str());
    });
    ArduinoOTA.onEnd([]() {
        String logMessage = "\nEnd\r\n";
        Serial.print(logMessage);
        sendTcpLog(logMessage.c_str());
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        String logMessage =
            "Progress: " + String((progress / (total / 100))) + "%\r\n";
        Serial.print(logMessage);
        sendTcpLog(logMessage.c_str());
    });
    ArduinoOTA.onError([](ota_error_t error) {
        String logMessage = "Error[" + String(error) + "]: ";
        Serial.print(logMessage);
        sendTcpLog(logMessage.c_str());
        if (error == OTA_AUTH_ERROR) {
            logMessage = "Auth Failed\r\n";
            Serial.print(logMessage);
            sendTcpLog(logMessage.c_str());
        } else if (error == OTA_BEGIN_ERROR) {
            logMessage = "Begin Failed\r\n";
            Serial.print(logMessage);
            sendTcpLog(logMessage.c_str());
        } else if (error == OTA_CONNECT_ERROR) {
            logMessage = "Connect Failed\r\n";
            Serial.print(logMessage);
            sendTcpLog(logMessage.c_str());
        } else if (error == OTA_RECEIVE_ERROR) {
            logMessage = "Receive Failed\r\n";
            Serial.print(logMessage);
            sendTcpLog(logMessage.c_str());
        } else if (error == OTA_END_ERROR) {
            logMessage = "End Failed\r\n";
            Serial.print(logMessage);
            sendTcpLog(logMessage.c_str());
        }
    });
    ArduinoOTA.begin();
    String logMessage = "OTA Ready\r\n";
    Serial.print(logMessage);
    sendTcpLog(logMessage.c_str());
}
