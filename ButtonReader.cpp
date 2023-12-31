#include "Arduino.h"
#include "ButtonReader.h"
#include "src/DigitalInput.h"
#include "src/Relay.h"
#include "EEPROMFunctions.h"
#include "Defines.h"

extern DigitalInput leftButton;
extern DigitalInput midButton;
extern DigitalInput rightButton;
extern DigitalInput auxHeatingInput;

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
extern bool auxHeatingIsOn;
extern bool forceHalt;

extern unsigned long auxHeatingDelayTime;
extern unsigned long readButtonsInterval;

extern uint8_t menuItem;

void ReadButtons()
{
    if (leftButton.isPressed())
    {
        SetMetaParameters();
        EditParameter(false);
    }
    if (midButton.isPressed())
    {
        SetMetaParameters();
        EditMenuItem(true);
    }
    if (rightButton.isPressed())
    {
        SetMetaParameters();
        EditParameter(true);
    }

    if (!activityFlag)
    {
        readButtonsInterval = READ_BUTTONS_INTERVAL;
    }
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
            if (deltaCollectorExchanger < DELTA_MAX)
            {
                deltaCollectorExchanger = deltaCollectorExchanger + TEMPERATURE_INCREMENT;
            }
        }
        else
        {
            if (deltaCollectorExchanger > DELTA_MIN)
            {
                deltaCollectorExchanger = deltaCollectorExchanger - TEMPERATURE_INCREMENT;
                if (hysteresisCollectorExchanger >= deltaCollectorExchanger)
                {
                    hysteresisCollectorExchanger = deltaCollectorExchanger - TEMPERATURE_INCREMENT;
                }
            }
        }
        break;

    case 2:
        if (increment)
        {
            if (hysteresisCollectorExchanger < HYSTERESIS_MAX && hysteresisCollectorExchanger < (deltaCollectorExchanger - 2 * TEMPERATURE_INCREMENT))
            {
                hysteresisCollectorExchanger = hysteresisCollectorExchanger + TEMPERATURE_INCREMENT;
            }
        }
        else
        {
            if (hysteresisCollectorExchanger > HYSTERESIS_MIN)
            {
                hysteresisCollectorExchanger = hysteresisCollectorExchanger - TEMPERATURE_INCREMENT;
            }
        }
        break;

    case 3:
        if (increment)
        {
            if (deltaExchangerBoiler < DELTA_MAX)
            {
                deltaExchangerBoiler = deltaExchangerBoiler + TEMPERATURE_INCREMENT;
            }
        }
        else
        {
            if (deltaExchangerBoiler > DELTA_MIN)
            {
                deltaExchangerBoiler = deltaExchangerBoiler - TEMPERATURE_INCREMENT;
                if (hysteresisExchangerBoiler >= deltaExchangerBoiler)
                {
                    hysteresisExchangerBoiler = deltaExchangerBoiler - TEMPERATURE_INCREMENT;
                }
            }
        }
        break;

    case 4:
        if (increment)
        {
            if (hysteresisExchangerBoiler < HYSTERESIS_MAX && hysteresisExchangerBoiler < (deltaExchangerBoiler - 2 * TEMPERATURE_INCREMENT))
            {
                hysteresisExchangerBoiler = hysteresisExchangerBoiler + TEMPERATURE_INCREMENT;
            }
        }
        else
        {
            if (hysteresisExchangerBoiler > HYSTERESIS_MIN)
            {
                hysteresisExchangerBoiler = hysteresisExchangerBoiler - TEMPERATURE_INCREMENT;
            }
        }
        break;

    case 5:
        degassingFlag = !degassingFlag;
        break;

    case 6:
        forceHalt = !forceHalt;
        break;
    case 7:
        if (increment)
        {
            if (haltTemperature < HALT_TEMP_MAX)
            {
                haltTemperature = haltTemperature + TEMPERATURE_INCREMENT;
            }
        }
        else
        {
            if (haltTemperature > HALT_TEMP_MIN)
            {
                haltTemperature = haltTemperature - TEMPERATURE_INCREMENT;
            }
        }
        break;

    case 8:
        if (increment)
        {
            if (auxHeatingDelayTime >= AUX_DELAY_TIME_MAX)
            {
                auxHeatingDelayTime = 0;
            }
            else
            {
                auxHeatingDelayTime = auxHeatingDelayTime + AUX_DELAY_TIME_INCREMENT;
            }
        }
        else
        {
            if (auxHeatingDelayTime <= 0)
            {
                auxHeatingDelayTime = AUX_DELAY_TIME_MAX;
            }
            else
            {
                auxHeatingDelayTime = auxHeatingDelayTime - AUX_DELAY_TIME_INCREMENT;
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
        if (menuItem == MENU_ITEM_COUNT)
            menuItem = 0;
        else
            menuItem++;
    }
    else
    {
        if (menuItem == 0)
            menuItem = MENU_ITEM_COUNT;
        else
            menuItem--;
    }
    UpdateEEPROM();
}
