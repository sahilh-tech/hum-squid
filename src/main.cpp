#include <Arduino.h>
#include <TaskScheduler.h>



// include all relevant libraries
//MQTT related Libraries
#include <Ethernet.h>
#include <PubSubClient.h>
#include "config.h"
#include "ActuatorController.h"
 

// VERSION NUMBER [UPDATE WITH Github Versions and Changlog.md in project]
//-------------------------------------------------------------------------------------------------------
#define FIRMWARE_MAJOR 0 
#define FIRMWARE_MINOR 0 
#define FIRMWARE_PATCH 0

#define HARDWARE_MAJOR 0 
#define HARDWARE_MINOR 0
#define HARDWARE_PATCH 0
//-------------------------------------------------------------------------------------------------------

versionNumber squidFirmwareVersion(FIRMWARE_MAJOR, FIRMWARE_MINOR, FIRMWARE_PATCH);
versionNumber squidHardwareVersion(HARDWARE_MAJOR, HARDWARE_MINOR, HARDWARE_PATCH);

 
//Istantiate Classes


// Scheduler runner;
// Task t1(1000, TASK_FOREVER, &myTask);

// void myTask(){
#define GPIO12 12
// }

void setup() {
  // debugging serial port (over uUSB)
  Serial.begin(115200);
  Serial.println("Hello, world!");
 

  // set Digital Output pins:
   // controller.init();
    pinMode(GPIO12, OUTPUT);
    digitalWrite(GPIO12, LOW);

        pinMode(DRUM_ROTATION_RELAY, OUTPUT);
    digitalWrite(DRUM_ROTATION_RELAY, LOW);
}

void loop() { 

  
  digitalWrite(GPIO12, HIGH);
   digitalWrite(DRUM_ROTATION_RELAY, HIGH);
  delay(5000);
 // controller.clearGreenLED();
  Serial.println("Hello, world!");
}