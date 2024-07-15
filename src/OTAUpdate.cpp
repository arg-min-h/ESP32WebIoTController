#include "OTAUpdate.h"

OTAUpdate::OTAUpdate() : tcpServerHandler(nullptr) {}

void OTAUpdate::begin(TCPServerHandler *tcpHandler) {
    tcpServerHandler = tcpHandler;

    // OTAの初期化とハンドラー設定
    ArduinoOTA.onStart([this]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {
            type = "filesystem";
        }
        String logMessage = "Start updating " + type + "\r\n";
        Serial.print(logMessage);
        tcpServerHandler->sendLog(logMessage.c_str());
    });
    ArduinoOTA.onEnd([this]() {
        String logMessage = "\nEnd\r\n";
        Serial.print(logMessage);
        tcpServerHandler->sendLog(logMessage.c_str());
    });
    ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
        String logMessage =
            "Progress: " + String(progress / (total / 100)) + "%\r\n";
        Serial.print(logMessage);
        tcpServerHandler->sendLog(logMessage.c_str());
    });
    ArduinoOTA.onError([this](ota_error_t error) {
        String logMessage = "Error[" + String(error) + "]: ";
        if (error == OTA_AUTH_ERROR) {
            logMessage += "Auth Failed\r\n";
        } else if (error == OTA_BEGIN_ERROR) {
            logMessage += "Begin Failed\r\n";
        } else if (error == OTA_CONNECT_ERROR) {
            logMessage += "Connect Failed\r\n";
        } else if (error == OTA_RECEIVE_ERROR) {
            logMessage += "Receive Failed\r\n";
        } else if (error == OTA_END_ERROR) {
            logMessage += "End Failed\r\n";
        }
        Serial.print(logMessage);
        tcpServerHandler->sendLog(logMessage.c_str());
    });
    ArduinoOTA.begin();
    String logMessage = "OTA Ready\r\n";
    Serial.print(logMessage);
    tcpServerHandler->sendLog(logMessage.c_str());
}

void OTAUpdate::handle() { ArduinoOTA.handle(); }
