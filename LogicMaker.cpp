#include "Arduino.h"
#include "LogicMaker.h"
#include "src/Button.h"
#include "src/Relay.h"
#include "src/LCI2C/LiquidCrystal_I2C.h"

extern Relay CollectorPump;
extern Relay BoilerPump;
extern Relay DegassingValve;

extern LiquidCrystal_I2C lcd;

extern uint8_t MenuItem;

extern float SolarCollectorTemperature;
extern float HeatExchangerTemperature;
extern float BoilerTemperature;

extern float DeltaCollectorExchanger;
extern float DeltaExchangerBoiler;

extern float HysteresisCollectorExchanger;
extern float HysteresisExchangerBoiler;

extern float HaltTemperature;

extern bool DegassingFlag;
extern unsigned long DegassingTimeStart;
extern unsigned long DegassingTime;

extern bool BacklightOn;
extern unsigned long BacklightTimeStart;

extern bool ActivityFlag;
extern unsigned long ActivityTime;
extern unsigned long ActivityTimeStart;

extern bool BoilerPumpShouldDoSomething;
extern bool BoilerWaitFlag;
extern unsigned long BoilerWaitTimeStart;
extern unsigned long BoilerWaitTime;

extern bool SensorErrorFlag;
extern bool RefreshScreenEvent;

void DoLogic()
{
    if (SolarCollectorTemperature > HaltTemperature)
    {

        if (CollectorPump.getState())
        {
            if (SolarCollectorTemperature - HeatExchangerTemperature < DeltaCollectorExchanger - HysteresisCollectorExchanger)
            {
                CollectorPump.off();
                RefreshScreenEvent = true;
            }
        }
        else
        {
            if (SolarCollectorTemperature - HeatExchangerTemperature >= DeltaCollectorExchanger)
            {
                CollectorPump.on();
                RefreshScreenEvent = true;
            }
        }

        if (BoilerPump.getState())
        {
            if (HeatExchangerTemperature - BoilerTemperature < DeltaExchangerBoiler - HysteresisExchangerBoiler)
            {
                BoilerPumpShouldDoSomething = false;
                BoilerPump.off();
                RefreshScreenEvent = true;
                if (!DegassingFlag)
                {
                    DegassingValve.off();
                }
            }
        }
        else
        {
            if (HeatExchangerTemperature - BoilerTemperature >= DeltaExchangerBoiler)
            {
                BoilerPumpShouldDoSomething = true;
            }
        }
        if (BoilerPumpShouldDoSomething)
        {
            DegassingValve.on();
            if (!BoilerWaitFlag)
            {
                BoilerWaitTimeStart = millis();
                BoilerWaitFlag = true;
                RefreshScreenEvent = true;
            }
            if (millis() - BoilerWaitTimeStart >= BoilerWaitTime)
            {
                BoilerPump.on();
                BoilerWaitFlag = false;
                RefreshScreenEvent = true;
            }
        }
    }
    else
    {
        CollectorPump.off();
        BoilerPump.off();
        if (!DegassingFlag)
        {
            DegassingValve.off();
        }
    }

    if (DegassingFlag)
    {
        DegassingTimeStart = millis();
        DegassingFlag = false;
        DegassingValve.on();
        RefreshScreenEvent = true;
    }
    if (DegassingValve.getState() && !BoilerPump.getState() && millis() - DegassingTimeStart >= DegassingTime)
    {
        DegassingValve.off();
        RefreshScreenEvent = true;
    }

    /*if (SensorErrorFlag)
    {
        // BacklightOn = true;
    }

    if (BacklightOn) // auto turn of backlight after set time
    {
        lcd.backlight();
        BacklightTimeStart = millis();
        BacklightOn = false;
    }
    if (millis() - BacklightTimeStart >= ActivityTime)
    {
        lcd.noBacklight();
    }
    */

    if (ActivityFlag)
    {
        ActivityTimeStart = millis();
        ActivityFlag = false;
    }
    if (millis() - ActivityTimeStart >= ActivityTime)
    {
        MenuItem = 0;
    }
}
