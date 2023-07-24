#include <SPI.h>
#include <Ethernet.h>
#include <aWOT.h>
#include "leds.h"
// #include "requests.h"
#include "options.h"

EthernetServer server(80);
Application app;

// Functions
void index(Request& req, Response& res) {
  res.print("Hello World!");
}

void ledEnableDisable(Request& req, Response& res) {
  char ledPin[64], action[64];
  req.route("led", ledPin, 64);
  req.route("action", action, 64);
  digitalWrite(atoi(ledPin), (!strcmp(action, "on") ? HIGH : LOW));
  res.print(!strcmp(action, "on") ? HIGH : LOW);
}

void ledToggle(Request& req, Response& res) {
  char ledPin[64];
  req.route("led", ledPin, 64);
  int currentLEDStatus = digitalRead(atoi(ledPin));
  digitalWrite(atoi(ledPin), !currentLEDStatus);
  res.print(!currentLEDStatus);
}

void ledStatus(Request& req, Response& res) {
  char ledPin[64];
  req.route("led", ledPin, 64);
  res.print(digitalRead(atoi(ledPin)));
}

void setup() {
  if (DEBUG) Serial.begin(9600);
  initLEDS();  //Initalize and reset all LEDs to LO

  //Start server & init ethernet port
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Server started at http://");
  Serial.println(Ethernet.localIP());

  while (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
  }

  // Routes
  app.get("/", &index);
  app.get("/:led/:action", &ledEnableDisable);
  app.get("/:led/toggle", &ledToggle);
  app.get("/:led/status", &ledStatus);
}

void loop() {
  EthernetClient client = server.available();

  //Ethernet disconnect warning
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
  } else

    //Request process if no warning
    if (client.connected()) {
      app.process(&client);
      client.stop();
    }
}
