

#ifndef SOILOXYGEN_H
#define SOILOXYGEN_H

#include <ModbusMaster.h>
#include "config.h"


class SoilOxygen {
public:
  SoilOxygen(uint8_t slaveAddress=2);
  void init();
  float readSoilOxygen();
  bool setSlaveAddress(uint8_t newAddress);

private:
  uint8_t mSlaveAddress;
  ModbusMaster node;

  const uint16_t SOIL_OXYGEN_REG = 0x00;
  const uint16_t SLAVE_ADDRESS_REG = 0x30;
};

#endif // SOILOXYGENSENSOR_H
