#include "SPIFFSSetup.h"
#include "TCPServerHandler.h" // TCPサーバーのヘッダーをインクルード

SPIFFSSetup::SPIFFSSetup() {
    // コンストラクタで必要な初期化を行う
}

bool SPIFFSSetup::begin() {
    if (!SPIFFS.begin(true)) {
        String logMessage = "An Error has occurred while mounting SPIFFS\r\n";
        Serial.print(logMessage);
        tcpServerHandler.sendLog(logMessage.c_str());
        return false;
    }
    return true;
}
