// k30_lib.h

#ifndef k30Sensor_h
#define k30Sensor_h

#include "Arduino.h"
#include "Wire.h"

class k30Sensor {
public:
    // Constructor: initializes the class with the I2C address of the CO2 sensor.
    k30Sensor(uint8_t i2cAddress, TwoWire &wirePort = Wire);

    // Setup function for initializing sensor and I2C communication.
    void setup();

    // Function to read CO2 concentration from the sensor.
    uint16_t read();

private:
    // I2C address of the CO2 sensor.
    uint8_t _i2cAddress = 0x68;

    // Reference to the I2C port used.
    TwoWire &_wire;
};

#endif