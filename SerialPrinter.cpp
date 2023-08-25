#include "Arduino.h"
#include "SerialPrinter.h"
#include "Defines.h"
#include "src/Relay.h"
#include "src/Button.h"

extern float solarCollectorTemperature;
extern float heatExchangerTemperature;
extern float boilerTemperature;

extern unsigned long sensorFailTime;

extern Relay collectorPump;
extern Relay boilerPump;
extern Relay degassingValve;

extern Button auxHeatingInput;

void InitSerial()
{
    Serial.begin(SERIAL_BAUD_RATE);
}

void PrintSerialData()
{
    // Ts
    Serial.print(String(solarCollectorTemperature, 1));
    Serial.print(",");
    // Tk
    Serial.print(String(heatExchangerTemperature, 1));
    Serial.print(",");
    // Tb
    Serial.print(String(boilerTemperature, 1));

    // Ss
    Serial.print(",");
    Serial.print(collectorPump.getStateString());
    Serial.print(",");
    // Sb
    Serial.print(boilerPump.getStateString());
    Serial.print(",");
    // DG
    Serial.print(degassingValve.getStateString());
    Serial.print(",");
    // BT
    if (auxHeatingInput.isPressed())
        Serial.print("ON");
    else
        Serial.print("OFF");
    Serial.print(",");
    // run time
    Serial.print(millis());
    // sensor fail time
    Serial.print(",");
    Serial.print(millis() - sensorFailTime);
    Serial.print("\n");
}