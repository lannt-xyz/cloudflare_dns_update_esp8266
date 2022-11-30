#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid     = "Long1";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "P@ssworld";     // The password of the Wi-Fi network

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
}

void loop() {
  getExternalIp();

  // check each 1 minute
  delay(60000);
}

void getExternalIp() {
  WiFiClient client;
  if (!client.connect("api.ipify.org", 80)) {
    Serial.println("Failed to connect with 'api.ipify.org' !");
  } else {
    int timeout = millis() + 5000;
    client.print("GET /?format=json HTTP/1.1\r\nHost: api.ipify.org\r\n\r\n");
    while (client.available() == 0) {
      if (timeout - millis() < 0) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available()) {

    }
  }
}
