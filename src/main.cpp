#include <Arduino.h>
#include <TaskScheduler.h>
#include "DFRobot_MultiGasSensor.h"


// include all relevant libraries
//MQTT related Libraries
#include <Ethernet.h>
#include <PubSubClient.h>
#include "config.h"
#include "ActuatorController.h"
#include "SerialMenu.h" 

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

squidConfig config;

//Instantiate Classes
SerialMenu squidMenu(squidFirmwareVersion, squidHardwareVersion, config);
ActuatorController controller;
// Scheduler runner;
// Task t1(1000, TASK_FOREVER, &myTask);

// State Management
bool isWarmupComplete = false;



// void myTask(){
#define GPIO12 12
// }

void setup() {
  // debugging serial port (over uUSB)
  squidMenu.init();
  //squidMenu.loadConfig(config)


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
  
  //begin transmitting data to server if warmup has finished
  if(isWarmupComplete == true) {
  controller.clearYellowLED();
  controller.setGreenLED();
  
  }
 
 
}

