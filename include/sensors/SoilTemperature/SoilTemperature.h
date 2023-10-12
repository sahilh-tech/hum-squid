#ifndef __SOIL_TEMPERATURE_H__
#define __SOIL_TEMPERATURE_H__

#include "TemperatureProbe.h"
#include "config.h"  // Include this to get ADC_TEMP_PROBE_x definitions

class SoilTemperature {
public:
    // SoilTemperature(sensorData& squidData);
    // void init();
    // void updateSoilTemperatureData();
    // void printProbeData(uint8_t probeIndex);
    // void printAllProbeData();

private:
    static const uint8_t mNumProbes = 3;
    static const uint8_t mNumSamples = 10;
    TemperatureProbe mProbes[mNumProbes];
    uint16_t mProbeData[mNumProbes];
    sensorData& mSquidData;
};

#endif // __SOIL_TEMPERATURE_H__