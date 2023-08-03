#include "Arduino.h"
#include "LogicMaker.h"
#include "EEPROMFunctions.h"
#include "src/Button.h"
#include "src/Relay.h"
#include "src/LCI2C/LiquidCrystal_I2C.h"
#include "Defines.h"

extern Relay collectorPump;
extern Relay boilerPump;
extern Relay degassingValve;

extern LiquidCrystal_I2C lcd;

extern uint8_t menuItem;

extern float solarCollectorTemperature;
extern float heatExchangerTemperature;
extern float boilerTemperature;

extern float deltaCollectorExchanger;
extern float deltaExchangerBoiler;

extern float hysteresisCollectorExchanger;
extern float hysteresisExchangerBoiler;

extern float haltTemperature;

extern bool degassingFlag;
extern bool degassingInProgress;
extern unsigned long degassingTimeStart;

extern bool backlightOn;
extern unsigned long backlightTimeStart;

extern bool activityFlag;
extern unsigned long activityTimeStart;

extern bool boilerPumpShouldDoSomething;
extern bool boilerWaitFlag;
extern unsigned long boilerWaitTimeStart;

extern bool sensorErrorFlag;
extern bool sensorErrorForLongTime;
extern bool refreshScreenEvent;

void DoLogic()
{
    if (solarCollectorTemperature > haltTemperature && !sensorErrorForLongTime)
    {

        if (collectorPump.getState())
        {
            if (solarCollectorTemperature - heatExchangerTemperature < deltaCollectorExchanger - hysteresisCollectorExchanger)
            {
                collectorPump.off();
                refreshScreenEvent = true;
            }
        }
        else
        {
            if (solarCollectorTemperature - heatExchangerTemperature >= deltaCollectorExchanger)
            {
                collectorPump.on();
                refreshScreenEvent = true;
            }
        }

        if (boilerPump.getState())
        {
            if (heatExchangerTemperature - boilerTemperature < deltaExchangerBoiler - hysteresisExchangerBoiler)
            {
                boilerPumpShouldDoSomething = false;
                refreshScreenEvent = true;
                boilerPump.off();
                if (!degassingInProgress)
                {
                    degassingValve.off();
                }
            }
        }
        else
        {
            if (heatExchangerTemperature - boilerTemperature >= deltaExchangerBoiler)
            {
                boilerPumpShouldDoSomething = true;
            }
        }
        if (boilerPumpShouldDoSomething && !boilerPump.getState())
        {
            degassingValve.on();
            if (!boilerWaitFlag)
            {
                boilerWaitTimeStart = millis();
                boilerWaitFlag = true;
            }
            else if (millis() - boilerWaitTimeStart >= BOILER_DELAY_TIME)
            {
                boilerPump.on();
                boilerWaitFlag = false;
                refreshScreenEvent = true;
            }
        }
    }
    else
    {
        boilerPumpShouldDoSomething = false;
        collectorPump.off();
        boilerPump.off();
        if (!degassingInProgress)
        {
            degassingValve.off();
        }
    }

    if (degassingFlag && !degassingInProgress)
    {
        degassingTimeStart = millis();
        degassingInProgress = true;
        degassingValve.on();
        refreshScreenEvent = true;
    }
    if ((!boilerPump.getState() && !boilerPumpShouldDoSomething) &&
        (!degassingFlag || (degassingInProgress && degassingValve.getState() && millis() - degassingTimeStart >= DEGASSING_TIME)))
    {
        degassingValve.off();
        refreshScreenEvent = true;
        degassingInProgress = false;
        degassingFlag = false;
    }

    /*if (sensorErrorFlag)
    {
        // backlightOn = true;
    }

    if (backlightOn) // auto turn of backlight after set time
    {
        lcd.backlight();
        backlightTimeStart = millis();
        backlightOn = false;
    }
    if (millis() - backlightTimeStart >= activityTime)
    {
        lcd.noBacklight();
    }
    */

    if (activityFlag)
    {
        activityTimeStart = millis();
        activityFlag = false;
    }
    if (millis() - activityTimeStart >= ACTIVITY_TIME)
    {
        menuItem = 0;
        UpdateEEPROM();
    }
}
