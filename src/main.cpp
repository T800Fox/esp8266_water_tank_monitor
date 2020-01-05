/*
    This sketch sends a string to a TCP server, and prints a one-line response.
    You must run a TCP server in your local network.
    For example, on Linux you can use this command: nc -v -l 3000
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <creds.h>

const char* ssid     = STASSID;
const char* password = STAPWD;
const int pwmPin = 2; // signal pin from the HXRL Max sonar
const float tankDepth = 3000; // depth of water tank in mm

float mm;
float percentOfTankFull; // expresses how full the tank is i decimel form

ESP8266WiFiMulti WiFiMulti;
WiFiServer server(3000);
WiFiClient client;

void setup() {
  // misc setup:
  Serial.begin(115200);
  pinMode(pwmPin, OUTPUT);


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
    mm = 1800; // pulseIn(pwmPin, HIGH);
    mm = mm - 300;  // the ultrasonic rangefinder cannot read any distance under 300mm
    percentOfTankFull = mm / tankDepth;
    Serial.print(mm);
    Serial.print(" / ");
    Serial.print(tankDepth);
    Serial.print(" = ");
    Serial.println(mm / tankDepth);
    Serial.print("Water tank currently holding: ");
    Serial.println(percentOfTankFull);

    client.println(percentOfTankFull);
  }
  client.stop();
  Serial.println("Client disconnected.\n");

}
