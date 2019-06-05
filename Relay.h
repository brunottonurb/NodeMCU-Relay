#ifndef RELAY_H
#define RELAY_H

class Relay
{
private:
  int pin;
  bool state;

public:
  Relay(int p, bool s);
  void turnOn();
  void turnOff();
  bool toggle();
  bool getState();
  int getPin();
};

#endif