#include "Arduino.h"
#include "EEPROMFunctions.h"
#include "src/EEPROM/EEPROM.h"

extern float DeltaCollectorExchanger;
extern float DeltaExchangerBoiler;
extern float HysteresisCollectorExchanger;
extern float HysteresisExchangerBoiler;
extern float HaltTemperature;
// float takes 4 bytes

void UpdateEEPROM()
{
    EEPROM.put(0, DeltaCollectorExchanger);
    EEPROM.put(4, DeltaExchangerBoiler);
    EEPROM.put(8, HysteresisCollectorExchanger);
    EEPROM.put(12, HysteresisExchangerBoiler);
    EEPROM.put(16, HaltTemperature);
}
void ReadEEPROM()
{
    EEPROM.get(0, DeltaCollectorExchanger);
    EEPROM.get(4, DeltaExchangerBoiler);
    EEPROM.get(8, HysteresisCollectorExchanger);
    EEPROM.get(12, HysteresisExchangerBoiler);
    EEPROM.get(16, HaltTemperature);
}