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
#include "config.h"
#include "Wire.h"

class K30Sensor {
public:
  K30Sensor(sensorData& squidData);
  uint16_t readSensorData(); // Model this from the existing Arduino sketch
  void printCO2Data();  // Print CO2 data
  void updateCO2Data(); // Update the CO2 data in the sensorData structure

private:
  uint8_t const CO2_ADDRESS = 0x68;
  sensorData& mSquidData;
};
 
 
#endif //__K30_SENSOR_PROBE_H__