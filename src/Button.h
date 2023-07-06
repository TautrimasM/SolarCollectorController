#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{

private:
  byte pin;
  byte active = LOW; // define active state

public:
  Button(byte pin);

  void init();
  void update();

  byte getState();
  bool isPressed();
};

#endif