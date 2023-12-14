#include "K30Sensor.h"

K30Sensor::K30Sensor(sensorData& squidData) : mSquidData(squidData) {
  // Initialize the sensor, if needed
}

uint16_t K30Sensor::readSensorData() {
  // Your existing readCO2 code here, with slight modifications
  uint16_t co2_value = 0; 
 

  // Begin Write Sequence
  Wire.beginTransmission(CO2_ADDRESS);
  Wire.write(0x22);
  Wire.write(0x00);
  Wire.write(0x08);
  Wire.write(0x2A);
  Wire.endTransmission();
  
  delay(10);  // Wait for the sensor to process the command

  // Begin Read Sequence
    Wire.requestFrom((uint8_t)CO2_ADDRESS, (uint8_t)4);

  byte i = 0;
  byte buffer[4] = {0, 0, 0, 0};
  while (Wire.available()) {
    buffer[i] = Wire.read();
    i++;
  }

  // Convert buffer into an integer
  co2_value = 0;
  co2_value |= buffer[1] & 0xFF;
  co2_value = co2_value << 8;
  co2_value |= buffer[2] & 0xFF;

  byte sum = 0; // Checksum Byte
  sum = buffer[0] + buffer[1] + buffer[2];

  if (sum == buffer[3]) { 
    return co2_value;
  } else { 
    return 65535; // checksum failure in error
  }
}

void K30Sensor::printCO2Data() {
  int co2Value = readSensorData();
  if ((co2Value > 0) && (co2Value < 10000)) {
    Serial.print("CO2 Data = ");
    Serial.println(co2Value);
  } else if (co2Value >= 65535 ){
    Serial.println("Checksum failed / Communication failure");
  }
}

void K30Sensor::updateCO2Data() {
  uint16_t co2Value = readSensorData();
  if ((co2Value > 0) && (co2Value < 10000)) {
    mSquidData.CO2 = co2Value;  
  } else if (co2Value >= 65535 ){
    Serial.println("co2 error, unable to update value");
  }

  if (co2Value > 0) {
  
  }
  // Handle error case if needed
}