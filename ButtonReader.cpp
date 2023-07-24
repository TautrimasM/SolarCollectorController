#include "Arduino.h"
#include "ButtonReader.h"
#include "src/Button.h"
#include "src/Relay.h"
#include "EEPROMFunctions.h"
#include "Defines.h"

extern Button leftButton;
extern Button midButton;
extern Button rightButton;

extern Relay degassingValve;

extern float deltaCollectorExchanger;
extern float deltaExchangerBoiler;
extern float hysteresisCollectorExchanger;
extern float hysteresisExchangerBoiler;
extern float haltTemperature;

extern float degassingTimeStart;
extern float readButtonsTime;

extern bool refreshScreenEvent;
extern bool backlightOn;
extern bool degassingFlag;
extern bool activityFlag;
extern bool logicEvent;

extern unsigned long readButtonsInterval;

extern uint8_t menuItem;

void ReadButtons()
{
    if (leftButton.isPressed())
    {
        SetMetaParameters();
        EditParameter(false);
        return;
    }
    if (midButton.isPressed())
    {
        SetMetaParameters();
        EditMenuItem(true);
        return;
    }
    if (rightButton.isPressed())
    {
        SetMetaParameters();
        EditParameter(true);
        return;
    }
    readButtonsInterval = READ_BUTTONS_INTERVAL;
}
void SetMetaParameters()
{
    backlightOn = true;
    refreshScreenEvent = true;
    readButtonsInterval = READ_BUTTONS_INTERVAL_WHEN_PRESSED;
    activityFlag = true;
    logicEvent = true;
}

void EditParameter(bool increment)
{
    switch (menuItem)
    {
    case 1:
        if (increment)
        {
            deltaCollectorExchanger = deltaCollectorExchanger + 0.5;
        }
        else
        {
            deltaCollectorExchanger = deltaCollectorExchanger - 0.5;
        }
        break;
    case 2:
        if (increment)
        {
            if (hysteresisCollectorExchanger < 20)
            {
                hysteresisCollectorExchanger = hysteresisCollectorExchanger + 0.5;
            }
        }
        else
        {
            if (hysteresisCollectorExchanger > 0)
            {
                hysteresisCollectorExchanger = hysteresisCollectorExchanger - 0.5;
            }
        }
        break;
    case 3:
        if (increment)
        {
            deltaExchangerBoiler = deltaExchangerBoiler + 0.5;
        }
        else
        {
            deltaExchangerBoiler = deltaExchangerBoiler - 0.5;
        }
        break;
    case 4:
        if (increment)
        {
            if (hysteresisExchangerBoiler < 20)
            {
                hysteresisExchangerBoiler = hysteresisExchangerBoiler + 0.5;
            }
        }
        else
        {
            if (hysteresisExchangerBoiler > 0)
            {
                hysteresisExchangerBoiler = hysteresisExchangerBoiler - 0.5;
            }
        }
        break;
    case 5:
        degassingFlag = !degassingFlag;
        break;
    case 6:
        if (increment)
        {
            if (haltTemperature < 60)
            {
                haltTemperature = haltTemperature + 0.5;
            }
        }
        else
        {
            if (haltTemperature > 0)
            {
                haltTemperature = haltTemperature - 0.5;
            }
        }
        break;

    default:
        break;
    }
}

void EditMenuItem(bool increment)
{
    if (increment)
    {
        if (menuItem == 6)
            menuItem = 0;
        else
            menuItem++;
    }
    else
    {
        if (menuItem == 0)
            menuItem = 6;
        else
            menuItem--;
    }
    UpdateEEPROM();
}
