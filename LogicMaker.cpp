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
extern Button auxHeatingInput;

extern LiquidCrystal_I2C lcd;

extern uint8_t menuItem;
extern unsigned long boilerPumpDelay;

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

extern bool boilerSideAllowedToOperate;
extern unsigned long auxHeatingStopTime;
extern unsigned long auxHeatingDelayTime;
extern bool auxHeatingWasOn;

void DoLogic()
{
    SetBoilerDelay();
    CheckAuxHeater();

    if (solarCollectorTemperature > haltTemperature && !sensorErrorForLongTime)
    {
        CollectorPumpLogic();
        BoilerPumpLogic();
    }
    else
    {
        HaltSystem();
    }

    CheckDegassingValve();

    CheckUserActivity();
}

void CheckUserActivity()
{
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
void CheckSensorError()
{
    if (sensorErrorFlag)
    {
        // backlightOn = true;
    }
}
void CheckBacklight()
{
    if (backlightOn) // auto turn of backlight after set time
    {
        lcd.backlight();
        backlightTimeStart = millis();
        backlightOn = false;
    }
    if (millis() - backlightTimeStart >= ACTIVITY_TIME)
    {
        lcd.noBacklight();
    }
}
void CheckAuxHeater()
{
    if (auxHeatingInput.isPressed()) // auto turn of backlight after set time
    {
        boilerSideAllowedToOperate = false;
        auxHeatingStopTime = millis();
        auxHeatingWasOn = true;
        refreshScreenEvent = true;
    }

    if ((!auxHeatingInput.isPressed() && !auxHeatingWasOn) || (auxHeatingWasOn && millis() - auxHeatingStopTime >= auxHeatingDelayTime + 1000))
    {
        boilerSideAllowedToOperate = true;
        auxHeatingWasOn = false;
        refreshScreenEvent = true;
    }
}
void CheckDegassingValve()
{
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
}
void HaltSystem()
{
    collectorPump.off();
    HaltBoilerSide(false);
}

void CollectorPumpLogic()
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
}

void BoilerPumpLogic()
{
    if (boilerSideAllowedToOperate)
    {

        if (boilerPump.getState())
        {
            if (heatExchangerTemperature - boilerTemperature < deltaExchangerBoiler - hysteresisExchangerBoiler)
            {
                HaltBoilerSide(false);
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
            else if (millis() - boilerWaitTimeStart >= boilerPumpDelay)
            {
                boilerPump.on();
                boilerWaitFlag = false;
                refreshScreenEvent = true;
            }
        }
    }
    else
    {
        HaltBoilerSide(true);
    }
}

void HaltBoilerSide(bool forceDegassClose)
{
    boilerPumpShouldDoSomething = false;
    refreshScreenEvent = true;
    boilerPump.off();
    if (!degassingInProgress || forceDegassClose)
    {
        degassingValve.off();
    }
}

void SetBoilerDelay() // sets boiler pump delay after degass valve opens
{
    if (boilerTemperature > BOILER_COLD_TEMP)
    {
        boilerPumpDelay = BOILER_DELAY_TIME;
    }
    else
    {
        boilerPumpDelay = BOILER_DELAY_TIME_COLD;
    }
}
