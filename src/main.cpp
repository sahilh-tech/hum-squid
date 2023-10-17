
//utility libraries
#include "config.h"
#include "SerialMenu.h" 

// include all relevant libraries
//MQTT related Libraries 
#include <PubSubClient.h>


// Perpheral libraries
#include "ActuatorController.h"
#include "AmmoniaSensor.h"
#include "TemperatureHumidity.h"
#include "SoilTemperature.h" 
#include "ModBusDriver.h"

#include "TemperatureProbe.h" 
#include "ADS1X15.h"

// VERSION NUMBER [UPDATE WITH Github Versions and Changlog.md in project]
//-------------------------------------------------------------------------------------------------------
#define FIRMWARE_MAJOR 0 
#define FIRMWARE_MINOR 0 
#define FIRMWARE_PATCH 1

#define HARDWARE_MAJOR 0 
#define HARDWARE_MINOR 0
#define HARDWARE_PATCH 1
//-------------------------------------------------------------------------------------------------------

versionNumber squidFirmwareVersion(FIRMWARE_MAJOR, FIRMWARE_MINOR, FIRMWARE_PATCH);
versionNumber squidHardwareVersion(HARDWARE_MAJOR, HARDWARE_MINOR, HARDWARE_PATCH);

//Data Structures
squidConfig config; //config parameters
sensorData squidData;


//Instantiate Classes
SerialMenu squidMenu(squidFirmwareVersion, squidHardwareVersion, config, squidData);
ActuatorController controller;
AmmoniaSensor ammoniaSensor(Wire, squidData);
TemperatureHumidity ambientSensor(Wire, squidData);
ModBusDriver modbusDriver(squidData);
TemperatureProbe soilTemperatureData(squidData);
 


// Scheduler runner;
// Task t1(1000, TASK_FOREVER, &myTask);

// State Management
bool isWarmupComplete = false;
 

 

void setup() {
  // debugging serial port (over uUSB)
  squidMenu.init();  
  Serial.begin(115200);
 
  // setup Ammonia Sensor
  if (!ammoniaSensor.init()) {
    Serial.println("NO Devices !");
    while (1) {
      delay(1000);
    }
  }
  soilTemperatureData.init();
  ambientSensor.init();
  // Initialize the modbus sensors
  modbusDriver.init(1); 

  // set Digital Output pins:
  controller.init();
  controller.setYellowLED();
  delay(2000);
  Serial.println("Hello, world!");
  
  isWarmupComplete = true;
  Serial.println("Finished initilaisation!");
}

void loop() { 
  if (Serial.available() > 0) {
    squidMenu.runSerialMenu();
  } 
  
  // begin transmitting data to server if warmup has finished
  if(!isWarmupComplete ) {

  } else { 
    
    controller.clearYellowLED();
    controller.setGreenLED();  
    soilTemperatureData.updateSoilTemperatureData();
    soilTemperatureData.printAllProbeData();

    ammoniaSensor.updateAmmoniaConcentration(); 

    ambientSensor.updateTempAndHumidity(); 
    modbusDriver.updateSoilOxygenData();
    modbusDriver.updateSoilMoistureData(); 
    modbusDriver.printSoilMoistureData();
    printSensorData(squidData);
  }
 
  delay(1000);
}
