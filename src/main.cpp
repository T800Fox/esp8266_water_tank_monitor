/*
    main.cpp
    esp8266_water_tank_monitor

    Copyright (c) 2020 Jeremy Fox All Rights Reserved.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <kenmoreCreds.h>

const char* ssid     = STASSID;
const char* password = STAPWD;
const int pwmPin = 2; // signal pin from the HXRL Max sonar
const float tankDepth = 2200; // depth of water tank in mm

float mm, sensor;
float percentOfTankFull; // expresses how full the tank is i decimel form
int flag = 0;

ESP8266WiFiMulti WiFiMulti;
WiFiServer server(3000);
WiFiClient client;

void setup() {
  // misc setup:
  Serial.begin(115200);


  // WiFi setup:
  WiFiMulti.addAP(STASSID, STAPWD);
  Serial.print("\n\nconnecting");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(750);
  }
  Serial.println("\nconnected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  // server setup:
  server.begin();

}


void loop() {
  client = server.available();

  if (client){
    // notify of new connection
    Serial.println("New connection from: ");
    Serial.print("  IP: ");
    Serial.println(client.remoteIP());
    Serial.print("  PORT: ");
    Serial.println(client.remotePort());

    // find how full the tank is:
    sensor = pulseIn(pwmPin, HIGH);
    mm = sensor - 300;  // the ultrasonic rangefinder cannot read any distance under 300mm

    percentOfTankFull = (tankDepth - mm) / tankDepth;
    Serial.print("Distance read: ");
    Serial.println(mm);
    Serial.print("Water tank currently holding: ");
    Serial.println(percentOfTankFull);

    client.println(percentOfTankFull);

    Serial.println("quitting client.");
    client.stopAll();

  }
}
