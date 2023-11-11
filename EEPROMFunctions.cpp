#include "Arduino.h"
#include "EEPROMFunctions.h"
#include "src/EEPROM/EEPROM.h"

extern float deltaCollectorExchanger;
extern float deltaExchangerBoiler;
extern float hysteresisCollectorExchanger;
extern float hysteresisExchangerBoiler;
extern float haltTemperature;
extern unsigned long auxHeatingDelayTime;
extern bool forceHalt;
// float takes 4 bytes
// long takes 4 bytes

void UpdateEEPROM()
{
    EEPROM.put(0, deltaCollectorExchanger);
    EEPROM.put(4, deltaExchangerBoiler);
    EEPROM.put(8, hysteresisCollectorExchanger);
    EEPROM.put(12, hysteresisExchangerBoiler);
    EEPROM.put(16, haltTemperature);
    EEPROM.put(20, auxHeatingDelayTime);
    EEPROM.put(24, forceHalt ? 1 : 0);
}
void ReadEEPROM()
{
    EEPROM.get(0, deltaCollectorExchanger);
    EEPROM.get(4, deltaExchangerBoiler);
    EEPROM.get(8, hysteresisCollectorExchanger);
    EEPROM.get(12, hysteresisExchangerBoiler);
    EEPROM.get(16, haltTemperature);
    EEPROM.get(20, auxHeatingDelayTime); // uncomment after first run

    byte temp = 0;
    EEPROM.get(24, temp);
    forceHalt = temp == 1 ? true : false;
}