 
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
    float newTemp = readTemp();
    int16_t newHumidity = readHumidity();

    // Update ambient temperature if within 10 units of the current value or if the current value is 0 (initial reading)
    if (abs(mSquidData.ambientTemp - newTemp) <= 10 || mSquidData.ambientTemp == 0.0f) {
        mSquidData.ambientTemp = newTemp;
    } else {
        Serial.println("New temperature reading is too different from the current value, not updating.");
    }

    // Update ambient humidity if within 10 units of the current value or if the current value is 0 (initial reading)
    if (abs(mSquidData.ambientHumidity - newHumidity) <= 10 || mSquidData.ambientHumidity == 0) {
        mSquidData.ambientHumidity = newHumidity;
    } else {
        Serial.println("New humidity reading is too different from the current value, not updating.");
    }
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
