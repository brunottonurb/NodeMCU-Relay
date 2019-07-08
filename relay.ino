#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <vector>
#include "Relay.h"
#include "conf.h"

#define NODEMCU_LED 16
#define ON true
#define OFF false

// pin 0 is the flash pin, but can be used as GPIO
// pin 2 is the WiFi LED, but can be used as GPIO
// pin 9 is unusable and crashes the board
const int pins[] = {5, 4, 0, 14, 12, 2, 10, 13};

// number of relays
const int pinsLen = 8;

std::vector<Relay> relays;

ESP8266WebServer server(80);

void setup()
{
  // setup start
  pinMode(NODEMCU_LED, OUTPUT);
  digitalWrite(NODEMCU_LED, LOW);

  relays.reserve(pinsLen);

  // initialize relays
  for (int i = 0; i < pinsLen; i++)
  {
    relays.emplace_back(pins[i], OFF);
  }

  Serial.begin(115200);

  WiFi.begin(SSID, PW);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Waiting to connect…");
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/off", turnOff);
  server.on("/on", turnOn);
  server.on("/toggle", toggle);
  server.onNotFound(notFound);

  server.begin();
  Serial.println("Server listening");

  // setup finished
  digitalWrite(NODEMCU_LED, HIGH);
}

void loop()
{
  server.handleClient();
}

// switch relay off
void turnOff()
{
  if (server.arg("relay") == "")
  {
    noId();
    return;
  }

  int pin = server.arg("relay").toInt();

  if (!inRange(pin))
  {
    unknownId(pin);
    return;
  }

  relays[pin].turnOff();

  String message = String("200: Relay " + server.arg("relay") + " off.");
  Serial.println(message);
  String response = String("{ \"id\":\"" + server.arg("relay") + "\",\"state\":\"off\"}");
  server.send(200, "application/json", response);
}

// switch relay on
void turnOn()
{
  if (server.arg("relay") == "")
  {
    noId();
    return;
  }

  int pin = server.arg("relay").toInt();

  if (!inRange(pin))
  {
    unknownId(pin);
    return;
  }

  relays[pin].turnOn();

  String message = String("200: Relay " + server.arg("relay") + " on");
  Serial.println(message);
  String response = String("{ \"id\":\"" + server.arg("relay") + "\",\"state\":\"on\"}");
  server.send(200, "application/json", response);
}

// toggle relay state
void toggle()
{
  if (server.arg("relay") == "")
  {
    noId();
    return;
  }

  int pin = server.arg("relay").toInt();

  if (!inRange(pin))
  {
    unknownId(pin);
    return;
  }

  String newState = String((relays[pin].toggle()) ? ("on") : ("off"));
  String message = String("200: Relay " + server.arg("relay") + " " + newState);
  Serial.println(message);
  String response = String("{ \"id\":\"" + server.arg("relay") + "\",\"state\":\"" + newState + "\"}");
  server.send(200, "application/json", response);
}

// returns the current relay states
void getState()
{
  // todo
}

// URI not found
void notFound()
{
  String message = String("404: " + server.uri());
  Serial.println(message);
  server.send(404, "application/json", "{\"error\":\"Not found\"}");
}

// check if val is a valid relay id
bool inRange(int val)
{
  return ((0 <= val) && (val < pinsLen));
}

// relay id in request does not exist
void unknownId(int id)
{
  String message = String("400: Unknown relay id " + String(id));
  Serial.println(message);
  String response = String("{ \"error\":\"Unknown relay id " + String(id) + "\"}");
  server.send(400, "application/json", response);
}

// request does not contain a relay parameter
void noId()
{
  Serial.println("400: No relay id specified");
  server.send(400, "application/json", "{ \"error\":\"No relay id specified\"}");
}
