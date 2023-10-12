#include "TemperatureProbe.h"
#include <math.h>

// Your sensorData structure definition here, or include the header that has it

TemperatureProbe::TemperatureProbe(uint8_t i2cAddress, sensorData& squidData)
: mAdc(i2cAddress), mSquidData(squidData)
{
}

void TemperatureProbe::init()
{
    mAdc.begin();
    mAdc.setGain(1);  // Set gain to 4.096V
}

float TemperatureProbe::read(uint8_t channel)
{
    float R, temperature;
    int16_t Vo;
    long sumVo = 0;

    for (int i = 0; i < mNumSamples; i++)
    {
        sumVo += mAdc.readADC(channel);
        delay(10);  // Small delay for more accurate analog reading
    }

    Vo = sumVo / mNumSamples;

    R = mFixedResistor * ((mMaxADCValue / Vo) - 1);
    temperature = mBeta / (log(R / mR0) + (mBeta / mT0)) - mTempInKelvin;

    return round(temperature * 100.0) / 100.0;  // Round to 2 decimal places
}

void TemperatureProbe::updateSoilTemperatureData()
{
    mSquidData.soilTempProbe1 = read(0);
    mSquidData.soilTempProbe2 = read(1);
    mSquidData.soilTempProbe3 = read(2);
}

void TemperatureProbe::printProbeData(uint8_t probeIndex)
{
    float temp = read(probeIndex);
    Serial.print("Temp Probe ");
    Serial.print(probeIndex);
    Serial.print(" = ");
    Serial.println(temp, 2);
}

void TemperatureProbe::printAllProbeData()
{
    for (uint8_t i = 0; i < 3; i++)
    {
        printProbeData(i);
    }
}