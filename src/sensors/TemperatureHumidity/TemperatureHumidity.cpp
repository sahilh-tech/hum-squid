 
#include "TemperatureHumidity.h"

TemperatureHumidity::TemperatureHumidity(TwoWire &wire, sensorData &data)
  : mSHT20(&wire, 0x40), mSquidData(data)  // initialize with the default I2C address
{ 
}

void TemperatureHumidity::init() {
  mSHT20.initSHT20();
}

float TemperatureHumidity::readTemp() {
  return mSHT20.readTemperature();
}

uint16_t TemperatureHumidity::readHumidity() {
  float humidity = mSHT20.readHumidity();
  return (uint16_t) round(humidity);  // Convert to 2 decimal places
}

void TemperatureHumidity::updateTempAndHumidity() {
  mSquidData.ambientTemp = readTemp();
  mSquidData.ambientHumidity = readHumidity();
}

void TemperatureHumidity::printTemp() {
  Serial.print("Ambient sensor Temperature: ");
  Serial.println(mSquidData.ambientTemp);
}

void TemperatureHumidity::printHumidity() {
  Serial.print("Ambient Sensor Humidity: ");
  Serial.println(mSquidData.ambientHumidity);
}

void TemperatureHumidity::printAll() {
  printTemp();
  printHumidity();
}
