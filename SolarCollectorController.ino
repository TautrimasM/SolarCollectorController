#include "src/Relay.h"
#include "src/Button.h"

#include "Defines.h"
#include "SensorReader.h"
#include "ScreenUpdater.h"
#include "ButtonReader.h"
#include "WatchdogPet.h"
#include "LogicMaker.h"
#include "EEPROMFunctions.h"

// variables for storing timing data
unsigned long currentMillis = 0;
unsigned long requestSensorsTime = 0;
unsigned long refreshScreenTime = 0;
unsigned long readButtonsTime = 0;
unsigned long petWatchdogTime = 0;
unsigned long degassingTimeStart = 0;
unsigned long backlightTimeStart = 0;
unsigned long activityTimeStart = 0;
unsigned long boilerWaitTimeStart = 0;
unsigned long sensorFailTime = 0;
unsigned long readButtonsInterval = READ_BUTTONS_INTERVAL;

// these variables values will be read from sensors
float solarCollectorTemperature = 0;
float heatExchangerTemperature = 0;
float boilerTemperature = 0;

// these variables will be read from eeprom and/or set by user
float deltaCollectorExchanger = 0;
float deltaExchangerBoiler = 0;
float hysteresisCollectorExchanger = 0;
float hysteresisExchangerBoiler = 0;
float haltTemperature = 0;

// event flags
bool refreshScreenEvent = true;
bool logicEvent = false;
bool degassingFlag = false;
bool degassingInProgress = false;
bool sensorErrorFlag = false;
bool backlightOn = true;
bool activityFlag = false;
bool boilerWaitFlag = false;
bool boilerPumpShouldDoSomething = false;
bool sensorErrorForLongTime = false;

uint8_t menuItem = 0;

// physical objects
Relay collectorPump(COLLECTOR_PUMP_PIN);
Relay boilerPump(BOILER_PUMP_PIN);
Relay degassingValve(DEGASSING_VALVE_PIN);

Button leftButton(LEFT_BUTTON_PIN);
Button midButton(MID_BUTTON_PIN);
Button rightButton(RIGHT_BUTTON_PIN);

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

  if (currentMillis - requestSensorsTime >= REQUEST_SENSORS_INTERVAL)
  {
    requestSensorsTime = currentMillis;
    RequestSensors();
  }

  if (currentMillis - refreshScreenTime >= REFRESH_SCREEN_INTERVAL || refreshScreenEvent)
  {
    refreshScreenEvent = false;
    refreshScreenTime = currentMillis;
    UpdateScreen();
  }

  if (currentMillis - readButtonsTime >= readButtonsInterval)
  {
    readButtonsTime = currentMillis;
    ReadButtons();
  }

  if (currentMillis - petWatchdogTime >= PET_WATCHDOG_INTERVAL)
  {
    petWatchdogTime = currentMillis;
    PetWatchdog();
  }

  if (logicEvent)
  {
    logicEvent = false;
    DoLogic();
  }
}
