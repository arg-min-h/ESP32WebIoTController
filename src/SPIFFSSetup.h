#ifndef SPIFFS_SETUP_H
#define SPIFFS_SETUP_H

#include <SPIFFS.h>

class SPIFFSSetup {
  public:
    SPIFFSSetup();
    bool begin();
};

#endif // SPIFFS_SETUP_H
