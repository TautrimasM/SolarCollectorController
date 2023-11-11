#ifndef DINPUT_H
#define DINPUT_H

#include <Arduino.h>
#define ACTIVE_STATE LOW // define active state
class DigitalInput
{

private:
  byte pin;

public:
  DigitalInput(byte pin);

  void init();
  bool isPressed();
};

#endif