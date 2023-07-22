#include <SPI.h>
#include <Ethernet.h>
#include <aWOT.h>

EthernetServer server(80);
Application app;

void index(Request &req, Response &res) {
  res.print("Hello World!");
}

void redOn(Request &req, Response &res) {
  digitalWrite(3, HIGH);
  res.print(HIGH);
}

void redOff(Request &req, Response &res) {
  digitalWrite(3, LOW);
  res.print(LOW);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);

  byte mac[] = { 0xA8, 0x61, 0xAE, 0XF3, 0x90 };
  byte ip[] = { 192, 168, 1, 54};

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  Serial.begin(9600);
  Ethernet.begin(mac, ip);

  app.get("/", &index);
  app.get("/red/on", &redOn);
  app.get("/red/off", &redOff);
  server.begin();

  Serial.print("Server started ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // put your main code here, to run repeatedly
  EthernetClient client = server.available();

  if (client.connected()) {
    app.process(&client);
    client.stop();
  }
}
