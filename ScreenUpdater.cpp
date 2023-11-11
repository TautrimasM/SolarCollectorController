#include "Arduino.h"
#include "src/LCI2C/LiquidCrystal_I2C.h"
#include "ScreenUpdater.h"
#include "src/Relay.h"
#include "src/DigitalInput.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

extern uint8_t menuItem;
/*
 0 - Info menu
 1 - DeltaCollectorExchanger
 2 - HysterysisCollectorExchanger
 3 - DeltaExchangerBoiler
 4 - HysterysisExchangerBoiler
 5 - Degassing valve Relay
*/

extern float solarCollectorTemperature;
extern float heatExchangerTemperature;
extern float boilerTemperature;

extern float deltaCollectorExchanger;
extern float deltaExchangerBoiler;

extern float hysteresisCollectorExchanger;
extern float hysteresisExchangerBoiler;

extern float haltTemperature;

extern unsigned long sensorFailTime;
extern bool sensorErrorForLongTime;
extern bool systemHalt;
extern bool forceHalt;

extern unsigned long auxHeatingDelayTime;
extern unsigned long auxHeatingStopTime;
extern bool auxHeatingWasOn;
extern DigitalInput auxHeatingInput;

extern Relay collectorPump;
extern Relay boilerPump;
extern Relay degassingValve;

byte degreesCelsius[8] = {
    0b11000,
    0b11000,
    0b00010,
    0b00101,
    0b00100,
    0b00101,
    0b00010,
    0b00000};

byte delta[8] = {
    0b00000,
    0b00100,
    0b01010,
    0b11011,
    0b10001,
    0b11111,
    0b00000,
    0b00000};

void InitDisplay()
{
    lcd.begin();
    lcd.backlight();
    lcd.createChar(0, degreesCelsius);
    lcd.createChar(1, delta);
}

void UpdateScreen()
{
    switch (menuItem)
    {
    case 0:
        InfoMenu();
        break;
    case 1:
        DeltaCollectorExchangerMenu();
        break;
    case 2:
        HysteresisCollectorExchangerMenu();
        break;
    case 3:
        DeltaExchangerBoilerMenu();
        break;
    case 4:
        HysterysisExchangerBoilerMenu();
        break;
    case 5:
        DegassVaveMenu();
        break;
    case 6:
        ForceHaltMenu();
        break;
    case 7:
        LowestTempMenu();
        break;
    case 8:
        DelayAfterAuxHeatingMenu();
        break;
    default:
        InfoMenu();
        break;
    }
}

void InfoMenu()
{
    // line 0
    lcd.setCursor(0, 0);
    lcd.print("                    ");
    lcd.setCursor(0, 0);
    lcd.print("Ts=");
    lcd.print(String(solarCollectorTemperature, 1));
    lcd.write(byte(0));
    lcd.setCursor(12, 0);
    lcd.write(byte(1));
    lcd.print("sk");
    lcd.print("=");
    lcd.print(String(deltaCollectorExchanger, 1));
    lcd.write(byte(0));

    // line 1
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    lcd.setCursor(0, 1);
    lcd.print("Tk=");
    lcd.print(String(heatExchangerTemperature, 1));
    lcd.write(byte(0));
    lcd.setCursor(12, 1);
    lcd.write(byte(1));
    lcd.print("kb");
    lcd.print("=");
    lcd.print(String(deltaExchangerBoiler, 1));
    lcd.write(byte(0));

    // line 2
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 2);
    lcd.print("Tb=");
    lcd.print(String(boilerTemperature, 1));
    lcd.write(byte(0));

    // diagnostic data:
    lcd.print(" RT");
    if (millis() < 86400000)
    {
        if (millis() < 3600000)
        {
            if (millis() < 60000)
            {
                uint8_t runs = millis() / 1000;
                lcd.print(runs);
                lcd.print("s");
            }
            else
            {
                uint8_t runmin = millis() / 60000;
                lcd.print(runmin);
                lcd.print("m");
            }
        }
        else
        {
            uint8_t runh = millis() / 3600000;
            lcd.print(runh);
            lcd.print("h");
        }
    }
    else
    {
        uint8_t rundays = millis() / 86400000;
        lcd.print(rundays);
        lcd.print("d");
    }
    lcd.print(" SK");
    if ((millis() - sensorFailTime) < 86400000)
    {
        if ((millis() - sensorFailTime) < 3600000)
        {
            if ((millis() - sensorFailTime) < 60000)
            {
                uint8_t fs = (millis() - sensorFailTime) / 1000;
                lcd.print(fs);
                lcd.print("s");
            }
            else
            {
                uint8_t fmin = (millis() - sensorFailTime) / 60000;
                lcd.print(fmin);
                lcd.print("m");
            }
        }
        else
        {
            uint8_t fh = (millis() - sensorFailTime) / 3600000;
            lcd.print(fh);
            lcd.print("h");
        }
    }
    else
    {
        uint8_t fdays = (millis() - sensorFailTime) / 86400000;
        lcd.print(fdays);
        lcd.print("d");
    }

    // line 3
    if (systemHalt)
    {
        lcd.setCursor(0, 3);
        lcd.print("Sys. Stopped ");
    }
    else
    {
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("Ss=");
        lcd.print(collectorPump.getStateString());
        lcd.print(" ");

        if (auxHeatingWasOn || auxHeatingInput.isPressed())
        {
            if (auxHeatingInput.isPressed())
            {
                lcd.print("BT=ON");
            }
            else
            {
                lcd.print("BT=OFF ");
                lcd.print((millis() - auxHeatingStopTime) / 60000);
                lcd.print("min");
            }
        }
        else
        {
            lcd.print("Sb=");
            lcd.print(boilerPump.getStateString());
        }
    }

    lcd.print(" DG=");
    lcd.print(degassingValve.getStateString());

    if (sensorErrorForLongTime)
    {
        String err = "Err";
        lcd.setCursor(3, 0);
        lcd.print(err);
        lcd.setCursor(3, 1);
        lcd.print(err);
        lcd.setCursor(3, 2);
        lcd.print(err);
    }
}

void DeltaCollectorExchangerMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(1));
    lcd.setCursor(2, 0);
    lcd.print("S. Kol. - Keit.: ");
    lcd.setCursor(2, 1);
    lcd.print(String(deltaCollectorExchanger, 1));
    lcd.write(byte(0));
}

void HysteresisCollectorExchangerMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hyst. Kol. - Keit.: ");
    lcd.setCursor(2, 1);
    lcd.print(String(hysteresisCollectorExchanger, 1));
    lcd.write(byte(0));
}

void DeltaExchangerBoilerMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(1));
    lcd.setCursor(2, 0);
    lcd.print("Keit. - Boil.: ");
    lcd.setCursor(2, 1);
    lcd.print(String(deltaExchangerBoiler, 1));
    lcd.write(byte(0));
}

void HysterysisExchangerBoilerMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hyst. Keit. - Boil.: ");
    lcd.setCursor(2, 1);
    lcd.print(String(hysteresisExchangerBoiler, 1));
    lcd.write(byte(0));
}

void DegassVaveMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nuorinimo ventilis");
    lcd.setCursor(2, 1);
    lcd.print(degassingValve.getStateString());
}

void ForceHaltMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sistemos stabdymas");
    lcd.setCursor(2, 1);
    lcd.print(forceHalt ? "STOP" : "RUN");
}

void LowestTempMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Atjungimo Temp.:");
    lcd.setCursor(2, 1);
    lcd.print(String(haltTemperature, 1));
    lcd.write(byte(0));
}

void DelayAfterAuxHeatingMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Delsimas po");
    lcd.setCursor(1, 1);
    lcd.print("sildymo tenu:");
    lcd.setCursor(2, 2);
    lcd.print(auxHeatingDelayTime / 60000);
    lcd.print("min");
}
void StartupScreen()
{
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("*System is Starting*");
    lcd.setCursor(8, 2);
    lcd.print("----");
}
