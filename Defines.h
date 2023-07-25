#ifndef Defines
#define Defines

#include "Arduino.h"

#define REQUEST_SENSORS_INTERVAL 1000 // 1s should be ok, it takes 750ms to do temp calculation
#define REFRESH_SCREEN_INTERVAL 5000
#define PET_WATCHDOG_INTERVAL 1000
#define READ_BUTTONS_INTERVAL 50
#define READ_BUTTONS_INTERVAL_WHEN_PRESSED 500

#define DEGASSING_TIME 600000         // 10min
#define ACTIVITY_TIME 10000           // 10s
#define BOILER_DELAY_TIME 120000      // 2min
#define SENSOR_ERROR_LONG_TIME 600000 // 10min

// degrees C
#define TEMPERATURE_INCREMENT 0.5
#define DELTA_MAX 40
#define DELTA_MIN 1
#define HYSTERESIS_MAX 39
#define HYSTERESIS_MIN 0.5
#define HALT_TEMP_MAX 60
#define HALT_TEMP_MIN 0

// set these according to existing sensors configuration
#define ID_COLLECTOR_SENSOR 0
#define ID_EXCHANGER_SENSOR 2
#define ID_BOILER_SENSOR 1

// physical objects connection pins
#define ONE_WIRE_BUS 2 // 15m line works ok witk ~1.5k pullup resistor and 150ohm resistors for each branch

#define COLLECTOR_PUMP_PIN 10
#define BOILER_PUMP_PIN 11
#define DEGASSING_VALVE_PIN 12

#define LEFT_BUTTON_PIN 7
#define MID_BUTTON_PIN 8
#define RIGHT_BUTTON_PIN 9

#endif
