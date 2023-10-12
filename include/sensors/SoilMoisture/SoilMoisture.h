#ifndef SoilMoisture_h
#define SoilMoisture_h

#include <ModbusMaster.h>  // Include the ModbusMaster library
#include "config.h"

// Define Modbus registers
#define TEMPERATURE_REG 0x0000  // Corresponds to temperature in Celsius
#define VWC_REG 0x0001          // Corresponds to VWC (Volumetric Water Content)
#define EC_REG 0x0002           // Corresponds to EC (Electrical Conductivity)
#define SALINITY_REG 0x0003     // Corresponds to Salinity
#define TDS_REG 0x0004          // Corresponds to TDS (Total Dissolved Solids)
#define EPSILON_REG 0x0005      // Corresponds to Epsilon
#define SOIL_TYPE_REG 0x0020    // Corresponds to Soil Type


 
class SoilMoisture {
public:
    SoilMoisture(uint8_t slaveID = 1);  // Constructor
    bool init();                   // Initialize Modbus communication
    float readTemperature();        // Read temperature
    float readVWC();                // Read VWC
    float readEC();                 // Read EC
    float readSalinity();           // Read Salinity
    float readTDS();                // Read TDS
    float readEpsilon();            // Read Epsilon
    uint16_t readSoilType();        // Read Soil Type

    void preTransmission();
    void postTransmission();
private:
    ModbusMaster node;  // Modbus master node
    uint8_t mSlaveID; 

}; 

#endif  // SoilMoisture_h
