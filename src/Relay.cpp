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
  digitalWrite(pin, ACTIVE_STATE);
}
void Relay::off()
{
  digitalWrite(pin, !ACTIVE_STATE);
}
bool Relay::getState()
{
  return (digitalRead(pin) == ACTIVE_STATE);
}
String Relay::getStateString()
{
  if (getState())
    return "ON";
  else
    return "OFF";
}