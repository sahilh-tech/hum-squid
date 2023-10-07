/*!
  * @file  TemperatureProbe.h
  * @brief This is a header file of the library for Analog Temp Probe Sensor 
  * @copyright   Copyright (c) 2023 Sahil Harriram Tech Pty Ltd.
  * @license     The MIT License (MIT)
  * @author      Sahil Harriram (Hello@sahil.harriram.tech)
  * @version     V0.0.1
  * @date        2023-09-02 
*/
#ifndef __TEMPERATURE_PROBE_H__
#define __TEMPERATURE_PROBE_H__

#include "Arduino.h"

class TemperatureProbe {
public:
    // Constructor: initializes the class with the I2C address of the CO2 sensor.
    TemperatureProbe(uint8_t probeAdcPin);

    // Setup function for initializing sensor and I2C communication.
    void init();

    // Function to read CO2 concentration from the sensor.
    uint16_t read();

private:
  uint8_t mProbeAdcPin;
  // Thermistor Parameters
  const float mFixedResistor = 10000.0; // fixed external resistor
  const float mMaxADCValue = 4095.0;    // maximum ADC value for 12 bit resistor
  const float mTempInKelvin = 273.15;   // Temp in Kelvin based on external resistance
  const float mBeta = 3977.0;           // Beta coefficient, corrected value
  const float mR0 = 10000.0;            // Resistance at 25C
  const float mT0 = 298.15;             // Temperature at 25C in Kelvin
 
};
 
 
#endif // __TEMPERATURE_PROBE_H__