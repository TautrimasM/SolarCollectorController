#include "Arduino.h"
#include "SerialHandler.h"
#include "Defines.h"
#include "src/Relay.h"
#include "src/DigitalInput.h"
#include "EEPROMFunctions.h"
#include "ScreenUpdater.h"

extern float solarCollectorTemperature;
extern float heatExchangerTemperature;
extern float boilerTemperature;
extern float deltaCollectorExchanger;
extern float deltaExchangerBoiler;
extern float hysteresisCollectorExchanger;
extern float hysteresisExchangerBoiler;
extern float haltTemperature;

extern unsigned long auxHeatingDelayTime;
extern unsigned long auxHeatingStopTime;
extern unsigned long sensorFailTime;

extern bool degassingFlag;
extern bool degassingInProgress;
extern bool forceHalt;
extern bool sensorErrorForLongTime;

extern Relay collectorPump;
extern Relay boilerPump;
extern Relay degassingValve;

extern DigitalInput auxHeatingInput;

bool timingStarted = false;
unsigned long startTime = 0;
char receivedData[MAX_UART_CHARS] = "";

unsigned long maxTime = ((MAX_UART_CHARS * 10) / SERIAL_BAUD_RATE) * 1000 + 1;

void InitSerial()
{
    Serial.begin(SERIAL_BAUD_RATE);
}

void HandleComm()
{
    if (Serial.available() > 0)
    {
        if (!timingStarted)
        {
            startTime = millis();
        }
        if (millis() - startTime > maxTime)
        {

            byte n = Serial.available();
            for (int i = 0; i < n; i++)
            {
                receivedData[i] = Serial.read();
            }
            HandleData();
        }
    }
}

void HandleData()
{
    if (receivedData[0] == '0')
    {
        SendData();
    }
    else if (receivedData[0] == '1')
    {
        int command = 0;
        int degass = 0, systemHalt = 0;

        // Tokenize the string using strtok
        char *token = strtok(receivedData, ",");
        int index = 0;

        while (token != NULL && index < 9)
        {
            switch (index)
            {
            case 0:
                command = atoi(token);
                break;
            case 1:
                deltaCollectorExchanger = atof(token);
                break;
            case 2:
                deltaExchangerBoiler = atof(token);
                break;
            case 3:
                hysteresisCollectorExchanger = atof(token);
                break;
            case 4:
                hysteresisExchangerBoiler = atof(token);
                break;
            case 5:
                haltTemperature = atof(token);
                break;
            case 6:
                auxHeatingDelayTime = strtoul(token, NULL, 10);
                break;
            case 7:
                degass = atoi(token);
                break;
            case 8:
                systemHalt = atoi(token);
                break;
            }

            token = strtok(NULL, ",");
            index++;
        }

        if (degass == 1)
        {
            degassingFlag = true;
        }
        else
        {
            degassingFlag = false;
        }
        if (systemHalt == 1)
        {
            forceHalt = true;
        }
        else
        {
            forceHalt = false;
        }
        UpdateEEPROM();
        SendData();
    }
}

void SendData()
{
    Serial.print(millis());
    Serial.print(",");
    Serial.print(sensorFailTime);
    Serial.print(",");
    Serial.print(solarCollectorTemperature);
    Serial.print(",");
    Serial.print(heatExchangerTemperature);
    Serial.print(",");
    Serial.print(boilerTemperature);
    Serial.print(",");
    Serial.print(auxHeatingStopTime);
    Serial.print(",");
    Serial.print(deltaCollectorExchanger);
    Serial.print(",");
    Serial.print(deltaExchangerBoiler);
    Serial.print(",");
    Serial.print(hysteresisCollectorExchanger);
    Serial.print(",");
    Serial.print(hysteresisExchangerBoiler);
    Serial.print(",");
    Serial.print(haltTemperature);
    Serial.print(",");
    Serial.print(auxHeatingDelayTime);
    Serial.print(",");
    Serial.print(collectorPump.getState() ? 1 : 0);
    Serial.print(",");
    Serial.print(boilerPump.getState() ? 1 : 0);
    Serial.print(",");
    Serial.print(degassingValve.getState() ? 1 : 0);
    Serial.print(",");
    Serial.print(auxHeatingInput.isPressed() ? 1 : 0);
    Serial.print(",");
    Serial.print(degassingInProgress ? 1 : 0);
    Serial.print(",");
    Serial.print(sensorErrorForLongTime ? 1 : 0);
    Serial.println();
}
