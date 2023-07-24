#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#define ACTIVE_STATE LOW // define active state
class Button
{

private:
  byte pin;

public:
  Button(byte pin);

  void init();
  void update();

  byte getState();
  bool isPressed();
};

#endif