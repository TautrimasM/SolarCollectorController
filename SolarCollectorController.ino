#include "src/Relay.h"
#include "src/Button.h"

#include "SensorReader.h"
#include "ScreenUpdater.h"
#include "ButtonReader.h"
#include "WatchdogPet.h"
#include "LogicMaker.h"
#include "EEPROMFunctions.h"

#define REQUEST_SENSORS_INTERVAL 1000 // 1s should be ok
#define REFRESH_SCREEN_INTERVAL 5000
#define PET_WATCHDOG_INTERVAL 1000
// these probably should be #define, but i did't manage to get them working in that case
unsigned long ReadButtonsInterval = 50;
unsigned long DegassingTime = 600000; // 600s or 10mins
unsigned long ActivityTime = 10000;   // 10s
unsigned long BoilerWaitTime = 90000; // 90s

// set these according to existing sensors configuration
uint8_t CollectorSensorId = 0;
uint8_t ExchangerSensorId = 2;
uint8_t BoilerSensorId = 1;

// variables for storing timing data
unsigned long currentMillis;
unsigned long RequestSensorsTime = 0;
unsigned long RefreshScreenTime = 0;
unsigned long ReadButtonsTime = 0;
unsigned long PetWatchdogTime = 0;
unsigned long DegassingTimeStart = 0;
unsigned long BacklightTimeStart = 0;
unsigned long ActivityTimeStart = 0;
unsigned long BoilerWaitTimeStart = 0;

// these variables values will be read from sensors
float SolarCollectorTemperature = 0;
float HeatExchangerTemperature = 0;
float BoilerTemperature = 0;

// these variables will be read from eeprom and/or set by user
float DeltaCollectorExchanger = 0;
float DeltaExchangerBoiler = 0;
float HysteresisCollectorExchanger = 0;
float HysteresisExchangerBoiler = 0;
float HaltTemperature = 0;

// event flags
bool RefreshScreenEvent = true;
bool LogicEvent = true;
bool DegassingFlag = false;
bool SensorErrorFlag = false;
bool BacklightOn = true;
bool ActivityFlag = false;
bool BoilerWaitFlag = false;

uint8_t MenuItem = 0;

// physical objects
Relay CollectorPump(10);
Relay BoilerPump(11);
Relay DegassingValve(12);

Button LeftButton(7);
Button MidButton(8);
Button RightButton(9);

void setup()
{
  InitWatchdog();
  InitDisplay();
  StartupScreen();
  InitSensors();
  ReadEEPROM();
  RequestSensors(); // request temps conversion from sensors
  PetWatchdog();
  delay(750);
  RequestSensors(); // get temps from sensor
}

void loop()
{
  currentMillis = millis();

  if (currentMillis - RequestSensorsTime >= REQUEST_SENSORS_INTERVAL)
  {
    RequestSensorsTime = currentMillis;
    RequestSensors();
  }

  if (currentMillis - RefreshScreenTime >= REFRESH_SCREEN_INTERVAL || RefreshScreenEvent)
  {

    RefreshScreenEvent = false;
    RefreshScreenTime = currentMillis;
    UpdateScreen();
  }

  if (currentMillis - ReadButtonsTime >= ReadButtonsInterval)
  {
    ReadButtonsTime = currentMillis;
    ReadButtons();
  }

  if (currentMillis - PetWatchdogTime >= PET_WATCHDOG_INTERVAL)
  {
    PetWatchdogTime = currentMillis;
    PetWatchdog();
  }

  if (LogicEvent)
  {
    DoLogic();
  }
}
