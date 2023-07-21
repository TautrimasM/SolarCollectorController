#include "Arduino.h"
#include "SensorReader.h"
#include "src/DallasTemperature/DallasTemperature.h"

#define ONE_WIRE_BUS 2
// 15m line works ok witk ~1.5k pullup resistor

extern uint8_t CollectorSensorId;
extern uint8_t ExchangerSensorId;
extern uint8_t BoilerSensorId;

extern float SolarCollectorTemperature;
extern float HeatExchangerTemperature;
extern float BoilerTemperature;

extern unsigned long SensorFailTime;

extern bool SensorErrorFlag;

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
int i = 0;

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

    SolarCollectorTemperature = sensors.getTempCByIndex(CollectorSensorId);
    HeatExchangerTemperature = sensors.getTempCByIndex(ExchangerSensorId);
    BoilerTemperature = sensors.getTempCByIndex(BoilerSensorId);

    if (
        // temp too low or onewire failure
        SolarCollectorTemperature < -50 ||
        HeatExchangerTemperature < 1 ||
        BoilerTemperature < 1 ||
        // temp too high
        SolarCollectorTemperature > 120 ||
        HeatExchangerTemperature > 90 ||
        BoilerTemperature > 80)
    {
        SensorErrorFlag = true;
        SensorFailTime = millis();
    }
    else
    {
        SensorErrorFlag = false;
    }
}