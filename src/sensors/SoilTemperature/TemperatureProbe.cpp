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

// which analog pin to connect
#define THERMISTORPIN A0         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3977
// the value of the 'other' resistor
#define SERIESRESISTOR 8500  

float TemperatureProbe::read(uint8_t channel)
{
//     int samples[NUMSAMPLES];
//     uint8_t i;
//   float average;

//   // take N samples in a row, with a slight delay
//   for (i=0; i< NUMSAMPLES; i++) {
//         mAdc.requestADC(0);       // request new conversion
//         delay(10);  // Small delay for more accurate analog reading
//         samples[i] = mAdc.getValue(); 

//   }
  
//   // average all the samples out
//   average = 0;
//   for (i=0; i< NUMSAMPLES; i++) {
//      average += samples[i];
//   }
//   average /= NUMSAMPLES;

//   Serial.print("Average analog reading "); 
//   Serial.println(average);
  
//   // convert the value to resistance
//   average = 32767 / average - 1;
//   average = SERIESRESISTOR / average;
//   Serial.print("Thermistor resistance "); 
//   Serial.println(average);
  
//   float steinhart;
//   steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
//   steinhart = log(steinhart);                  // ln(R/Ro)
//   steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
//   steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
//   steinhart = 1.0 / steinhart;                 // Invert
//   steinhart -= 273.15;                         // convert absolute temp to C
  
//   Serial.print("Temperature "); 
//   Serial.print(steinhart);
//   Serial.println(" *C");
//   delay(1000);
//   return steinhart;


    float R, temperature;
    int16_t Vo;
    long sumVo = 0;

    for (int i = 0; i < mNumSamples; i++)
    {   
        mAdc.requestADC(channel);       // request new conversion
        delay(10);  // Small delay for more accurate analog reading
        sumVo += mAdc.getValue(); 
    }

    Vo = sumVo / mNumSamples;

    R = mFixedResistor / ((mMaxADCValue / Vo) - 1); 
 
    temperature = mBeta / (log(R / mR0) + (mBeta / mT0)) - mTempInKelvin;

    Serial.print("Vo =  ");
    Serial.println(Vo);
    Serial.print("R = ");
    Serial.println(R);
    Serial.print("t = ");
    Serial.println(temperature);
    delay(1000);

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