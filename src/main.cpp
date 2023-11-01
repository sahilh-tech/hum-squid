
//utility libraries
#include "config.h"
#include "SerialMenu.h" 
#include "TaskScheduler.h"
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


//Instantiate Classes
SerialMenu squidMenu(squidFirmwareVersion, squidHardwareVersion, config, squidData);
DataManager dataManager;  
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
Task co2WarmUpEvent(1000, 1, &co2WarmUpTask);  // 1 minute task, execute only once // 60000
Task ammoniaWarmUpEvent(5000, 1, &ammoniaWarmUpTask); // 5 minute task, execute only once // 300000
Task dataTransmitEvent(2000, TASK_FOREVER, &readAndTransmitData); // 1 second task, exec
Task retryTransmitEvent(600000, TASK_FOREVER, &retryTransmitData); // Trigger every 10 minutes


// State Management
bool isWarmupComplete = false;






// int clk = SD_CLK;
// int cmd = SD_CMD;
// int d0  = SD_DATA0;
// int d1  = SD_DATA1;
// int d2  = SD_DATA2;
// int d3  = SD_DATA3;
 







void setup() {

  // debugging serial port (over uUSB)
    // Add tasks to the scheduler
  runner.addTask(co2WarmUpEvent);
  runner.addTask(ammoniaWarmUpEvent);
  runner.addTask(dataTransmitEvent);
  

  squidMenu.init();  
  Serial.begin(115200);
  

    // if(!SD_MMC.begin()){
    //     Serial.println("Card Mount Failed");
    //     return;
    // }
    // uint8_t cardType = SD_MMC.cardType();

    // if(cardType == CARD_NONE){
    //     Serial.println("No SD_MMC card attached");
    //     return;
    // }

    // Serial.print("SD_MMC Card Type: ");
    // if(cardType == CARD_MMC){
    //     Serial.println("MMC");
    // } else if(cardType == CARD_SD){
    //     Serial.println("SDSC");
    // } else if(cardType == CARD_SDHC){
    //     Serial.println("SDHC");
    // } else {
    //     Serial.println("UNKNOWN");
    // }

    // uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    // Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

    // listDir(SD_MMC, "/", 0);
    // createDir(SD_MMC, "/mydir");
    // listDir(SD_MMC, "/", 0);
    // removeDir(SD_MMC, "/mydir");
    // listDir(SD_MMC, "/", 2);
    // writeFile(SD_MMC, "/hello.txt", "Hello ");
    // appendFile(SD_MMC, "/hello.txt", "World!\n");
    // readFile(SD_MMC, "/hello.txt");
    // deleteFile(SD_MMC, "/foo.txt");
    // renameFile(SD_MMC, "/hello.txt", "/foo.txt");
    // readFile(SD_MMC, "/foo.txt");
    // testFileIO(SD_MMC, "/test.txt");
    // Serial.printf("Total space: %lluMB\n", SD_MMC.totalBytes() / (1024 * 1024));
    // Serial.printf("Used space: %lluMB\n", SD_MMC.usedBytes() / (1024 * 1024));






  dataManager.init();  // Initialize DataManager
  // Wire.begin();
  // setup Ammonia Sensor
  // if (!ammoniaSensor.init()) {
  //   Serial.println("NO Devices !");
  //   while (1) {
  //     delay(1000);
  //   }
  // }
   ammoniaWarmUpEvent.enable();

  // //Cco2.init();
   co2WarmUpEvent.enable();

  //soilTemperatureData.init();
  // ambientSensor.init();


  // // Initialize the modbus sensors
  // modbusDriver.init(1); 

  // // set Digital Output pins:
  controller.init();
  delay(2000);
  Serial.println("Hello, world!");
  
  isWarmupComplete = true;
  Serial.println("Finished initilaisation!");
}

void loop() { 
    // Let TaskScheduler handle the tasks
  runner.execute(); 
  if (Serial.available() > 0) {
    squidMenu.runSerialMenu();
  } 
  
  // begin transmitting data to server if warmup has finished
  // Check if both warm-up tasks are complete
  if (ammoniaWarmUpEvent.isLastIteration() && co2WarmUpEvent.isLastIteration()) {
    isWarmupComplete = true;

    // Enable the read and transmit task only after warm-up is complete
    dataTransmitEvent.enable();
  }  
  Serial.println("ON");
  controller.turnOnCO2Pump();
  delay(5000);
  controller.turnOffCO2Pump();
    Serial.println("OFF");
    delay(5000);

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
 
    controller.setGreenLED();  
    //controller.
    // soilTemperatureData.updateSoilTemperatureData();
    // soilTemperatureData.printAllProbeData();
    // soilTemperatureData.printRawData();
  // soilTemperatureData.
    // ammoniaSensor.updateAmmoniaConcentration(); 

    // ambientSensor.updateTempAndHumidity(); 
    // modbusDriver.updateSoilOxygenData(); 
    // modbusDriver.updateSoilMoistureData(); 
    // co2Sensor.updateCO2Data(); 
    //printSensorData(squidData); 
    dataManager.updateData(squidData);
    // controller.turnOnCO2Pump();
    // delay(1000);
    // controller.turnOffCO2Pump();
}
 

void retryTransmitData() {
  if (dataManager.dataPendingTransmission) { // Check the flag
    // Read data from SD card and try to send it to the server.
    // If successful, clear the SD card data.
    // if (dataManager.retrySDTransmitDataToServer()) { // You will need to implement this function
    //   dataManager.clearDataLog(); // Clear SD card data
    //   Serial.println("Successfully re-sent SD card data to server and cleared local data.");
    // }
  } else {
    Serial.println("No data pending transmission.");
  }
}