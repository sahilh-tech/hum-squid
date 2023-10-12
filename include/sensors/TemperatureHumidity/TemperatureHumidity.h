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

#include <Arduino.h>
#include "config.h"
#include <Wire.h>
#include "DFRobot_SHT20.h"  // Include the SHT20 library

 

class TemperatureHumidity {
public:
  TemperatureHumidity(TwoWire &wire, sensorData &data);
  void init();
  float readTemp();
  uint16_t readHumidity(); // returns humidity in percentage with 2 decimal places (multiplied by 100)
  void updateTempAndHumidity();
  void printTemp();
  void printHumidity();
  void printAll();

private:
  DFRobot_SHT20 mSHT20;
  sensorData &mSquidData;
};
 
#endif // __TEMPERATURE_HUMIDITY_H__