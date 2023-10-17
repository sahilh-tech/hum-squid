

#ifndef SOILOXYGEN_H
#define SOILOXYGEN_H
 
#include "ModBusDriver.h"
#include "config.h"
 

class SoilOxygen {
public:
  SoilOxygen(ModBusDriver& modbusDriver, sensorData& squidData, uint8_t slaveID=2); //default modbus address = 2
  bool init();
  float readSoilOxygen();
  bool setSlaveAddress(uint8_t newAddress);
  void printSoilOxygenData();
  void updateSoilOxygenData();

  void preTransmission();
  void postTransmission();
private:
  uint8_t mSlaveID;
  // ModbusMaster node;
  sensorData mSquidData;
  ModBusDriver mModbusDriver;
  const uint16_t SOIL_OXYGEN_REG = 0x00;
  const uint16_t SLAVE_ADDRESS_REG = 0x30;
};

#endif // SOILOXYGENSENSOR_H
