#include "Arduino.h"
#include "WatchdogPet.h"
#include <avr/wdt.h>

void InitWatchdog()
{
    wdt_disable();
    wdt_enable(WDTO_2S); // Enable WDT with a timeout of 2 seconds
}

void PetWatchdog()
{
    wdt_reset();
}