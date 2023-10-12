#include "SerialMenu.h"
#include <EEPROM.h>  // Include the EEPROM library



SerialMenu::SerialMenu(versionNumber firmware, versionNumber hardware, squidConfig& config, 
                        sensorData& squidData, uint16_t eepromSize) :
    mFirmware(firmware), 
    mHardware(hardware), 
    mConfig(config),
    mSquidData(squidData),
    mEepromSize(eepromSize) {

}

void SerialMenu::init(){
    // Initialize Serial
    Serial.begin(115200);
    // Initialize EEPROM

    if(mEepromSize > MAX_ALLOCATED_EEPROM ) {
        Serial.println("Error Invalid EEPROM SIZE, cannot initialise Write to Memory");
    } else {
        EEPROM.begin(mEepromSize);  // Initialize EEPROM
    }
    
    // load default values
    loadConfig();

}

void SerialMenu::loadConfig(){
    // Load values from EEPROM into the mConfig structure
    mConfig.drumRotationDuration = getDrumRotation(false); // get drum rotation duration
    mConfig.ventHVACDuration = getVentControl(false); // get vent control duration
    mConfig.temperatureThreshold = getTempThreshold(false); // get temperature threshold
    mConfig.co2Threshold = getCO2Threshold(false); // get CO2 threshold
    mConfig.ammoniaThreshold = getAmmoniaThreshold(false); // get Ammonia threshold

    // For WiFi SSID and Password, you'll have to read them directly since you don't have getter functions for these
    char ssidBuffer[maxWifiSSIDLength];
    getWifiSSID(ssidBuffer, false); // get WiFi SSID
    mConfig.wifiSSID = String(ssidBuffer);

    // Assuming you have a getWifiPassword function similar to getWifiSSID
    // char passwordBuffer[maxWifiPasswordLength];
    // getWifiPassword(passwordBuffer, false); // get WiFi Password
    // mConfig.wifiPassword = String(passwordBuffer);

    mConfig.squidID = getSquidID(false); // get Squid ID
    mConfig.nodeID = getNodeID(false); // get Node ID
    mSquidData.nodeID = mConfig.nodeID;
    mSquidData.squidID = mConfig.squidID;
    Serial.println("Successfully loaded parameters into program");
    // printSquidConfig(mConfig);
}

void SerialMenu::runSerialMenu() {
    handleInput();
}

void SerialMenu::printHelp() {
    // Print the available commands
    Serial.println("Available commands:");
    Serial.println("help - Prints this help menu");
    Serial.println("getVersion - Prints firmware and hardware version");
    Serial.println("setDrumRotation [seconds] - Sets drum rotation time");
    Serial.println("getDrumRotation - Gets drum rotation time");
    Serial.println("setVentControl [seconds] - Sets Vent Signal On time");
    Serial.println("getVentControl - Gets Vent Signal On time");
    Serial.println("setTempThreshold [degrees] - Sets temperature threshold");
    Serial.println("getTempThreshold - Gets temperature threshold");
    Serial.println("setCO2Threshold [ppm] - Sets CO2 threshold");
    Serial.println("getCO2Threshold - Gets CO2 threshold");
    Serial.println("setAmmoniaThreshold [ppm] - Sets Ammonia threshold");
    Serial.println("getAmmoniaThreshold - Gets Ammonia threshold");
    Serial.println("setWifi [SSID] [Password] - Sets WiFi credentials");
    Serial.println("setSquidID [ID] - Sets Squid ID");
    Serial.println("getSquidID - Gets Squid ID");
    Serial.println("setNodeID [ID] - Sets Node ID");
    Serial.println("getNodeID - Gets Node ID");
    Serial.println("printConfigTable - Prints all items written to memory");
}



void SerialMenu::setDrumRotation(uint32_t seconds) {
    // Check if the value is within the range of uint16_t
    if (seconds >= minDrumRotation && seconds <= maxDrumRotation)  {
        uint16_t current_value;
        EEPROM.get(DRUM_ROTATION_ADDR, current_value);
        if (current_value != seconds) {
            EEPROM.put(DRUM_ROTATION_ADDR, seconds);

            // Commit changes to EEPROM
            EEPROM.commit();
            loadConfig();
            Serial.print("Successfully updated Drum Rotation with ");
            Serial.println(seconds);
        } else {
            Serial.print("Unable to update value of Drum Rotation with: ");
            Serial.println(seconds);
            Serial.print("Drum rotation is already set to this value");
        }
    } else {
        Serial.print("Invalid drum rotation value. Enter an integer between ");
        Serial.print(minDrumRotation);
        Serial.print(" and ");
        Serial.print(maxDrumRotation);
        Serial.println(".");    
    }
}
 
uint16_t SerialMenu::getDrumRotation(bool verbose) {
    uint16_t value;
    EEPROM.get(DRUM_ROTATION_ADDR, value);
    if (verbose) {
        Serial.print("Drum Rotation [seconds] = ");
        Serial.println(value);
    }
    return value;
}

void SerialMenu::setVentControl(uint32_t seconds) {
        // Check if the value is within the range of uint16_t
        if (seconds >= minVentControl && seconds <= maxVentControl)   {
        uint16_t current_value;
        EEPROM.get(VENT_CONTROL_ADDR, current_value);
        if (current_value != seconds) {
            EEPROM.put(VENT_CONTROL_ADDR, seconds);

            // Commit changes to EEPROM
            EEPROM.commit();
            loadConfig();
            Serial.print("Successfully updated Vent On Duration with ");
            Serial.println(seconds);
        } else {
            Serial.print("Unable to update value of Vent On Duration with: ");
            Serial.println(seconds);
            Serial.print("Vent On Duration is already set to this value");
        }
    } else {
        Serial.print("Invalid Vent On Duration value. Enter an integer between ");
        Serial.print(minVentControl);
        Serial.print(" and ");
        Serial.print(maxVentControl);
        Serial.println(".");    
    }
}
uint16_t SerialMenu::getVentControl(bool verbose){
    uint16_t value;
    EEPROM.get(VENT_CONTROL_ADDR, value);
    if (verbose) {
        Serial.print("Vent On Duration [seconds] = ");
        Serial.println(value);
    }
    return value;
}


void SerialMenu::setTempThreshold(int32_t degrees) {
    if (degrees >= minTempThreshold && degrees <= maxTempThreshold) {
        int8_t current_value;
        EEPROM.get(TEMP_THRESHOLD_ADDR, current_value);
        if (current_value != degrees) {
            EEPROM.put(TEMP_THRESHOLD_ADDR, degrees);
            // Commit changes to EEPROM
            EEPROM.commit(); 
            loadConfig();
            Serial.print("Successfully updated Temperature Threshold with ");
            Serial.println(degrees);
        }  else {
            Serial.print("Unable to update value of Temperature Threshold with: ");
            Serial.println(degrees);
            Serial.print("Temperature Threshold is already set to this value");
        }
    } else {
        Serial.print("Invalid temperature Threshold value. Enter an integer between ");
        Serial.print(minTempThreshold);
        Serial.print(" and ");
        Serial.print(maxTempThreshold);
        Serial.println(".");   
    }
}

int8_t SerialMenu::getTempThreshold(bool verbose) {
    int8_t value;
    EEPROM.get(TEMP_THRESHOLD_ADDR, value);
    if (verbose) {
        Serial.print("Temp Threshold [degrees celsius] = ");
        Serial.println(value);
    }
    return value;
}

void SerialMenu::setCO2Threshold(uint32_t ppm) {
    if (ppm >= minCO2Threshold && ppm <= maxCO2Threshold) {
        uint16_t current_value;
        EEPROM.get(CO2_THRESHOLD_ADDR, current_value);
        if (current_value != ppm) {
            EEPROM.put(CO2_THRESHOLD_ADDR, ppm);
            // Commit changes to EEPROM
            EEPROM.commit();
            loadConfig();
            Serial.print("Successfully updated CO2 Threshold with ");
            Serial.println(ppm);
        } else {
            Serial.print("Unable to update value of CO2 Threshold with: ");
            Serial.println(ppm);
            Serial.print("CO2 Threshold is already set to this value");
        }
    } else {
        Serial.print("Invalid CO2 Threshold value. Enter an integer between");
        Serial.print(minCO2Threshold);
        Serial.print(" and ");
        Serial.print(maxCO2Threshold);
        Serial.println(".");    
    }
}

uint16_t SerialMenu::getCO2Threshold(bool verbose) {
    uint16_t value;
    EEPROM.get(CO2_THRESHOLD_ADDR, value);
    if (verbose) {
        Serial.print("CO2 Threshold [ppm] = ");
        Serial.println(value);
    }
    return value;
}

void SerialMenu::setAmmoniaThreshold(uint32_t ppm) {
    if (ppm >= 0 && ppm <= 100) {
        uint8_t current_value;
        EEPROM.get(AMMONIA_THRESHOLD_ADDR, current_value);
        if (current_value != ppm) {
            EEPROM.put(AMMONIA_THRESHOLD_ADDR, ppm);
            // Commit changes to EEPROM
            EEPROM.commit();
            loadConfig();
            Serial.print("Successfully updated Ammonia Threshold with ");
            Serial.println(ppm); 
        }  else { 
            Serial.print("Unable to update value of Ammonia Threshold with: ");
            Serial.println(ppm);
            Serial.print("CO2 Threshold is already set to this value");
        }
    } else {
            Serial.print("Invalid Ammonia Threshold value. Enter an integer between");
            Serial.print(minAmmoniaThreshold);
            Serial.print(" and ");
            Serial.print(maxAmmoniaThreshold);
            Serial.println(".");  
    }
}

uint8_t SerialMenu::getAmmoniaThreshold(bool verbose) {
    uint8_t value;
    EEPROM.get(AMMONIA_THRESHOLD_ADDR, value);
    if (verbose) {
        Serial.print("Ammonia Threshold [ppm] = ");
        Serial.println(value);
    }
    return value;
}

 void SerialMenu::setWifiCredentials(const String& ssid, const String& password) {
    // Check if the SSID and Password are within the allowable length
    if (ssid.length() > maxWifiSSIDLength) {
        Serial.print("SSID is too long. It should be at most ");
        Serial.print(maxWifiSSIDLength);
        Serial.println(" characters.");
        return;
    }
    if (password.length() > maxWifiPasswordLength) {
        Serial.print("Password is too long. It should be at most ");
        Serial.print(maxWifiPasswordLength);
        Serial.println(" characters.");
        return;
    }

    // Read current values from EEPROM
    char current_ssid[maxWifiSSIDLength];
    char current_password[maxWifiPasswordLength];
    String currentSSID = readStringFromEEPROM(WIFI_SSID_ADDR, maxWifiSSIDLength);
    String currentPassword = readStringFromEEPROM(WIFI_PASS_ADDR, maxWifiPasswordLength);
    if (ssid != currentSSID) {
        writeStringToEEPROM(WIFI_SSID_ADDR, ssid, maxWifiSSIDLength);
        loadConfig();
        Serial.print("Successfully updated SSID with ");
        Serial.println(ssid);
    } else {
        Serial.println("Unable to update SSID because it's the same as what is already stored.");
    }

    if (password != currentPassword) {
        writeStringToEEPROM(WIFI_PASS_ADDR, password, maxWifiPasswordLength);
        loadConfig();
        Serial.print("Successfully updated Password with ");
        Serial.println(password);
    } else {
        Serial.println("Unable to update Password because it's the same as what is already stored.");
    }
}



void SerialMenu::getWifiSSID(char ssidBuffer[MAX_WIFI_SSID_LENGTH], bool verbose) {
   
    // Read the SSID from EEPROM
    String current_ssid = readStringFromEEPROM(WIFI_SSID_ADDR, maxWifiSSIDLength);
 
    
    if (verbose) { 
        Serial.print("Wifi SSID = ");
        Serial.println(current_ssid);
    }

    // Copy to the provided buffer
    strncpy(ssidBuffer, current_ssid.c_str(), MAX_WIFI_SSID_LENGTH);
}


void SerialMenu::setSquidID(uint32_t id) {
    uint16_t current_id;
    EEPROM.get(SQUID_ID_ADDR, current_id);
    if (current_id != id) {
        EEPROM.put(SQUID_ID_ADDR, id);
        // Commit changes to EEPROM
        EEPROM.commit();
        loadConfig();
        Serial.print("Successfully updated SquidID with ");
        Serial.println(id);
    } else {
         Serial.print("Unable to update SquidID because it's the same as what is already stored. ");
    }
}

uint16_t SerialMenu::getSquidID(bool verbose) {
    uint16_t id;
    EEPROM.get(SQUID_ID_ADDR, id);
    if (verbose) {
        Serial.print("Squid ID = "); 
        Serial.println(id);
    }
    return id;
}

void SerialMenu::setNodeID(uint32_t id) {
    uint16_t current_id;
    EEPROM.get(NODE_ID_ADDR, current_id);
    if (current_id != id) {
        EEPROM.put(NODE_ID_ADDR, id);
        // Commit changes to EEPROM
        EEPROM.commit();
        loadConfig();
        Serial.print("Successfully updated NodeID with ");
        Serial.println(id);
    } else {
         Serial.print("Unable to update NodeID because it's the same as what is already stored. ");
    }
}

uint16_t SerialMenu::getNodeID(bool verbose) {
    uint16_t id;
    EEPROM.get(NODE_ID_ADDR, id); 
    if (verbose) {
        Serial.print("Node ID = "); 
        Serial.println(id);
    }
    return id;
} 

void SerialMenu::printVersion() {
    Serial.print("Firmware Version = ");
    Serial.println(versionToString(mFirmware));
    Serial.print("Hardware Version = ");
        Serial.println(versionToString(mHardware));
}

void SerialMenu::printConfigTable() {
  // Header
  Serial.println("+------------------+---------+--------+");
  Serial.println("| Parameter        | Value   | Units  |");
  Serial.println("+------------------+---------+--------+");

  // Firmware Version
  Serial.print("| Firmware Version | ");
  Serial.print(versionToString(mFirmware));
  Serial.println("  |        |");

  // Hardware Version
  Serial.print("| Hardware Version | ");
  Serial.print(versionToString(mHardware));
  Serial.println("  |        |");

  // Drum Rotation
  Serial.print("| Drum Rotation    | ");
  Serial.print(getDrumRotation(false));
  Serial.println("    | seconds |");

  // Vent Control
  Serial.print("| Vent Control     | ");
  Serial.print(getVentControl(false));
  Serial.println("    | seconds |");

  // Temperature Threshold
  Serial.print("| Temp. Threshold  | ");
  Serial.print(getTempThreshold(false));
  Serial.println("      | C       |");

  // CO2 Threshold
  Serial.print("| CO2 Threshold    | ");
  Serial.print(getCO2Threshold(false));
  Serial.println("    | ppm     |");

  // Ammonia Threshold
  Serial.print("| Ammonia Threshold| ");
  Serial.print(getAmmoniaThreshold(false));
  Serial.println("      | ppm     |");

  // WiFi SSID
  char ssid[maxWifiSSIDLength];  // Assuming a maximum of 32 characters for SSID + null terminator
  getWifiSSID(ssid, false);      
  Serial.print("| WiFi SSID        | ");
  Serial.print(ssid);
  Serial.println("             |");

  // Squid ID
  Serial.print("| Squid ID         | ");
  Serial.print(getSquidID(false));
  Serial.println("      |        |");

  // Node ID
  Serial.print("| Node ID          | ");
  Serial.print(getNodeID(false));
  Serial.println("      |        |");

  // Footer
  Serial.println("+------------------+---------+--------+");
}

/*
 *         SERIAL FUNCTIONALITY
 *
 */

String SerialMenu::readSerialLine() {
    String received = "";
    char ch;
    while (Serial.available()) {
        // Read a character from Serial
        ch = Serial.read();
        
        // Check for end of line
        if (ch == '\n' || ch == '\r') {
            Serial.println("received cmomand");
            break;
        }
        
        // Append the read character to the string
        received += ch;
    }
    
    // Wait a bit to allow the buffer to clear and
    // any remaining characters to be read
    delay(20);
    
    // Clear the buffer to remove any remaining newline characters
    while (Serial.available()) {
        Serial.read();
    }
    
    return received;
}
void SerialMenu::handleInput() {
    // Handle user input from the Serial interface
    String input = readSerialLine();
    if (input.length() > 0) {
        if (input.startsWith("help")) {
            printHelp();
        } else if (input.startsWith("getVersion")) {
            printVersion();
        } else if (input.startsWith("setDrumRotation ")) {
            String arg = input.substring(16);
 
            if (isPositiveInteger(arg)) {
                setDrumRotation(arg.toInt());
            } else {
                Serial.println("Invalid argument. Please enter a positive integer for drum rotation.");
            }
        } else if (input.startsWith("getDrumRotation")) {
            Serial.println(getDrumRotation());
        }  else if (input.startsWith("setVentControl ")) {
            String arg = input.substring(16);
            if (isPositiveInteger(arg)) {
                setVentControl(arg.toInt());
            } else {
                Serial.println("Invalid argument. Please enter a positive integer for Vent Control.");
            }
        } else if (input.startsWith("getVentControl")) {
            Serial.println(getVentControl());
        } else if (input.startsWith("setTempThreshold ")) {
            String arg = input.substring(17);
            // Check if valid integer (including negative)
            if (isInteger(arg)) {
                setTempThreshold(arg.toInt()); 
            } else {
                Serial.println("Invalid argument. Please enter an integer between -20 and 100 for temperature threshold.");
            }
        } else if (input.startsWith("getTempThreshold")) {
            Serial.println(getTempThreshold());
        } else if (input.startsWith("setCO2Threshold ")) {
            String arg = input.substring(16);
            if (isInteger(arg)) {
                setCO2Threshold(arg.toInt());
            } else {
                Serial.println("Invalid argument. Please enter an integer between 0 and 10000 for CO2 threshold.");
            }
        } else if (input.startsWith("getCO2Threshold")) {
            Serial.println(getCO2Threshold());
        } else if (input.startsWith("setAmmoniaThreshold ")) {
            String arg = input.substring(20);
            if (isInteger(arg)) {
                setAmmoniaThreshold(arg.toInt());
            } else {
                Serial.println("Invalid argument. Please enter an integer between 0 and 100 for Ammonia threshold.");
            }
        } else if (input.startsWith("getAmmoniaThreshold")) {
            Serial.println(getAmmoniaThreshold());
        } else if (input.startsWith("setWifi ")) {
            int firstSpace = input.indexOf(' ', 8);
            String ssid = input.substring(8, firstSpace);
            String password = input.substring(firstSpace + 1);
            setWifiCredentials(ssid, password);
        } else if (input.startsWith("setSquidID ")) {
            String arg = input.substring(11);
            if (isInteger(arg)) {
                uint16_t id = arg.toInt();
                setSquidID(id);
            } else {
                Serial.println("Invalid argument. Please enter a valid Squid ID as an unsigned integer.");
            }
        } else if (input.startsWith("getSquidID")) {
            getSquidID();
        } else if (input.startsWith("setNodeID ")) {
            String arg = input.substring(10);
             if (isInteger(arg)) {
                uint16_t id = arg.toInt();
                setNodeID(id);
            } else {
                Serial.println("Invalid argument. Please enter a valid Node ID as an unsigned integer.");
            }
        } else if (input.startsWith("getNodeID")) {
            getNodeID();
        } else if (input.startsWith("printConfigTable")){
            printConfigTable();
        } else if (input.startsWith("factoryReset")) {
            factoryReset();
            Serial.println("Factory Reset executed.");
        }
        else if (input.startsWith("hardMemoryReset")) {
            hardMemoryReset();
            Serial.println("Hard Memory Reset executed.");
        } else
        {
            Serial.println("Invalid command. Type 'help' for a list of available commands.");
        }
    }
}

void SerialMenu::writeStringToEEPROM(uint16_t addr, const String& data, uint8_t maxLength) {
        if (data.length() > maxLength - 1) {
            Serial.print("Data too long for address ");
            Serial.print(addr);
            return;
        }
        EEPROM.write(addr, data.length());
        for (uint8_t i = 0; i < data.length(); ++i) {
            EEPROM.write(addr + 1 + i, data[i]);
        }
        EEPROM.commit();  // Commit changes to EEPROM
    }

String SerialMenu::readStringFromEEPROM(uint16_t addr, uint8_t maxLength) {
        uint8_t length = EEPROM.read(addr);
        if (length > maxLength - 1) {
            Serial.print("Stored data at address ");
            Serial.print(addr);
            Serial.print(" is too long.");
            return "";
        }
        char data[length + 1];
        for (uint8_t i = 0; i < length; ++i) {
            data[i] = EEPROM.read(addr + 1 + i);
        }
        data[length] = '\0';
        return String(data);
    }
 
    
void SerialMenu::factoryReset(){
    // Reset drum rotation time to a default 5 mins
    EEPROM.put(DRUM_ROTATION_ADDR, (uint16_t)300);

    // Reset vent control to a default 5 mins
    EEPROM.put(VENT_CONTROL_ADDR, (uint16_t)300);

    // Reset temperature threshold to a default  70 degrees
    EEPROM.put(TEMP_THRESHOLD_ADDR, (int8_t)70);

    // Reset CO2 threshold to a default 5000 ppm
    EEPROM.put(CO2_THRESHOLD_ADDR, (uint16_t)5000);

    // Reset Ammonia threshold to a default  10 ppm
    EEPROM.put(AMMONIA_THRESHOLD_ADDR, (uint8_t)10);

    // Reset WiFi SSID and Password to default empty strings
    char defaultSSID[maxWifiSSIDLength] = "";
    char defaultPassword[maxWifiPasswordLength] = "";
    writeStringToEEPROM(WIFI_SSID_ADDR, defaultSSID, maxWifiSSIDLength);
    writeStringToEEPROM(WIFI_PASS_ADDR, defaultPassword, maxWifiPasswordLength);

    // Reset Squid ID to a default (say, 0)
    EEPROM.put(SQUID_ID_ADDR, (uint16_t)0);

    // Reset Node ID to a default (say, 0)
    EEPROM.put(NODE_ID_ADDR, (uint16_t)0);

    // Commit changes to EEPROM
    EEPROM.commit();
    loadConfig();

    Serial.println("Factory reset completed. All values set to their defaults.");
}

// Function to perform a hard memory reset by clearing the EEPROM
void SerialMenu::hardMemoryReset() {
  // Clear all of the EEPROM memory (assuming 512 bytes allocated)
  for (int i = 0; i < mEepromSize; i++) {
    EEPROM.write(i, 0);
  }

  // Commit changes to EEPROM
  EEPROM.commit();
  loadConfig(); 

  Serial.println("Hard memory reset completed. All EEPROM bytes set to zero.");
}