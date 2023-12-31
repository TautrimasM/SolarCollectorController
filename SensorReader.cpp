#include "Arduino.h"
#include "SensorReader.h"
#include "src/DallasTemperature/DallasTemperature.h"
#include "Defines.h"

extern float solarCollectorTemperature;
extern float heatExchangerTemperature;
extern float boilerTemperature;

extern unsigned long sensorFailTime;

extern bool sensorErrorFlag;
extern bool logicEvent;
extern bool sensorErrorForLongTime;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int i = 0, j = 0;

void InitSensors()
{
    sensors.setResolution(10); // 10-bit adc - 0.25C resolution
    sensors.begin();
}

void RequestSensors()
{

    if (i == 0)
    {
        sensors.requestTemperatures();
        i++;
    }

    else
    {
        ReadSensors();
        i = 0;
    }
}
void ReadSensors()
{
    // discard data if there are errors
    float temp1 = sensors.getTempCByIndex(ID_COLLECTOR_SENSOR);
    float temp2 = sensors.getTempCByIndex(ID_EXCHANGER_SENSOR);
    float temp3 = sensors.getTempCByIndex(ID_BOILER_SENSOR);

    if (temp1 < -50 || temp2 < -50 || temp3 < -50) // if oneWire fail, data corrupt, it returns -127, -50 is lowest temp for sensor
    {
        sensorErrorFlag = true;
        sensorFailTime = millis();

        j++;
        if (j >= (SENSOR_ERROR_LONG_TIME / REQUEST_SENSORS_INTERVAL))
        {
            sensorErrorForLongTime = true;
            logicEvent = true;
        }
    }
    else if (sensorErrorFlag) // skip one read to allow onewire bus devices to reconfigure after failure
    {
        sensorErrorFlag = false;
    }
    else
    {
        solarCollectorTemperature = temp1;
        heatExchangerTemperature = temp2;
        boilerTemperature = temp3;
        logicEvent = true;
        sensorErrorForLongTime = false;
        j = 0;
    }

    if (
        // temp too low
        solarCollectorTemperature < -50 ||
        heatExchangerTemperature < 1 ||
        boilerTemperature < 1 ||
        // temp too high
        solarCollectorTemperature > 120 ||
        heatExchangerTemperature > 90 ||
        boilerTemperature > 80)
    {
        // danger temp warning
    }
    else
    {
        // danger temp warning clear
    }
}