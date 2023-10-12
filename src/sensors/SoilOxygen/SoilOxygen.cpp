
#include "SoilOxygen.h"

SoilOxygen::SoilOxygen(uint8_t slaveAddress) :mSlaveAddress(slaveAddress)
{
  
}

void SoilOxygen::init()
{
    node.begin(mSlaveAddress, Serial1);
    Serial1.begin(9600); 
    pinMode(DE_RE_RS485, OUTPUT);
     
}

float SoilOxygen::readSoilOxygen()
{
  uint8_t result;
  uint16_t data;
  float soilOxygen;

  result = node.readHoldingRegisters(SOIL_OXYGEN_REG, 1);

  if (result == node.ku8MBSuccess) {
    data = node.getResponseBuffer(0);
    soilOxygen = (float)data / 10.0;
    return soilOxygen;
  } else {
    return -1.0; // Indicate an error
  }
}

bool SoilOxygen::setSlaveAddress(uint8_t newAddress)
{
  uint8_t result;

  result = node.writeSingleRegister(SLAVE_ADDRESS_REG, newAddress);

  if (result == node.ku8MBSuccess) {
    mSlaveAddress = newAddress;
    node.begin(mSlaveAddress, Serial1);
    return true;
  } else {
    return false;
  }
}
