
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

// Static IP address configuration
IPAddress staticIP(192, 168, 1, 101); // ESP32 static IP // Make sure to update this for every new node
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
const char *testTopic = "sensor_data_topic/1";


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
void collectSensorData(); 
void aggregateAndPublishData();

// mqtt and network functions
void reconnectToMQTT();
void mqttCallback(char* topic, byte* payload, unsigned int length);
String mqttStateToString(int state);
void WiFiEvent(WiFiEvent_t event); 

//Task Scheduler Events
Scheduler runner;  
Task co2WarmUpEvent(1000, 1, &co2WarmUpTask);  // 1 second task, execute only once // 60000
Task ammoniaWarmUpEvent(5000, 1, &ammoniaWarmUpTask); // 5 second task, execute only once // 300000
Task dataCollectionEvent(1000, TASK_FOREVER, &collectSensorData); //60000 = 1 minute 
Task dataPublishEvent(2000, TASK_FOREVER, &aggregateAndPublishData); //300000 = every 5 minutes

// State Management
bool isWarmupComplete = false;
SensorState currentState = READ_AMMONIA;
unsigned long lastSensorUpdateTime = 0;
const unsigned long sensorUpdateInterval = 50; // 50 milliseconds between readings



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


 
  delay(2000);
  Serial.println("Finished Initialisation!");  
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
  if (ammoniaWarmUpEvent.isLastIteration() && co2WarmUpEvent.isLastIteration() && !isWarmupComplete) {
    isWarmupComplete = true;
    controller.setGreenLED();
    // Enable the read and transmit task only after warm-up is complete
    dataCollectionEvent.enable();
    dataPublishEvent.enable();
    Serial.println("dataTransmitEvent initiated...");

  }    
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

void collectSensorData() {
  // Use globalTimestamp for your data 
  Serial.print(" Timestamp: ");
  Serial.println(squidData.timestamp);

  static unsigned long lastCallTime = 0;
  unsigned long currentCallTime = millis();
  Serial.print("collectSensorData called at: ");
  Serial.print(currentCallTime);
  Serial.print(" ms, Delta since last call: ");
  Serial.print(currentCallTime - lastCallTime);
  Serial.println(" ms");
  lastCallTime = currentCallTime; 
  
  unsigned long currentMillis = millis(); 

  //Process all sensor readings in a non-blocking manner
  while (currentMillis - lastSensorUpdateTime >= sensorUpdateInterval) {
    switch (currentState) {
      case READ_AMMONIA:
        ammoniaSensor.updateAmmoniaConcentration();
        // ammoniaSensor.printTemperature();
        // ammoniaSensor.printGasConcentration();
        currentState = READ_SOIL_MOISTURE;
        break;
      case READ_SOIL_MOISTURE:
       modbusDriver.updateSoilMoistureData();
        // modbusDriver.printSoilMoistureData();
        currentState = READ_TEMP_HUMIDITY;
        break;
      case READ_TEMP_HUMIDITY:
        ambientSensor.updateTempAndHumidity();
        // ambientSensor.printTemp();
        // ambientSensor.printHumidity();
        currentState = READ_SOIL_OXYGEN;
        break;
      case READ_SOIL_OXYGEN:
        modbusDriver.updateSoilOxygenData();
      //  modbusDriver.printSoilOxygenData();
        currentState = READ_CO2;
        break;
      case READ_CO2:
        co2Sensor.updateCO2Data();
        // co2Sensor.printCO2Data();
        currentState = READ_SOIL_TEMP;
        break;
      case READ_SOIL_TEMP:
        soilTemperatureData.updateSoilTemperatureData();
        // soilTemperatureData.printAllProbeData();
        // soilTemperatureData.printRawData();
        currentState = END_CYCLE;
        break;
      case END_CYCLE:
        // Serial.println("End data");
        // Serial.println();
        //printSensorData(squidData);
        currentState = READ_AMMONIA; // Start the cycle over on the next call
        return; // End the function here to wait for the next 2-second interval
    }
    lastSensorUpdateTime = millis(); // Reset the last update time
 }
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
    Serial.println(testTopic);  

      // Debugging: Print the CSV string size
  Serial.print("CSV String Size: ");
  Serial.println(csvString.length());
    Serial.print("full csv = "); 
    Serial.println(csvString); 

    
    // Prepend header row to the CSV string
    String fullCsv = "Squid ID,Node ID,TimeStamp,Soil Oxygen,CO2,Ammonia,Soil Moisture,Soil Temp Probe,Soil Temp Probe 1,Soil Temp Probe 2,Soil Temp Probe 3,Ambient Temp,Ambient Humidity\n" + csvString;

    // Publish the data
    //client.publish(testTopic, fullCsv.c_str()); // QoS 1, retained message
  // Attempt to publish
  bool publishResult = client.publish(topic, csvString.c_str(), true); // QoS 1, retained message
  if (publishResult) {
    Serial.println("Data published successfully.");
  } else {
    Serial.println("Failed to publish data.");
  }
//  client.publish(testTopic,  (const char*)csvString, strlen(csvString)); 
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
            
            // Serial.print("Timestamp updated: ");
            // Serial.println(squidData.timestamp);
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
