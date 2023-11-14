
//utility libraries
#include "config.h"
#include "SerialMenu.h" 
#include "TaskScheduler.h"
// include all relevant libraries
//MQTT related Libraries 
#include <PubSubClient.h>
#include <ETH.h>
#include <esp_task_wdt.h>


// Perpheral libraries
#include "ActuatorController.h"
#include "AmmoniaSensor.h"
#include "TemperatureHumidity.h"
#include "SoilTemperature.h" 
#include "ModBusDriver.h"

#include "TemperatureProbe.h" 
#include "ADS1X15.h"
#include "DataManager.h" // Include the DataManager class
#include "K30Sensor.h"

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

// initialise hardware serial object
// HardwareSerial uart2(2); // Use UART2

//Instantiate Classes
SerialMenu squidMenu(squidFirmwareVersion, squidHardwareVersion, config, squidData);
//DataManager dataManager;  
ActuatorController controller;
AmmoniaSensor ammoniaSensor(Wire, squidData);
TemperatureHumidity ambientSensor(Wire, squidData);
ModBusDriver modbusDriver(squidData);
TemperatureProbe soilTemperatureData(squidData);
K30Sensor co2Sensor(squidData); 




//tasks
void ammoniaWarmUpTask();
void co2WarmUpTask();
void readAndTransmitData();
void retryTransmitData();

Scheduler runner;  
Task co2WarmUpEvent(1000, 1, &co2WarmUpTask);  // 1 second task, execute only once // 60000
Task ammoniaWarmUpEvent(5000, 1, &ammoniaWarmUpTask); // 5 secojd task, execute only once // 300000
Task dataTransmitEvent(2000, TASK_FOREVER, &readAndTransmitData); // 2 secodnd
Task retryTransmitEvent(600000, TASK_FOREVER, &retryTransmitData); // Trigger every 10 minutes


// State Management
bool isWarmupComplete = false;
SensorState currentState = READ_AMMONIA;
unsigned long lastSensorUpdateTime = 0;
const unsigned long sensorUpdateInterval = 50; // 50 milliseconds between readings


void setup() {
  // Initialize WDT
    esp_task_wdt_init(5, true); 
  // debugging serial port (over uUSB)
    // Add tasks to the scheduler
  runner.addTask(co2WarmUpEvent);
  runner.addTask(ammoniaWarmUpEvent);
  runner.addTask(dataTransmitEvent);
  
  
  Serial.begin(115200);
  squidMenu.init();
    Serial.println("Starting up");

  //dataManager.init();  // Initialize DataManager
  //Wire.begin();
  //setup Ammonia Sensor
  if (!ammoniaSensor.init()) {
    Serial.println("NO Ammonia Sensor detected Devices !");
    while (1) {
      delay(1000);
    }
  }
   ammoniaWarmUpEvent.enable();
 // pinMode(DE_RE_RS485, OUTPUT);
  // co2Sensor.init();
  co2WarmUpEvent.enable();

  soilTemperatureData.init();
  ambientSensor.init();


  // // Initialize the modbus sensors
  modbusDriver.init(1); 

  // // set Digital Output pins:
  //controller.init();
  delay(2000);
  Serial.println("Hello, world!");
  
 
  Serial.println("Finished initilaisation!");
}

void loop() { 
  // Feed the watchdog
  esp_task_wdt_reset();
    // Let TaskScheduler handle the tasks
  runner.execute(); 
  if (Serial.available() > 0) {
    squidMenu.runSerialMenu();
  } 
  
  // begin transmitting data to server if warmup has finished
  // Check if both warm-up tasks are complete
  if (ammoniaWarmUpEvent.isLastIteration() && co2WarmUpEvent.isLastIteration() && !isWarmupComplete) {
    isWarmupComplete = true;

    // Enable the read and transmit task only after warm-up is complete
    dataTransmitEvent.enable();
    Serial.println("dataTransmitEvent initiated...");

  }   
  //controller.turnOffCO2Pump(); 

}


void ammoniaWarmUpTask() {
  ammoniaWarmUpEvent.disable();
  Serial.println("Warm-up for Ammonia Sensor complete.");
}



void co2WarmUpTask() {
  // Disable the task after completion
  co2WarmUpEvent.disable();
   Serial.println("Warm-up for CO2 sensor complete.");
}

void readAndTransmitData() {
  static unsigned long lastCallTime = 0;
  unsigned long currentCallTime = millis();
  Serial.print("readAndTransmitData called at: ");
  Serial.print(currentCallTime);
  Serial.print(" ms, Delta since last call: ");
  Serial.print(currentCallTime - lastCallTime);
  Serial.println(" ms");
  lastCallTime = currentCallTime;
  //   ammoniaSensor.updateAmmoniaConcentration();
  //   ammoniaSensor.printTemperature();
  //   ammoniaSensor.printGasConcentration();
  //          co2Sensor.updateCO2Data();
  //        co2Sensor.printCO2Data();
  //     ambientSensor.updateTempAndHumidity();
  //     ambientSensor.printTemp();
  //     ambientSensor.printHumidity();
  
  
  unsigned long currentMillis = millis();
  
  //Process all sensor readings in a non-blocking manner
  while (currentMillis - lastSensorUpdateTime >= sensorUpdateInterval) {
    switch (currentState) {
      case READ_AMMONIA:
        ammoniaSensor.updateAmmoniaConcentration();
        ammoniaSensor.printTemperature();
        ammoniaSensor.printGasConcentration();
        currentState = READ_SOIL_MOISTURE;
        break;
      case READ_SOIL_MOISTURE:
        modbusDriver.updateSoilMoistureData();
        modbusDriver.printSoilMoistureData();
        currentState = READ_TEMP_HUMIDITY;
        break;
      case READ_TEMP_HUMIDITY:
        ambientSensor.updateTempAndHumidity();
        ambientSensor.printTemp();
        ambientSensor.printHumidity();
        currentState = READ_SOIL_OXYGEN;
        break;
      case READ_SOIL_OXYGEN:
        modbusDriver.printSoilOxygenData();
        currentState = READ_CO2;
        break;
      case READ_CO2:
        co2Sensor.updateCO2Data();
        co2Sensor.printCO2Data();
        currentState = READ_SOIL_TEMP;
        break;
      case READ_SOIL_TEMP:
        soilTemperatureData.updateSoilTemperatureData();
        soilTemperatureData.printAllProbeData();
        soilTemperatureData.printRawData();
        currentState = END_CYCLE;
        break;
      case END_CYCLE:
        Serial.println("End data");
        Serial.println();
        currentState = READ_AMMONIA; // Start the cycle over on the next call
        return; // End the function here to wait for the next 2-second interval
    }
    lastSensorUpdateTime = millis(); // Reset the last update time
  }
}
 

void retryTransmitData() {
  // if (dataManager.dataPendingTransmission) { // Check the flag
  //   // Read data from SD card and try to send it to the server.
  //   // If successful, clear the SD card data.
  //   // if (dataManager.retrySDTransmitDataToServer()) { // You will need to implement this function
  //   //   dataManager.clearDataLog(); // Clear SD card data
  //   //   Serial.println("Successfully re-sent SD card data to server and cleared local data.");
  //   // }
  // } else {
  //   Serial.println("No data pending transmission.");
  // }
}