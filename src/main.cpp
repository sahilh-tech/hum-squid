#include <Arduino.h>
#include <TaskScheduler.h>



// include all relevant libraries
//MQTT related Libraries
#include <Ethernet.h>
#include <PubSubClient.h>

 

// VERSION NUMBER [UPDATE WITH Github Versions and Changlog.md in project]
//-------------------------------------------------------------------------------------------------------
#define FIRMWARE_MAJOR 0 //
#define FIRMWARE_MINOR 0 
#define FIRMWARE_PATCH 0

#define HARDWARE_MAJOR 0 //
#define HARDWARE_MINOR 0
#define HARDWARE_PATCH 0
//-------------------------------------------------------------------------------------------------------

// versionNumber bladeFirmwareVersion = {FIRMWARE_MAJOR, FIRMWARE_MINOR, FIRMWARE_PATCH};
// versionNumber bladeHardwareVersion = {HARDWARE_MAJOR, HARDWARE_MINOR, HARDWARE_PATCH};

Scheduler runner;
Task t1(1000, TASK_FOREVER, &myTask);

void myTask(){

}

void setup() {
  // debugging serial port (over uUSB)
  Serial.begin(115200);
  Serial.println("Hello, world!");
 
}

void loop() {
  delay(1000);
  Serial.println("Hello, world!");
}



