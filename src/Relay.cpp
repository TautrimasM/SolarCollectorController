#include "Relay.h"
Relay::Relay(byte pin)
{
  this->pin = pin;
  init();
}
void Relay::init()
{
  pinMode(pin, OUTPUT);
  off();
}
void Relay::on()
{
  digitalWrite(pin, active);
}
void Relay::off()
{
  digitalWrite(pin, !active);
}
bool Relay::getState()
{
  return (digitalRead(pin) == active);
}
String Relay::getStateString()
{
  if (getState())
    return "ON";
  else
    return "OFF";
}