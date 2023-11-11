#include "DigitalInput.h"

DigitalInput::DigitalInput(byte pin)
{
  this->pin = pin;
  init();
}
void DigitalInput::init()
{
  pinMode(pin, INPUT);
}

bool DigitalInput::isPressed()
{
  return (digitalRead(pin) == ACTIVE_STATE);
}