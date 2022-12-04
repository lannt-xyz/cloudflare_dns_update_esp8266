#ifndef DDConfig_H
#define DDConfig_H

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
};

#endif
