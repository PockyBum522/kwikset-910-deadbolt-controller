#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
#include <ArduinoOTA.h>
#include <UDPDebug.h>

const char* ssid = "SSID HERE";
const char* password = "PSK HERE";

ESP8266WebServer server(80);
UDPDebug debugOut = UDPDebug(31445);

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(15, INPUT);
      
  pinMode(14, INPUT);
  
  wifiSetupOTA();

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/lock", []() {
    server.send(200, "text/plain", "lock confirmation");
    lockDoor();
  });

  server.on("/unlock", []() {
    server.send(200, "text/plain", "unlock confirmation");
    unlockDoor();
  });

  server.onNotFound(handleNotFound);

  server.begin();
}

void loop(void) {
  server.handleClient();
  ArduinoOTA.handle();
}

void lockDoor() {
  pinMode (14, OUTPUT);
  SoftwareSerial softSerial(12, 14, false, 256); // RX, TX  12 is D6 and 14 is D5
  softSerial.begin(9600);
  
  byte lockCmd[] =   {0xBD, 0x04, 0x0C, 0xE7, 0x03, 0x13}; // Lock command
  softSerial.write(lockCmd, sizeof(lockCmd)); // Send it

  delay(100);
  ESP.restart();
}

void unlockDoor() {
  pinMode (14, OUTPUT);
  SoftwareSerial softSerial(12, 14, false, 256); // RX, TX 12 is D6 and 14 is D5
  softSerial.begin(9600);
  
  byte unlockCmd[] = {0xBD, 0x04, 0x03, 0xE7, 0x05, 0x1A}; // Unlock Command
  softSerial.write(unlockCmd, sizeof(unlockCmd)); // Send it

  delay(100);
  ESP.restart();
}
