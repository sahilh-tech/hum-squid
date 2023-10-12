#include "SoilMoisture.h"
 
 
SoilMoisture* globalSoilMoistureObject = nullptr;  // Initialize the global pointer

void globalPreTransmission() {  // Implement the trampoline function for preTransmission
    if (globalSoilMoistureObject) {
        globalSoilMoistureObject->preTransmission();
    }
}

void globalPostTransmission() {  // Implement the trampoline function for postTransmission
    if (globalSoilMoistureObject) {
        globalSoilMoistureObject->postTransmission();
    }
}


// Constructor
SoilMoisture::SoilMoisture(uint8_t slaveID) : mSlaveID(slaveID) {
    globalSoilMoistureObject = this;  // Set the global pointer to this object
}

// Initialize Modbus communication
bool SoilMoisture::init() {
    node.begin(mSlaveID, Serial1);  // Initialize Modbus communication via Serial2
    Serial1.begin(9600); 
    pinMode(DE_RE_RS485, OUTPUT);
    node.preTransmission(globalPreTransmission);
    node.postTransmission(globalPostTransmission);
    return true;
}

void SoilMoisture::preTransmission()
{
  digitalWrite(DE_RE_RS485, HIGH);
}
// Pin 4 made low for Modbus receive mode
void SoilMoisture::postTransmission()
{
  digitalWrite(DE_RE_RS485, LOW);
}

// Function to read temperature
float SoilMoisture::readTemperature() {
    uint16_t raw_data;
    node.readHoldingRegisters(TEMPERATURE_REG, 1);
    raw_data = node.getResponseBuffer(0);

    // Check for negative temperature
    if (raw_data & 0x8000) {
        return (float)((int16_t)raw_data - 0xFFFF - 1) / 100.0;
    } else {
        return (float)raw_data / 100.0;
    }
}

// Function to read VWC (Volumetric Water Content)
float SoilMoisture::readVWC() {
    uint16_t raw_data;
    node.readHoldingRegisters(VWC_REG, 1);
    raw_data = node.getResponseBuffer(0);
    return (float)raw_data / 10000.0;  // Dividing by 10000 to convert to percentage
}

// Function to read EC (Electrical Conductivity)
float SoilMoisture::readEC() {
    uint16_t raw_data;
    node.readHoldingRegisters(EC_REG, 1);
    raw_data = node.getResponseBuffer(0);
    return (float)raw_data;  // Data is already in the correct units (us/cm)
}

// Function to read Salinity
float SoilMoisture::readSalinity() {
    uint16_t raw_data;
    node.readHoldingRegisters(SALINITY_REG, 1);
    raw_data = node.getResponseBuffer(0);
    return (float)raw_data;  // Data is already in the correct units (mg/L)
}

// Function to read TDS (Total Dissolved Solids)
float SoilMoisture::readTDS() {
    uint16_t raw_data;
    node.readHoldingRegisters(TDS_REG, 1);
    raw_data = node.getResponseBuffer(0);
    return (float)raw_data;  // Data is already in the correct units (mg/L)
}

// Function to read Epsilon
float SoilMoisture::readEpsilon() {
    uint16_t raw_data;
    node.readHoldingRegisters(EPSILON_REG, 1);
    raw_data = node.getResponseBuffer(0);
    return (float)raw_data / 100.0;  // Dividing by 100 to get actual value
}

// Function to read Soil Type
uint16_t SoilMoisture::readSoilType() {
    uint16_t raw_data;
    node.readHoldingRegisters(SOIL_TYPE_REG, 1);
    raw_data = node.getResponseBuffer(0);
    return raw_data;  // Data is already in the correct units (0-3 representing different soil types)
}

