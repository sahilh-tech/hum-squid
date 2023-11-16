#include "ModBusDriver.h"
 
 
 

ModBusDriver::ModBusDriver(sensorData& squidData) : 
    mSquidData(squidData), 
    serial2(2) {
    
}

bool ModBusDriver::init(uint8_t slaveID) {
    mSlaveID = slaveID;
    Serial1.begin(9600); 
    // node.begin(mSlaveID, Serial1);
    // serial2.begin(9600,SERIAL_8N1 , RO_RS485, DI_RS485); // Use member variable serial2
    node.begin(mSlaveID, Serial1); // Pass serial2 to ModbusMaster

 

   // pinMode(DE_RE_RS485, OUTPUT);
    node.preTransmission([this]() { this->preTransmission(); });
    node.postTransmission([this]() { this->postTransmission(); });
    return true;
}

void  ModBusDriver::testLoopBack(){
  serial2.write("Hello UART2"); // Send data on UART2

  delay(1000); // Wait for data to be transmitted and received back

  // Read the received data from UART2 and print it on the debug Serial port
  while (serial2.available()) {
    char c = serial2.read(); // Read the data back from UART2
    Serial.print(c); // Print to the debug serial port (UART0)
  }

  Serial.println(); // Print a newline for readability
  delay(2000); // Wait before sending the next message
}

uint16_t ModBusDriver::readRegister(uint8_t slaveID, uint16_t regAddress) {
    uint8_t result;
    uint16_t data[1];
     Serial.println(" setting slaveID... ");
    node.setSlaveId(slaveID);
        Serial.println("finished setting slaveID");
    result = node.readHoldingRegisters(regAddress, 1);
        Serial.print("result finished reading = ");
            Serial.println(result);
   // data[0] = node.getResponseBuffer(0);
    //return data[0];
    if (result == node.ku8MBSuccess)
    {
        return data[0] = node.getResponseBuffer(0);
    }  else {
        return 65535; // Return max value on failure
    }
    
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
  //  digitalWrite(DE_RE_RS485, HIGH);
}

void ModBusDriver::postTransmission() {
  //  digitalWrite(DE_RE_RS485, LOW);
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
    Serial.println(oxygenData);
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
    Serial.print("inisde readTempetature");
    raw_data = readRegister(soilMoistureSlaveID, TEMPERATURE_REG);
    Serial.print("finished readRegister ");

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
    Serial.print("Soil Moisture probe= ");
    Serial.println(soilMoistureData);
}

void ModBusDriver::updateSoilMoistureData() {
       Serial.println("inside updateSoilMoistureData");
    float temperatureData = readTemperature();
    Serial.println("inside readTemperature complete...");
    temperatureData = roundToDecimalPlaces(temperatureData, 2); 
    float soilMoistureData = readVWC(); 
     Serial.println("inside readVWC complete...");
    mSquidData.soilTempProbe = temperatureData; //round(temperatureData);
    mSquidData.soilMoisture =  soilMoistureData;

}


 
 