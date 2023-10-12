#ifndef ModBusDriver_h
#define ModBusDriver_h

#include <ModbusMaster.h>

class ModBusDriver {
public:
    ModBusDriver();
    bool init();
    uint16_t readRegister(uint8_t slaveID, uint16_t regAddress);
    void writeRegister(uint8_t slaveID, uint16_t regAddress, uint16_t value);
    void preTransmission();
    void postTransmission();

private:
    ModbusMaster node;
};

#endif
