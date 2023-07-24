#ifndef Defines
#define Defines

#include "Arduino.h"

#define REQUEST_SENSORS_INTERVAL 1000 // 1s should be ok, it takes 750ms to do temp calculation
#define REFRESH_SCREEN_INTERVAL 5000
#define PET_WATCHDOG_INTERVAL 1000
#define READ_BUTTONS_INTERVAL 50
#define READ_BUTTONS_INTERVAL_WHEN_PRESSED 500

#define DEGASSING_TIME 600000    // 10min
#define ACTIVITY_TIME 10000      // 10s
#define BOILER_DELAY_TIME 120000 // 2min

// set these according to existing sensors configuration
#define ID_COLLECTOR_SENSOR 0
#define ID_EXCHANGER_SENSOR 2
#define ID_BOILER_SENSOR 1

#endif
