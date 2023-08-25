#ifndef LogicMaker
#define LogicMaker

#include "Arduino.h"

void DoLogic();
void CheckUserActivity();
void CheckSensorError();
void CheckBacklight();
void CheckDegassingValve();
void CheckAuxHeater();
void HaltSystem();
void CollectorPumpLogic();
void BoilerPumpLogic();
void SetBoilerDelay();
void HaltBoilerSide(bool forceDegassClose);

#endif