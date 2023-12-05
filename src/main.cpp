
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

// VERSION NUMBER  
//---------------------------------------------------------------------------------------- 
#define FIRMWARE_MAJOR 0 
#define FIRMWARE_MINOR 0 
#define FIRMWARE_PATCH 1

#define HARDWARE_MAJOR 0 
#define HARDWARE_MINOR 0
#define HARDWARE_PATCH 1
//----------------------------------------------------------------------------------------

versionNumber squidFirmwareVersion(FIRMWARE_MAJOR, FIRMWARE_MINOR, FIRMWARE_PATCH);
versionNumber squidHardwareVersion(HARDWARE_MAJOR, HARDWARE_MINOR, HARDWARE_PATCH);

// Static IP address configuration
IPAddress staticIP(192, 168, 1, 101); // ESP32 static IP UPDATE THIS FOR EVERY SQUID
IPAddress gateway(192, 168, 1, 1);    // Gateway (usually the router IP)
IPAddress subnet(255, 255, 255, 0);   // Subnet Mask
IPAddress primaryDNS(8, 8, 8, 8);     // Optional: Google DNS
IPAddress secondaryDNS(8, 8, 4, 4);   // Optional: Secondary Google DNS

// MQTT Broker settings
const char* mqtt_broker = "192.168.1.100"; // MQTT Broker IP
const char* mqtt_user = "hum";          // MQTT username
const char* mqtt_password = "masternode"; // MQTT password
const int mqtt_port = 1883;
char topic[50]; 
char ventRequestTopic[50];


// MQTT client
WiFiClient ethClient;
PubSubClient client(ethClient);


//Data Structures
squidConfig config; //config parameters
sensorData squidData;

// global variables for MQTT message publishing
// Global Variables
String csvString; // CSV string to hold aggregated data
int publishCount = 0; // Counter to track when to publish
unsigned long triggerTime = 0;
bool isVentSet = false;    // Variable to track the pin state


//Instantiate Classes
SerialMenu squidMenu(squidFirmwareVersion, squidHardwareVersion, config, squidData);
//DataManager dataManager;  
ActuatorController controller;
AmmoniaSensor ammoniaSensor(Wire, squidData);
TemperatureHumidity ambientSensor(Wire, squidData);
ModBusDriver modbusDriver(squidData);
TemperatureProbe soilTemperatureData(squidData);
K30Sensor co2Sensor(squidData); 


// mqtt and network functions
void reconnectToMQTT();
void mqttCallback(char* topic, byte* payload, unsigned int length);
String mqttStateToString(int state);
void WiFiEvent(WiFiEvent_t event); 


//tasks
void ammoniaWarmUpTask();
void co2WarmUpTask();
void collectSensorData(); 
void aggregateAndPublishData();
void ventilationCheck();
void turnOnCO2Pump();  


//Task Scheduler Events
Scheduler runner;  
Task co2WarmUpEvent(1000, 1, &co2WarmUpTask);  //  execute only once // 60000
Task ammoniaWarmUpEvent(5000, 1, &ammoniaWarmUpTask); //   execute only once // 300000
Task dataCollectionEvent(60000, TASK_FOREVER, &collectSensorData); //60000 = 1 minute 
Task dataPublishEvent(300000, TASK_FOREVER, &aggregateAndPublishData);//300000 = every 5 min
Task ventilationCheckEvent(1000, TASK_FOREVER, &ventilationCheck);// every second
Task turnOnCO2PumpEvent(45000, TASK_ONCE, &turnOnCO2Pump); // Runs 45 seconds after being enabled



// State Management
bool isWarmupComplete = false; 
bool eth_connected = false;


void setup() {
  // Initialize WDT
  //  esp_task_wdt_init(5, true); 
  // debugging serial port (over uUSB)
    // Add tasks to the scheduler
  runner.addTask(co2WarmUpEvent);
  runner.addTask(ammoniaWarmUpEvent);
  runner.addTask(dataCollectionEvent);
  runner.addTask(dataPublishEvent);
  runner.addTask(ventilationCheckEvent);
  runner.addTask(turnOnCO2PumpEvent);


  Serial.begin(115200);
  squidMenu.init();
  Serial.println("Starting up");

  //dataManager.init();  // Initialize DataManager
  Wire.begin();
  // setup Ammonia Sensor
  if (!ammoniaSensor.init()) {
    Serial.println("NO Ammonia Sensor detected Devices !");
    while (1) {
      delay(1000);
    }
  }

  ammoniaWarmUpEvent.enable(); 
  co2WarmUpEvent.enable();
  soilTemperatureData.init();
  ambientSensor.init();
  // Initialize the modbus sensors
  modbusDriver.init(1);   
 
  // set Digital Output pins:
  controller.init();

 
  // intitialise ethernet port
    // Initialize Ethernet
    WiFi.onEvent(WiFiEvent);
    ETH.begin();
 
 
  if (ETH.linkUp() && !eth_connected) {
    Serial.println("Ethernet link is up, configuring static IP...");
    ETH.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS);
    IPAddress ip = ETH.localIP();
    IPAddress subnet = ETH.subnetMask();
    Serial.print("IP Address: ");
    Serial.println(ip);
    Serial.print("Subnet Mask: ");
    Serial.println(subnet);
  } 

  // Connect to MQTT Broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(mqttCallback);
  // Call function to connect to MQTT
  reconnectToMQTT();

  client.subscribe("timestamp");
  snprintf(topic, sizeof(topic), "sensor_data_topic/%d", squidData.squidID);
  snprintf(ventRequestTopic, sizeof(ventRequestTopic), "squidActuation/ventRequest/%d", squidData.squidID);
 

  delay(2000);
  Serial.println("Finished Initialisation!"); 

  //config.temperatureThreshold = 20;
}

void loop() {  

  // Feed the watchdog
  // esp_task_wdt_reset();
    // Let TaskScheduler handle the tasks
  runner.execute(); 
  client.loop();

  if (Serial.available() > 0) {
    squidMenu.runSerialMenu();
  } 
  
  // begin transmitting data to server if warmup has finished
  // Check if both warm-up tasks are complete
  if (ammoniaWarmUpEvent.isLastIteration() && co2WarmUpEvent.isLastIteration() &&
     !isWarmupComplete) {
    isWarmupComplete = true;
    controller.setGreenLED();
    // Enable the read and transmit task only after warm-up is complete
    dataCollectionEvent.enable();
    dataPublishEvent.enable();
    ventilationCheckEvent.enable();
    Serial.println("dataTransmitEvent initiated...");

  }    
}

void turnOnCO2Pump() {
    controller.turnOnCO2Pump();
    Serial.println("Turn on co2 pump.");
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

void ventilationCheck() {  
  if (((millis() - triggerTime) >= (config.ventHVACDuration * 1000)) && isVentSet) {
    controller.clearHVACRequest();
    isVentSet = false;  
    config.temperatureThreshold = 60;
  }

  if (((squidData.ammonia >= config.ammoniaThreshold) || 
      (squidData.CO2 >=  config.co2Threshold) || 
      (squidData.ambientTemp >=  config.temperatureThreshold )) && !isVentSet)
  {
    controller.setHVACRequest();
    isVentSet= true;
    triggerTime = millis();
  }
  if(isVentSet) {
    // publish HVAC request topic 
    bool publishResult = client.publish(ventRequestTopic, "Vent Request Set");
    if (publishResult) {
    Serial.println("Data published successfully.");
    } else {
    Serial.println("Failed to publish data.");
    }
  }

}


void collectSensorData() {
 
  ammoniaSensor.updateAmmoniaConcentration();
  ammoniaSensor.printTemperature();
  ammoniaSensor.printGasConcentration();
  delay(50);
  modbusDriver.updateSoilMoistureData();
  modbusDriver.printSoilMoistureData();
  delay(50);
  ambientSensor.updateTempAndHumidity();
  ambientSensor.printTemp();
  ambientSensor.printHumidity();
  delay(50);
  modbusDriver.updateSoilOxygenData();
  modbusDriver.printSoilOxygenData();
  delay(50);
  co2Sensor.updateCO2Data();
  co2Sensor.printCO2Data();
  delay(50);
  soilTemperatureData.updateSoilTemperatureData();
  soilTemperatureData.printAllProbeData();
  soilTemperatureData.printRawData();

  printSensorData(squidData);
      Serial.println("Turn off pump.");
  controller.turnOffCO2Pump();
  turnOnCO2PumpEvent.enable();


 
 } 
 

void aggregateAndPublishData() {
  // Append current data to the CSV string
  csvString += String(squidData.squidID) + "," + 
               String(squidData.nodeID) + "," + 
               String(squidData.timestamp) + "," +
               String(squidData.soilOxygen) + "," +
               String(squidData.CO2) + "," +
               String(squidData.ammonia) + "," +
               String(squidData.soilMoisture) + "," +
               String(squidData.soilTempProbe) + "," +
               String(squidData.soilTempProbe1) + "," +
               String(squidData.soilTempProbe2) + "," +
               String(squidData.soilTempProbe3) + "," +
               String(squidData.ambientTemp) + "," +
               String(squidData.ambientHumidity) + "\n";

  // Increment the counter
  publishCount++;

  // Check if it's time to publish (every 3rd aggregation)
  if (publishCount >= 3) {
    Serial.print("publishing data on topic: "); 
    Serial.println(topic);  

  // Debugging: Print the CSV string size
  Serial.print("CSV String Size: ");
  Serial.println(csvString.length());
  Serial.print("full csv = "); 
  Serial.println(csvString); 

     
  // Attempt to publish
  bool publishResult = client.publish(topic, csvString.c_str(), true); //QoS 1
  if (publishResult) {
    Serial.println("Data published successfully.");
  } else {
    Serial.println("Failed to publish data.");
  }
    // Reset the CSV string and counter
    csvString = "";
    publishCount = 0;
  }
}


void mqttCallback(char* topic, byte* payload, unsigned int length) {
    // Handle incoming messages
    if (strcmp(topic, "timestamp") == 0) {
        // Check if the payload length is 10 bytes for a Unix timestamp
        if (length == 10) {
            char strPayload[11]; // Buffer for 10 characters + null terminator
            // Copy the payload to the buffer and null-terminate it
            strncpy(strPayload, (char*)payload, length);
            strPayload[length] = '\0'; // Ensure null termination
            
            // Convert the payload to an unsigned long
            uint32_t timestamp = strtoul(strPayload, NULL, 10);
            squidData.timestamp = timestamp; // Update squidData.timestamp
             
        } else {
            Serial.println("Invalid payload length for Unix timestamp");
        }
    }
} 

void reconnectToMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT Broker...");
    if (client.connect("ESP32Client" , mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT Broker.");
      client.subscribe("timestamp"); // Subscribe to the timestamp topic
    } else {
      Serial.print("Connection failed with state: ");
      Serial.println(mqttStateToString(client.state()));
      Serial.println("Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

String mqttStateToString(int state) {
  switch (state) {
    case -4: return "MQTT_CONNECTION_TIMEOUT";
    case -3: return "MQTT_CONNECTION_LOST";
    case -2: return "MQTT_CONNECT_FAILED";
    case -1: return "MQTT_DISCONNECTED";
    case 0: return "MQTT_CONNECTED";
    case 1: return "MQTT_CONNECT_BAD_PROTOCOL";
    case 2: return "MQTT_CONNECT_BAD_CLIENT_ID";
    case 3: return "MQTT_CONNECT_UNAVAILABLE";
    case 4: return "MQTT_CONNECT_BAD_CREDENTIALS";
    case 5: return "MQTT_CONNECT_UNAUTHORIZED";
    default: return "UNKNOWN_ERROR";
  }
}

 
void WiFiEvent(WiFiEvent_t event) {
    switch (event) {
        case SYSTEM_EVENT_ETH_START:
            Serial.println("ETH Started");
            ETH.setHostname("ESP32-ETH");
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
            Serial.println("ETH Connected");
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            eth_connected = true;
            Serial.print("ETH MAC: ");
            Serial.print(ETH.macAddress());
            Serial.print(", IPv4: ");
            Serial.print(ETH.localIP());
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            Serial.println("ETH Disconnected");
            eth_connected = false;
            break;
        case SYSTEM_EVENT_ETH_STOP:
            Serial.println("ETH Stopped");
            eth_connected = false;
            break;
        default:
            break;
    }
}
