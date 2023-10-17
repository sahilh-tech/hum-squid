
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

// SoilTemperature soilTemperature(Wire, squidData); 
//TemperatureProbe temp1(ADC_TEMP_PROBE_2);
//SoilMoisture SoilMoistureSensor;


// Scheduler runner;
// Task t1(1000, TASK_FOREVER, &myTask);

// State Management
bool isWarmupComplete = false;
 

 

void setup() {
  // debugging serial port (over uUSB)
  squidMenu.init();  
  Serial.begin(115200);


  // setup Ammonia Sesnor
  if (!ammoniaSensor.init()) {
    Serial.println("NO Devices !");
    while (1) {
      delay(1000);
    }
  }
  ambientSensor.init();
  // Initialize the SoilTemperature object
  // soilTemperature.init();
  //SoilMoistureSensor.init();
  //temp1.init();
  modbusDriver.init(1);
  // soilOxygenSensor.init();

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
    ammoniaSensor.updateAmmoniaConcentration();
    // float soilMoisture = SoilMoistureSensor.readVWC();
    // float soilTemp = SoilMoistureSensor.readTemperature();
    // Serial.print("soil moisture: ");
    // Serial.println(soilMoisture);
    // Serial.print("soil soilTemp: ");
    // Serial.println(soilTemp); 
    controller.clearYellowLED();
    controller.setGreenLED();
    ammoniaSensor.getAndSaveAmmoniaReading();
   // ammoniaSensor.printAll();
    ambientSensor.updateTempAndHumidity();
    // ambientSensor.printAll(); 
    // Update the soil temperature da ta
      // soilTemperature.updateSoilTemperatureData( );

      // // Print individual probe data
      // soilTemperature.printProbeData(0); // Print data for probe 0
      // soilTemperature.printProbeData(1); // Print data for probe 1
      // soilTemperature.printProbeData(2); // Print data for probe 2

      // // Or print all probe data at once
      // soilTemperature.printAllProbeData();
     // modbusDriver.printSoilOxygenData();
     modbusDriver.updateSoilOxygenData();
     modbusDriver.updateSoilMoistureData();
    //temp1.read(); 
   modbusDriver.printSoilMoistureData();
    printSensorData(squidData);
  }
 
  delay(1000);
}
