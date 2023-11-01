#include "ModBusDriver.h"
 
 
 

ModBusDriver::ModBusDriver(sensorData& squidData) : mSquidData(squidData) {
 
}

bool ModBusDriver::init(uint8_t slaveID) {
    mSlaveID = slaveID;
    Serial1.begin(9600); 
    node.begin(mSlaveID, Serial1);

    pinMode(DE_RE_RS485, OUTPUT);
    node.preTransmission([this]() { this->preTransmission(); });
    node.postTransmission([this]() { this->postTransmission(); });
    return true;
}

uint16_t ModBusDriver::readRegister(uint8_t slaveID, uint16_t regAddress) {
    uint8_t result;
    uint16_t data[1];

    node.setSlaveId(slaveID);
    result = node.readHoldingRegisters(regAddress, 1);
    data[0] = node.getResponseBuffer(0);
    return data[0];
    // if (result == node.ku8MBSuccess)
    // {
    //     return data[0] = node.getResponseBuffer(0);
    // }  else {
    //     return 65535; // Return max value on failure
    // }
    
}

bool ModBusDriver::writeRegister(uint8_t slaveID, uint16_t regAddress, uint16_t value) {
    uint8_t result;
    setSlaveID(slaveID);
    result = node.writeSingleRegister(regAddress, value);
    if (result == node.ku8MBSuccess) { 
        return true;
    } else {
        return false;
    }
}

void ModBusDriver::preTransmission() {
    digitalWrite(DE_RE_RS485, HIGH);
}

void ModBusDriver::postTransmission() {
    digitalWrite(DE_RE_RS485, LOW);
}

void ModBusDriver::setSlaveID(uint8_t slaveID) {
    node.setSlaveId(mSlaveID);
    mSlaveID = slaveID;
}



/* 
 *  Soil Oxygen Sensor Functions
 * 
 */
float ModBusDriver::readSoilOxygenData()
{
    return  (float) (readRegister(soilOxygenSlaveID, SOIL_OXYGEN_REG) / 10.0);
}

void ModBusDriver::printSoilOxygenData() {
    float oxygenData = readSoilOxygenData();
    uint16_t roundedOxygenData = round(oxygenData);
    Serial.print("Soil Oxygen = ");
    Serial.println(roundedOxygenData);
}

void ModBusDriver::updateSoilOxygenData() {
    float oxygenData = readSoilOxygenData();
    mSquidData.soilOxygen = round(oxygenData);
}

bool ModBusDriver::setSoilOxygenSlaveAddress(uint8_t newAddress)
{ 
    bool result;
    result =  writeRegister(soilOxygenSlaveID, SLAVE_ADDRESS_REG, newAddress);

    if (result == true) {
        soilOxygenSlaveID = newAddress; 
        return true;
    } else {
        return false;
    }
}

/* 
 *  Soil Oxygen Sensor Functions
 *
 * 
 */

// Function to read temperature
float ModBusDriver::readTemperature() {
    uint16_t raw_data;
    raw_data = readRegister(soilMoistureSlaveID, TEMPERATURE_REG);

    // Check for negative temperature
    if (raw_data & 0x8000) {
        return (float)((int16_t)raw_data - 0xFFFF - 1) / 100.0;
    } else {
        return (float)raw_data / 100.0;
    }
}

// Function to read VWC (Volumetric Water Content)
float ModBusDriver::readVWC() {
    uint16_t raw_data;
    raw_data = readRegister(soilMoistureSlaveID, VWC_REG);
    Serial.print("raw data");
    Serial.println(raw_data);
    return (float)raw_data / 10000.0;  // Dividing by 10000 to convert to percentage
}

// Function to read EC (Electrical Conductivity)
float ModBusDriver::readEC() {
    uint16_t raw_data;
    raw_data = readRegister(soilMoistureSlaveID, EC_REG); 
    return (float)raw_data;  // Data is already in the correct units (us/cm)
}

// Function to read Salinity
float ModBusDriver::readSalinity() {
    uint16_t raw_data;
    raw_data = readRegister(soilMoistureSlaveID, SALINITY_REG); 
    return (float)raw_data;  // Data is already in the correct units (mg/L)
}

// Function to read TDS (Total Dissolved Solids)
float ModBusDriver::readTDS() {
    uint16_t raw_data;
    raw_data = readRegister(soilMoistureSlaveID, TDS_REG); 
    return (float)raw_data;  // Data is already in the correct units (mg/L)
}

// Function to read Epsilon
float ModBusDriver::readEpsilon() {
    uint16_t raw_data;
    raw_data = readRegister(soilMoistureSlaveID, EPSILON_REG); 
    return (float)raw_data / 100.0;  // Dividing by 100 to get actual value
}

// Function to read Soil Type
uint16_t ModBusDriver::readSoilType() {
    uint16_t raw_data;
    raw_data = readRegister(soilMoistureSlaveID, SOIL_TYPE_REG); 
    return raw_data;  // Data is already in the correct units (0-3 representing different soil types)
}

void ModBusDriver::printSoilMoistureData() {
    float temperatureData = readTemperature();
    //temperatureData = roundToDecimalPlaces(temperatureData, 2); 
    float soilMoistureData = readVWC();
    //soilMoistureData = roundToDecimalPlaces(soilMoistureData,2);
    Serial.print("Soil Temp Probe 0 = ");
    Serial.println(temperatureData);
    Serial.print("Soil Moisture = ");
    Serial.println(soilMoistureData);
}

void ModBusDriver::updateSoilMoistureData() {
    float temperatureData = readTemperature();
   // temperatureData = roundToDecimalPlaces(temperatureData, 2); 
    float soilMoistureData = readVWC(); 

    mSquidData.soilTempProbe = temperatureData; //round(temperatureData);
    mSquidData.soilMoisture =  soilMoistureData;
}


 
 