#include "ModBusDriver.h"
#include "config.h"
ModBusDriver::ModBusDriver() {
    // Initialization code if any
}

bool ModBusDriver::init() {
    node.begin(1, Serial1);
    Serial1.begin(9600);
    pinMode(DE_RE_RS485, OUTPUT);
    return true;
}

uint16_t ModBusDriver::readRegister(uint8_t slaveID, uint16_t regAddress) {
    uint16_t result;
   // node.setSlave(slaveID);
    node.readHoldingRegisters(regAddress, 1);
    result = node.getResponseBuffer(0);
    return result;
}

void ModBusDriver::writeRegister(uint8_t slaveID, uint16_t regAddress, uint16_t value) {
    //node.setSlave(slaveID);
    node.writeSingleRegister(regAddress, value);
}

void ModBusDriver::preTransmission() {
    digitalWrite(DE_RE_RS485, HIGH);
}

void ModBusDriver::postTransmission() {
    digitalWrite(DE_RE_RS485, LOW);
}
