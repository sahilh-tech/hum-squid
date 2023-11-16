#include "TemperatureProbe.h"
#include <math.h>

// Your sensorData structure definition here, or include the header that has it

TemperatureProbe::TemperatureProbe( sensorData& squidData, uint8_t i2cAddress)
: mSquidData(squidData), mAdc(i2cAddress)
{
}

void TemperatureProbe::init()
{
    mAdc.begin();
    mAdc.setMode(1);               // SINGLE SHOT MODE
    mAdc.setGain(1);  // Set gain to 4.096V
}

void TemperatureProbe::printRawData(){
    int16_t rawData = 0;
    for (int i = 0; i < 3; i++)
    {   
        mAdc.requestADC(i);       // request new conversion
        delay(10);  // Small delay for more accurate analog reading
        rawData = mAdc.getValue(); 
        Serial.print("raw Channel ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(rawData);
    } 
} 
/* 
 min value -50 degrees,
 max value 105
 min ADC = 0
 max ADC = 32767
 108
*/ 

float TemperatureProbe::read(uint8_t channel)
{
    float R, temperature;
    int16_t Vo;
    long sumVo = 0;
    int validReadings = 0;
    long interimValue = 0;
    const float threshold = 2000;  // Set a threshold for deviation

    // Determine the baseline temperature based on the channel
    float baselineTemp = 0;

    switch(channel) {
        case 0: baselineTemp = mSquidData.soilTempProbe1; break;
        case 1: baselineTemp = mSquidData.soilTempProbe2; break;
        case 2: baselineTemp = mSquidData.soilTempProbe3; break;
    }
    Serial.print(" baselineTemp = ");
    Serial.println(baselineTemp);
    for (int i = 0; i < mNumSamples; i++)
    {   
        mAdc.requestADC(channel);       // request new conversion
        delay(10);  // Small delay for more accurate analog reading
        interimValue = mAdc.getValue(); 
        Serial.print("i = ");
        Serial.print(i);
        Serial.print(" An R val = ");
        Serial.println(interimValue);

        // If baselineTemp is 0, use the first reading as the baseline
        if (baselineTemp == 0 && validReadings == 0) {
            baselineTemp = interimValue;
        }

        // Check if the reading is within the acceptable range
        if (abs(interimValue - baselineTemp) < threshold || validReadings == 0)
        {
            sumVo += interimValue;
            validReadings++;
        }
    }

    // Calculate final average only if there are valid readings
    Vo = validReadings > 0 ? sumVo / validReadings : 0;

    R = mFixedResistor / ((mMaxADCValue / Vo) - 1); 
    temperature = mBeta / (log(R / mR0) + (mBeta / mT0)) - mTempInKelvin;

    Serial.print("Vo =  ");
    Serial.println(Vo);

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