#include "Arduino.h"
#include "SerialPrinter.h"
#include "Defines.h"
#include "src/Relay.h"

extern float solarCollectorTemperature;
extern float heatExchangerTemperature;
extern float boilerTemperature;

extern unsigned long sensorFailTime;

extern Relay collectorPump;
extern Relay boilerPump;
extern Relay degassingValve;

void InitSerial()
{
    Serial.begin(SERIAL_BAUD_RATE);
}

void PrintSerialData()
{
    Serial.print(
        String(solarCollectorTemperature, 1) + "," + String(heatExchangerTemperature, 1) + "," + String(boilerTemperature, 1) + "," + collectorPump.getState() + "," + boilerPump.getState() + "," + degassingValve.getState() + "," + String(millis()) + "," + String(millis() - sensorFailTime) + "\n");
}