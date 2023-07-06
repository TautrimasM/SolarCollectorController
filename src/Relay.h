#ifndef RELAY
#define RELAY

#include <Arduino.h>

class Relay
{

private:
  byte pin;
  byte active = LOW; // define active state

public:
  Relay(byte pin);
  void init();
  void on();
  void off();
  bool getState();
  String getStateString();
};

#endif