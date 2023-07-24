#ifndef RELAY
#define RELAY

#include <Arduino.h>
#define ACTIVE_STATE LOW // define active state

class Relay
{

private:
  byte pin;

public:
  Relay(byte pin);
  void init();
  void on();
  void off();
  bool getState();
  String getStateString();
};

#endif