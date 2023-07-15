#include "Arduino.h"
#include "src/LCI2C/LiquidCrystal_I2C.h"
#include "ScreenUpdater.h"
#include "src/Relay.h"
#include "src/Button.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

extern uint8_t MenuItem;
/*
 0 - Info menu
 1 - DeltaCollectorExchanger
 2 - HysterysisCollectorExchanger
 3 - DeltaExchangerBoiler
 4 - HysterysisExchangerBoiler
 5 - Degassing valve Relay
*/

extern float SolarCollectorTemperature;
extern float HeatExchangerTemperature;
extern float BoilerTemperature;

extern float DeltaCollectorExchanger;
extern float DeltaExchangerBoiler;

extern float HysteresisCollectorExchanger;
extern float HysteresisExchangerBoiler;

extern float HaltTemperature;

extern Relay CollectorPump;
extern Relay BoilerPump;
extern Relay DegassingValve;

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
    switch (MenuItem)
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
        LowestTempMenu();
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
    lcd.print(String(SolarCollectorTemperature, 1));
    lcd.write(byte(0));
    lcd.setCursor(12, 0);
    lcd.write(byte(1));
    lcd.print("sk");
    lcd.print("=");
    lcd.print(String(DeltaCollectorExchanger, 1));
    lcd.write(byte(0));

    // line 1
    lcd.setCursor(0, 1);
    lcd.print("                    ");
    lcd.setCursor(0, 1);
    lcd.print("Tk=");
    lcd.print(String(HeatExchangerTemperature, 1));
    lcd.write(byte(0));
    lcd.setCursor(12, 1);
    lcd.write(byte(1));
    lcd.print("kb");
    lcd.print("=");
    lcd.print(String(DeltaExchangerBoiler, 1));
    lcd.write(byte(0));

    // line 2

    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 2);
    lcd.print("Tb=");
    lcd.print(String(BoilerTemperature, 1));
    lcd.write(byte(0));

    // line 3
    lcd.setCursor(0, 3);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("Ss=");
    lcd.print(CollectorPump.getStateString());
    lcd.print(" ");
    lcd.print("Sb=");
    lcd.print(BoilerPump.getStateString());
    lcd.print(" DG=");
    lcd.print(DegassingValve.getStateString());
}

void DeltaCollectorExchangerMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(1));
    lcd.setCursor(2, 0);
    lcd.print("S. Kol. - Keit.: ");
    lcd.setCursor(2, 1);
    lcd.print(String(DeltaCollectorExchanger, 1));
    lcd.write(byte(0));
}

void HysteresisCollectorExchangerMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hyst. Kol. - Keit.: ");
    lcd.setCursor(2, 1);
    lcd.print(String(HysteresisCollectorExchanger, 1));
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
    lcd.print(String(DeltaExchangerBoiler, 1));
    lcd.write(byte(0));
}

void HysterysisExchangerBoilerMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hyst. Keit. - Boil.: ");
    lcd.setCursor(2, 1);
    lcd.print(String(HysteresisExchangerBoiler, 1));
    lcd.write(byte(0));
}

void DegassVaveMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nuorinimo ventilis");
    lcd.setCursor(2, 1);
    lcd.print(DegassingValve.getStateString());
}

void LowestTempMenu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Atjungimo Temp.:");
    lcd.setCursor(2, 1);
    lcd.print(String(HaltTemperature, 1));
    lcd.write(byte(0));
}

void StartupScreen()
{
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("*System is Starting*");
    lcd.setCursor(8, 2);
    lcd.print("----");
}