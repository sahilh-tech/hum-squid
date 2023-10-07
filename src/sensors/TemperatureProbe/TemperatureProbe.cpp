
#include "TemperatureProbe.h"
//ToDo: get average of adc values over 10ms 
TemperatureProbe::TemperatureProbe(uint8_t probeAdcPin):  mProbeAdcPin(probeAdcPin)
{

}

void TemperatureProbe::init()
{
 
}

uint16_t TemperatureProbe::read()
{
    float R, temperature;
    int Vo;

    Vo = analogRead(mProbeAdcPin);
    R = mFixedResistor * ((mMaxADCValue / Vo) - 1);
    temperature = mBeta / (log(R / mR0) + (mBeta / mT0)) - 273.15;

    return temperature;
}
