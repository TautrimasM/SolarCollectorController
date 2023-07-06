#include "Arduino.h"
#include "ButtonReader.h"
#include "src/Button.h"
#include "src/Relay.h"
#include "EEPROMFunctions.h"

extern Button LeftButton;
extern Button MidButton;
extern Button RightButton;

extern Relay DegassingValve;

extern float DeltaCollectorExchanger;
extern float DeltaExchangerBoiler;
extern float HysteresisCollectorExchanger;
extern float HysteresisExchangerBoiler;
extern float HaltTemperature;

extern float DegassingTimeStart;
extern float ReadButtonsTime;

extern bool RefreshScreenEvent;
extern bool LogicEvent;
extern bool BacklightOn;
extern bool DegassingFlag;
extern bool ActivityFlag;
;
extern unsigned long ReadButtonsInterval;

extern uint8_t MenuItem;

void ReadButtons()
{
    if (LeftButton.isPressed())
    {
        EditParameter(false);
        return;
    }
    if (MidButton.isPressed())
    {
        EditMenuItem(true);
        return;
    }
    if (RightButton.isPressed())
    {
        EditParameter(true);
        return;
    }
    ReadButtonsInterval = 100;
}

void EditParameter(bool increment)
{
    BacklightOn = true;
    LogicEvent = true;
    RefreshScreenEvent = true;
    ReadButtonsInterval = 500;
    ActivityFlag = true;

    switch (MenuItem)
    {
    case 1:
        if (increment)
        {
            DeltaCollectorExchanger = DeltaCollectorExchanger + 0.5;
        }
        else
        {
            DeltaCollectorExchanger = DeltaCollectorExchanger - 0.5;
        }
        break;
    case 2:
        if (increment)
        {
            if (HysteresisCollectorExchanger < 20)
            {
                HysteresisCollectorExchanger = HysteresisCollectorExchanger + 0.5;
            }
        }
        else
        {
            if (HysteresisCollectorExchanger > 0)
            {
                HysteresisCollectorExchanger = HysteresisCollectorExchanger - 0.5;
            }
        }
        break;
    case 3:
        if (increment)
        {
            DeltaExchangerBoiler = DeltaExchangerBoiler + 0.5;
        }
        else
        {
            DeltaExchangerBoiler = DeltaExchangerBoiler - 0.5;
        }
        break;
    case 4:
        if (increment)
        {
            if (HysteresisExchangerBoiler < 20)
            {
                HysteresisExchangerBoiler = HysteresisExchangerBoiler + 0.5;
            }
        }
        else
        {
            if (HysteresisExchangerBoiler > 0)
            {
                HysteresisExchangerBoiler = HysteresisExchangerBoiler - 0.5;
            }
        }
        break;
    case 5:
        DegassingFlag = !DegassingFlag;
        break;
    case 6:
        if (increment)
        {
            if (HaltTemperature < 60)
            {
                HaltTemperature = HaltTemperature + 0.5;
            }
        }
        else
        {
            if (HaltTemperature > 0)
            {
                HaltTemperature = HaltTemperature - 0.5;
            }
        }
        break;

    default:
        break;
    }
}

void EditMenuItem(bool increment)
{
    BacklightOn = true;
    RefreshScreenEvent = true;
    ReadButtonsInterval = 500;
    ActivityFlag = true;

    if (increment)
    {
        if (MenuItem == 6)
            MenuItem = 0;
        else
            MenuItem++;
    }
    else
    {
        if (MenuItem == 0)
            MenuItem = 6;
        else
            MenuItem--;
    }
    UpdateEEPROM();
}
