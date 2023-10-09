/*!
  * @file  SerialMenu.h
  * @brief This is a header file that contains all config parameters for the platform
  * @copyright   Copyright (c) 2023 Sahil Harriram Tech Pty Ltd.
  * @license     The MIT License (MIT)
  * @author      Sahil Harriram (Hello@sahil.harriram.tech)
  * @version     V0.0.1
  * @date        2023-09-02 
*/
#ifndef SerialMenu_h
#define SerialMenu_h

#include <Arduino.h>
#include "Utilities.h" 
#include "config.h"

#define MAX_WIFI_SSID_LENGTH 33
#define MAX_ALLOCATED_EEPROM 4096



class SerialMenu {
public:
    // WARNING!!eepromSize cannot be greater than   4096 bytes
    SerialMenu(versionNumber firmware, versionNumber hardware, squidConfig& config, 
               uint16_t eepromSize=512);
    void init();
    void loadConfig();
    void runSerialMenu();
    void printHelp();
    void printVersion();
    void setDrumRotation(uint32_t seconds);
    uint16_t getDrumRotation(bool verbose = true);
    void setVentControl(uint32_t seconds);
    uint16_t getVentControl(bool verbose = true);
    void setTempThreshold(int32_t degrees);
    int8_t getTempThreshold(bool verbose = true);
    void setCO2Threshold(uint32_t ppm);
    uint16_t getCO2Threshold(bool verbose = true);
    void setAmmoniaThreshold(uint32_t ppm);
    uint8_t getAmmoniaThreshold(bool verbose = true);
    void setWifiCredentials(const String& ssid, const String& password);
    void getWifiSSID(char ssidBuffer[MAX_WIFI_SSID_LENGTH], bool verbose);
    uint16_t getSquidID(bool verbose = true);
    void setSquidID(uint32_t id);
    uint16_t getNodeID(bool verbose = true);
    void setNodeID(uint32_t id);
    void printConfigTable();

    //Admin Functions
    void factoryReset(); // resets system to default values
    void hardMemoryReset(); // hard resets the memory on the platform
 
 private:
    // constants for software
    const uint16_t minDrumRotation= 0;
    const uint16_t maxDrumRotation = 65535;
    const uint16_t minVentControl = 0;
    const uint16_t maxVentControl = 65535;
    const int8_t minTempThreshold = -20;
    const int8_t maxTempThreshold = 100;
    const uint16_t minCO2Threshold = 0;
    const uint16_t maxCO2Threshold = 10000;   
    const uint8_t minAmmoniaThreshold = 0;
    const uint8_t maxAmmoniaThreshold = 100; 
    const uint8_t maxWifiSSIDLength = MAX_WIFI_SSID_LENGTH;
    const uint8_t maxWifiPasswordLength =64;

    // Define EEPROM Addresses for each variable
    const uint16_t DRUM_ROTATION_ADDR = 0;      // 2 bytes (uint16_t), so next starts at 2
    const uint16_t VENT_CONTROL_ADDR = 2;       // 2 bytes (uint16_t), so next starts at 4
    const uint16_t TEMP_THRESHOLD_ADDR = 4;     // 1 byte (int8_t), so next starts at 5
    const uint16_t CO2_THRESHOLD_ADDR = 5;      // 2 bytes (uint16_t), so next starts at 7
    const uint16_t AMMONIA_THRESHOLD_ADDR = 7;  // 1 byte (uint8_t), so next starts at 8
    const uint16_t WIFI_SSID_ADDR = 8;          // 33 bytes (char array), so next starts at 41
    const uint16_t WIFI_PASS_ADDR = 41;         // 64 bytes (char array), so next starts at 105
    const uint16_t SQUID_ID_ADDR = 105;         // 2 bytes (uint16_t), so next starts at 107
    const uint16_t NODE_ID_ADDR = 107;          // 2 bytes (uint16_t), so next starts at 109



    versionNumber mFirmware; 
    versionNumber mHardware;
    uint16_t mEepromSize; // this cannot be greater than 4096 bytes
    void handleInput();
    String readSerialLine();
    void writeStringToEEPROM(uint16_t addr, const String& data, uint8_t maxLength);
    String readStringFromEEPROM(uint16_t addr, uint8_t maxLength);
    squidConfig& mConfig;
};

#endif