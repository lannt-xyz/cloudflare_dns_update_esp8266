#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
#include<WiFiManager.h>

#include "DDConfig.h"

HTTPClient httpClient;
WiFiClient wfClient;

DDConfig ddConfig;

void setup() {
  Serial.begin(115200);
  LittleFS.begin();

  WiFiManager wm;
  bool res;
  res = wm.autoConnect("DNS Updater", "Abc12345");

  if (!res) {
    Serial.println("Failed to connect.");
  } else {
    Serial.println("WiFi connected.");
  }

  // reset stored IP
  DynamicJsonDocument config = ddConfig.get();
  config["ip"] = "";
  ddConfig.update(config);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  
  // retrive global IP
  String ip = getExternalIp();
  // retrive from eprom
  String storedIp = getIpFromRom();
  if (String("") != ip && ip != storedIp) {
    Serial.println(">>> new IP found:" + ip);
    // request to cloudflare for ip update
    updateDNS(ip);
    // store new IP to eprom
    updateEprom(ip);
  }
  delay(6000);
}

String getExternalIp() {
  httpClient.begin(wfClient, "http://api.ipify.org");
  int statusCode = httpClient.GET();
  String ip = "";
  if (statusCode == 200) {
    ip = httpClient.getString();
  }
  httpClient.end();
  return ip;
}

String getIpFromRom() {
  return ddConfig.get()["ip"];
}

void updateEprom(String ip) {
  DynamicJsonDocument config = ddConfig.get();
  config["ip"] = ip;
  ddConfig.update(config);
}

void updateDNS(String ip) {
  String zone = "b195e88cd18715f2423dac134dd47f35";
  String dnsName = "lannt.store";
  String dnsIdentifier = "405fb98a9c0646bb0b764ecc1999632d";
  String email = "mr.n.anrew@gmail.com";
  String key = "601ca050c6ecdef36eace80fc7785c86abc61";
  String updateUrl = "https://api.cloudflare.com/client/v4/zones/" + zone + "/dns_records/" + dnsIdentifier;

  DynamicJsonDocument body(2480);
  body["content"] = ip;
  body["name"] = dnsName;
  body["type"] = "A";
  body["proxied"] = false;
  body["ttl"] = 1;

  String requestBody;
  serializeJson(body, requestBody);

  WiFiClientSecure sslWifi;
  sslWifi.setFingerprint("E0B9C358AC40E0D18E6DD208DE99A1AC46CA9476");

  httpClient.begin(sslWifi, updateUrl);
  httpClient.addHeader("Host", "api.cloudflare.com");
  httpClient.addHeader("Content-Type", "application/json");
  httpClient.addHeader("X-Auth-Email", email);
  httpClient.addHeader("X-Auth-Key", key);
  int statusCode = httpClient.PUT(requestBody);
  if (statusCode == 200) {
    Serial.println(">>> DNS updated with IP address: " + ip);
  }
  httpClient.end();
}
