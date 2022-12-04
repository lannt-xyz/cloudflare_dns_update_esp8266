#include "DDConfig.h"

const String filePath = "/ddclient.json";
DynamicJsonDocument config(2048);

DynamicJsonDocument DDConfig::get() {
  File file = LittleFS.open(filePath, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    Serial.println("this is probally first usage, so the file does not exist");
    return config;
  }

  String configuration = "";
  Serial.print("Read from file: ");
  while (file.available()) {
    configuration += file.readString();

  }
  Serial.println(configuration);
  deserializeJson(config, configuration);
  file.close();

  return config;
}

void DDConfig::update(DynamicJsonDocument newConfig) {
  String output;
  serializeJson(newConfig, output);
  Serial.print(output);
  Serial.printf("Writing file: %s\n", filePath);

  File file = LittleFS.open(filePath, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(output)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}
