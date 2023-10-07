 /*!
  * @file  TemperatureProbe.h
  * @brief This is a header file of the library for the K30 CO2 Sensor
  * @copyright   Copyright (c) 2023 Sahil Harriram Tech Pty Ltd.
  * @license     The MIT License (MIT)
  * @author      Sahil Harriram (Hello@sahil.harriram.tech)
  * @version     V0.0.1
  * @date        2023-09-02 
*/
#ifndef __K30_SENSOR_PROBE_H__
#define __K30_SENSOR_PROBE_H__

#include "Arduino.h"

class K30Sensor {
public:
    // Constructor: initializes the class with the I2C address of the CO2 sensor.
    K30Sensor();

    // Setup function for initializing sensor and I2C communication.
    void init();

    // Function to read CO2 concentration from the sensor.
    uint16_t read();

private:
 
 
};
 
 
#endif //__K30_SENSOR_PROBE_H__