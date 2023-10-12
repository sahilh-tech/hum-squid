#include "SoilTemperature.h"
#include "Arduino.h"

// SoilTemperature::SoilTemperature(sensorData& squidData)
//   : mSquidData(squidData), 
//     mProbes{
//       TemperatureProbe(ADC_TEMP_PROBE_0),
//       TemperatureProbe(ADC_TEMP_PROBE_1),
//       TemperatureProbe(ADC_TEMP_PROBE_2)
//     }
// {
//   // Initialize probe data to zero
//   memset(mProbeData, 0, sizeof(mProbeData));
// }

// void SoilTemperature::init() {
//   // Initialize each TemperatureProbe
//   for (uint8_t i = 0; i < mNumProbes; ++i) {
//     mProbes[i].init();
//   }
// }

// void SoilTemperature::updateSoilTemperatureData() {
//   // Update each TemperatureProbe
//   for (uint8_t i = 0; i < mNumProbes; ++i) {
//     uint32_t sum = 0;
//     for (uint8_t j = 0; j < mNumSamples; ++j) {
//       sum += mProbes[i].read();
//       delay(1);  // Short delay to allow ADC to stabilize
//     }
//     mProbeData[i] = sum / mNumSamples;  // Average over the samples
//   }
  
//   // Update squidData
//   mSquidData.soilTempProbe1  = mProbeData[0];
//   mSquidData.soilTempProbe2 = mProbeData[1];
//   mSquidData.soilTempProbe3 = mProbeData[2];
// }

// void SoilTemperature::printProbeData(uint8_t probeIndex) {
//   if (probeIndex >= mNumProbes) {
//     Serial.println("Invalid probe index");
//     return;
//   }
//   Serial.print("Probe ");
//   Serial.print(probeIndex);
//   Serial.print(": ");
//   Serial.println(mProbeData[probeIndex]);
// }

// void SoilTemperature::printAllProbeData() {
//   for (uint8_t i = 0; i < mNumProbes; ++i) {
//     printProbeData(i);
//   }
// }
