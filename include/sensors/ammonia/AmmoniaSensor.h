/*!
  * @file  AmmoniaSensor.h
  * @brief This is a header file of the library for the Ammonia Sensor in the 
  * @copyright   Copyright (c) 2023 Sahil Harriram Tech Pty Ltd.
  * @license     The MIT License (MIT)
  * @author      Sahil Harriram (Hello@sahil.harriram.tech)
  * @version     V0.0.1
  * @date        2023-09-02 
*/
#ifndef __Ammonia_Sensor_H__
#define __Ammonia_Sensor_H__


#include "config.h"
#include <Wire.h>
#include "DFRobot_MultiGasSensor.h"
 
class AmmoniaSensor {
public:
  AmmoniaSensor(TwoWire& wire, sensorData& squidData);
  bool init();
  void checkWarmUpStatus();
  void updateAmmoniaConcentration(); 
  void getAndSaveAmmoniaReading();    
  void printGasConcentration();
  void printTemperature();
  void printAll();
  //void runScheduler();
private:
  sensorData& mSquidData;

  void warmUpCallback();
  static const uint8_t I2C_ADDRESS = 0x74;
  DFRobot_GAS_I2C gasSensor;
  unsigned long warmUpStartTime;
  static const unsigned long WARM_UP_TIME = 3000;  // 5 minutes in milliseconds  300000
  // Scheduler runner;
  // Task warmUpTask;
  // bool isWarmupComplete;
};
 
#endif // __Ammonia_Sensor_H__