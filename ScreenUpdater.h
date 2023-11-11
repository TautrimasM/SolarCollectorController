#ifndef ScreenUpdater
#define ScreenUpdater

#include "Arduino.h"

void InitDisplay();
void UpdateScreen();

void InfoMenu();
void DeltaCollectorExchangerMenu();
void HysteresisCollectorExchangerMenu();
void DeltaExchangerBoilerMenu();
void HysterysisExchangerBoilerMenu();
void DegassVaveMenu();
void ForceHaltMenu();
void LowestTempMenu();
void DelayAfterAuxHeatingMenu();
void StartupScreen();

#endif