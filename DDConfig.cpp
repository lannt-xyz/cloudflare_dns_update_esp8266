#include "DDConfig.h"
#include <ESPAsyncWebServer.h>

const String filePath = "/ddclient.json";
DynamicJsonDocument config(2048);
AsyncWebServer server(80);

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

void DDConfig::beginServer() {
  server.begin();
}

void DDConfig::createServer() {
  
  server.on("/ddns-config", HTTP_GET, [&](AsyncWebServerRequest * request) {
    DynamicJsonDocument newConfig = get();

    String html = "";
    html += "<!doctype html>";
    html += "<html>";
    html += "<head>";
    html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    html += "<title>Dynamic DNS update configuartion</title>";
    html += "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css\"";
    html += "integrity=\"sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65\" crossorigin=\"anonymous\">";
    html += "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.min.js\"";
    html += "integrity=\"sha384-cuYeSxntonz0PPNlHhBs68uyIAVpIIOZZ5JqeqvYYIcEL727kskC66kF92t6Xl2V\"";
    html += "crossorigin=\"anonymous\"></script>";
    html += "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js\"";
    html += "integrity=\"sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4\"";
    html += "crossorigin=\"anonymous\"></script>";
    html += "<script src=\"https://code.jquery.com/jquery-3.6.1.min.js\" integrity=\"sha256-o88AwQnZB+VDvE9tvIXrMQaPlFFSUTR+nldQm1LuPXQ=\" crossorigin=\"anonymous\"></script>";
    html += "<script type=\"text/javascript\">";
    html += "$(document).ready(function() {";
    html += "$('#btnClear').click(function(e) {";
    html += "$('input.form-control').val('');";
    html += "});";
    html += "})";
    html += "</script>";
    html += "<style>";
    html += "form {";
    html += "padding: 10px 20px;";
    html += "}";
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += "<form action=\"/ddns-config\" method=\"POST\">";
    html += "<h4>Dynamic DNS update configuartion</h4>";
    html += "<div class=\"form-group row mb-2\">";
    html += "<label for=\"dnsZone\" class=\"col-sm-2 col-form-label\">DNS zone</label>";
    html += "<div class=\"col-sm-10\">";
    html += "<input type=\"text\"value=\"" + String(newConfig["zone"]) + "\" class=\"form-control\" id=\"dnsZone\" name=\"zone\" placeholder=\"023e105f4ecef8ad9ca31a8372d0c353\">";
    html += "</div>";
    html += "</div>";
    html += "<div class=\"form-group row mb-2\">";
    html += "<label for=\"dnsName\" class=\"col-sm-2 col-form-label\">DNS name</label>";
    html += "<div class=\"col-sm-10\">";
    html += "<input type=\"text\" value=\"" + String(newConfig["dnsName"]) + "\" class=\"form-control\" id=\"dnsName\" name=\"name\" placeholder=\"example.com\">";
    html += "</div>";
    html += "</div>";
    html += "<div class=\"form-group row mb-2\">";
    html += "<label for=\"dnsId\" class=\"col-sm-2 col-form-label\">DNS ID</label>";
    html += "<div class=\"col-sm-10\">";
    html += "<input type=\"text\" value=\"" + String(newConfig["id"]) + "\"class=\"form-control\" id=\"dnsId\" name=\"id\" placeholder=\"372e67954025e0ba6aaa6d586b9e0b59\">";
    html += "</div>";
    html += "</div>";
    html += "<div class=\"form-group row mb-2\">";
    html += "<label for=\"email\" class=\"col-sm-2 col-form-label\">Email</label>";
    html += "<div class=\"col-sm-10\">";
    html += "<input type=\"text\" value=\"" + String(newConfig["email"]) + "\"class=\"form-control\" id=\"email\" name=\"email\" aria-describedby=\"emailHelp\"";
    html += "placeholder=\"december@example.com\">";
    html += "</div>";
    html += "</div>";
    html += "<div class=\"form-group row mb-3\">";
    html += "<label for=\"key\" class=\"col-sm-2 col-form-label\">Key</label>";
    html += "<div class=\"col-sm-10\">";
    html += "<input type=\"password\" value=\"" + String(newConfig["key"]) + "\"class=\"form-control\" id=\"key\" name=\"key\" placeholder=\"Authentification key\">";
    html += "</div>";
    html += "</div>";
    html += "<div class=\"form-group row\">";
    html += "<button type=\"submit\" class=\"btn btn-primary col-sm-2\">Save</button>";
    html += "<div style=\"width: 5px; height: 5px;\"></div>";
    html += "<button type=\"button\" class=\"btn btn-secondary col-sm-2\" id=\"btnClear\">Clear</button>";
    html += "</div>";
    html += "</form>";
    html += "</body>";
    html += "</html>";

    request->send(200, "text/html", html);
  });

  server.on("/ddns-config", HTTP_POST, [&](AsyncWebServerRequest * request) {
    DynamicJsonDocument newConfig = get();
    newConfig["zone"] = request->getParam("zone", true)->value();
    newConfig["dnsName"] = request->getParam("name", true)->value();
    newConfig["id"] = request->getParam("id", true)->value();
    newConfig["email"] = request->getParam("email", true)->value();
    newConfig["key"] = request->getParam("key", true)->value();

    update(newConfig);

    request->redirect("/ddns-config");
  });
}
