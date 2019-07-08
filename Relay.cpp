#include <Arduino.h>
#include "Relay.h"
#define ON true
#define OFF false

Relay::Relay(int p, bool s)
{
  pin = p;
  state = s;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, !state);
}

void Relay::turnOn()
{
  digitalWrite(pin, LOW);
  state = ON;
}

void Relay::turnOff()
{
  digitalWrite(pin, HIGH);
  state = OFF;
}

bool Relay::toggle()
{
  state = !state;
  digitalWrite(pin, !state);
  return state;
}

bool Relay::getState()
{
  return state;
}

int Relay::getPin()
{
  return pin;
}
