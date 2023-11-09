#include "Utilities.h"
  
float roundToDecimalPlaces(float value, int decimalPlaces) {
    float multiplier = std::pow(10.0f, decimalPlaces);  // calculate 10^n
    value = value * multiplier;  // shift decimal places to the right by n places
    value = std::round(value);   // round to the nearest integer
    value = value / multiplier;  // shift decimal places back to the left by n places
    return value;
}

String versionToString(const versionNumber& version) {
    return String(version.majorVersion) + "." + String(version.minorVersion) + "." + String(version.patchVersion);
}

bool isPositiveInteger(const String& str) {
  if (str.length() == 0) {
    return false;
  }
  for (unsigned int i = 0; i < str.length(); i++) {
    if (!isDigit(str.charAt(i))) {
      return false;
    }
  }
  return true;
}

bool isInteger(String str) {
    if (str.length() == 0) {
        return false;
    }
    
    for (int i = 0; i < str.length(); i++) {
        // Check for a negative sign at the first position
        if (i == 0 && str[i] == '-') {
            continue;
        }
        
        // Check if each character is a digit
        if (!isDigit(str[i])) {
            return false;
        }
    }
    
    return true;
}

void printSquidConfig(const squidConfig& config) {
  Serial.println("Squid Configurations:");
  Serial.println("---------------------");
  
  Serial.print("Drum Rotation Duration: ");
  Serial.print(config.drumRotationDuration);
  Serial.println(" seconds");
  
  Serial.print("Vent HVAC Duration: ");
  Serial.print(config.ventHVACDuration);
  Serial.println(" seconds");
  
  Serial.print("Temperature Threshold: ");
  Serial.print(config.temperatureThreshold);
  Serial.println(" degrees Celsius");
  
  Serial.print("CO2 Threshold: ");
  Serial.print(config.co2Threshold);
  Serial.println(" ppm");
  
  Serial.print("Ammonia Threshold: ");
  Serial.print(config.ammoniaThreshold);
  Serial.println(" ppm");
  
  Serial.print("WiFi SSID: ");
  Serial.println(config.wifiSSID);
  
  Serial.print("WiFi Password: ");
  // Uncomment the following line if you're okay with printing WiFi Passwords
  // Serial.println(config.wifiPassword);
  Serial.println("******");  // Masked for security reasons
  
  Serial.print("Squid ID: ");
  Serial.println(config.squidID);
  
  Serial.print("Node ID: ");
  Serial.println(config.nodeID);

  Serial.println("---------------------");
}


void printSensorData(const sensorData& data) {
  Serial.println("Sensor Data:");
  
  // Print Metadata
  Serial.print("Squid ID: ");
  Serial.println(data.squidID);
  
  Serial.print("Timestamp: ");
  Serial.println(data.timestamp);
  
  Serial.print("Node ID: ");
  Serial.println(data.nodeID);

  // Print Sensor Values
  Serial.print("Soil Oxygen: ");
  Serial.print(data.soilOxygen);
  Serial.println(" VOL %");

  Serial.print("CO2: ");
  Serial.print(data.CO2);
  Serial.println(" ppm");

  Serial.print("Ammonia: ");
  Serial.print(data.ammonia);
  Serial.println(" ppm");

  Serial.print("Soil Moisture: ");
  Serial.print(data.soilMoisture);
  Serial.println(" %");

  // Print Temperatures
  Serial.print("Soil Temp Probe: ");
  Serial.print(data.soilTempProbe);
  Serial.println(" °C");

  Serial.print("Soil Temp Probe 1: ");
  Serial.print(data.soilTempProbe1);
  Serial.println(" °C");

  Serial.print("Soil Temp Probe 2: ");
  Serial.print(data.soilTempProbe2);
  Serial.println(" °C");

  Serial.print("Soil Temp Probe 3: ");
  Serial.print(data.soilTempProbe3);
  Serial.println(" °C");

  Serial.print("Ambient Temp: ");
  Serial.print(data.ambientTemp);
  Serial.println(" °C");

  // Print Humidity
  Serial.print("Ambient Humidity: ");
  Serial.print(data.ambientHumidity);
  Serial.println(" %");

  // Print Enclosure Temp
  Serial.print("Enclosure Temp: ");
  Serial.print(data.enclosureTemp, 2);
  Serial.println(" °C");

  Serial.println("-----------------------");
}
