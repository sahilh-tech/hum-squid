/*!
  * @file  TemperatureHumiditySensor.h
  * @brief This is a header file that contains functionality to read the ambient Temp & Humidity sensor
  * @copyright   Copyright (c) 2023 Sahil Harriram Tech Pty Ltd.
  * @license     The MIT License (MIT)
  * @author      Sahil Harriram (Hello@sahil.harriram.tech)
  * @version     V0.0.1
  * @date        2023-09-02 
*/
#ifndef __TEMPERATURE_HUMIDITY_H__
#define __TEMPERATURE_HUMIDITY_H__

#include "Arduino.h"
#include <DFRobot_SHT3x.h>

class TemperatureHumidity {
public:
    // Constructor: initializes the class with the I2C address of the CO2 sensor.
    // TemperatureProbe(uint8_t probeAdcPin);

    // Setup function for initializing sensor and I2C communication.
    void init();

    // Function to read CO2 concentration from the sensor.
    uint16_t read();

private:
  
 
 
};
 
#endif // __TEMPERATURE_HUMIDITY_H__