#ifndef ModBusDriver_h
#define ModBusDriver_h

#include "ModbusMaster.h"
#include "Utilities.h"
#include "config.h" 
#include <HardwareSerial.h>

/*
 NOTES: Do not immediately read from one device and then read from the next.
        IF you wait at least 50ms between each device read, it allows enough time
        for the ModBus driver to change 
*/
class ModBusDriver {
public:
    ModBusDriver(sensorData& squidData);
    bool init(uint8_t slaveID);


    //Soil Oxygen Sensor Functions
    float readSoilOxygenData();
    void printSoilOxygenData();
    void updateSoilOxygenData();
    bool setSoilOxygenSlaveAddress(uint8_t newAddress);

    //Soil Moisture Sensor Functions
    float readTemperature();        // Read temperature
    float readVWC();                // Read VWC
    float readEC();                 // Read EC
    float readSalinity();           // Read Salinity
    float readTDS();                // Read TDS
    float readEpsilon();            // Read Epsilon
    uint16_t readSoilType();        // Read Soil Type

    void printSoilMoistureData();
    void updateSoilMoistureData(); 

private:
    uint8_t mSlaveID;
    sensorData& mSquidData;
    ModbusMaster node;
    HardwareSerial serial2; // Add this line

    uint8_t soilOxygenSlaveID=2;    
    uint8_t soilMoistureSlaveID=1;
    // soil Oxygen Registers
    const uint16_t SOIL_OXYGEN_REG = 0x00;
    const uint16_t SLAVE_ADDRESS_REG = 0x30;

    //Soil Moisture ModBus Registers 
    const uint16_t TEMPERATURE_REG=0x0000;  // Corresponds to temperature in Celsius
    const uint16_t VWC_REG=0x0001;          // Corresponds to VWC (Volumetric Water Content)
    const uint16_t EC_REG=0x0002;           // Corresponds to EC (Electrical Conductivity)
    const uint16_t SALINITY_REG=0x0003;     // Corresponds to Salinity
    const uint16_t TDS_REG=0x0004;          // Corresponds to TDS (Total Dissolved Solids)
    const uint16_t EPSILON_REG=0x0005;      // Corresponds to Epsilon
    const uint16_t SOIL_TYPE_REG=0x0020;    // Corresponds to Soil Type

    // ModBus Driver Functions
    uint16_t readRegister(uint8_t slaveID, uint16_t regAddress);
    bool writeRegister(uint8_t slaveID, uint16_t regAddress, uint16_t value);
    void preTransmission();
    void postTransmission();
    void setSlaveID(uint8_t mSlaveID);

};

#endif
 