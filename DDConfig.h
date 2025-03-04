#ifndef DDConfig_H
#define DDConfig_H

//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>

#if (ARDDUINO >=100)
  #include "Arduino.h"
#endif

class DDConfig {
  public:
    DynamicJsonDocument get();
    void update(DynamicJsonDocument newConfig);

    void beginServer();
    void createServer();
};

#endif
