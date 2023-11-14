#include "DataManager.h"
#include "SPI.h"

// code is based on 
// https://github.com/espressif/arduino-esp32/blob/master/libraries/SD_MMC/examples/SDMMC_Test/SDMMC_Test.ino
DataManager::DataManager() : counter(0), maxIterations(3) {
  // Moved hardware initialization to init()
}

void DataManager::init() {
  // Initialize SD card
  if(!SD_MMC.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  
  // Identify card type
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if(cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  // Get and display card size
  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  
  // Check if "datalog.csv" exists, if not, create it
  if (!SD_MMC.exists("/datalog.csv")) {
    writeFile(SD_MMC, "/datalog.csv", "Squid ID,Node ID,TimeStamp,Soil Oxygen,CO2,Ammonia,Soil Moisture,Soil Temp Probe,Soil Temp Probe 1,Soil Temp Probe 2,Soil Temp Probe 3,Ambient Temp,Ambient Humidity\n");
    Serial.println("datalog.csv created.");
  } else {
    Serial.println("datalog.csv already exists.");
  }
}

void DataManager::clearDataLog() {
  dataPendingTransmission = false;
  // Remove existing datalog.csv file
  if (SD_MMC.exists("/datalog.csv")) {
    deleteFile(SD_MMC, "/datalog.csv");
  }
  
  // Create a new datalog.csv file with just the header row
  writeFile(SD_MMC, "/datalog.csv", "Squid ID,Node ID,TimeStamp,Soil Oxygen,CO2,Ammonia,Soil Moisture,Soil Temp Probe,Soil Temp Probe 1,Soil Temp Probe 2,Soil Temp Probe 3,Ambient Temp,Ambient Humidity\n");
  Serial.println("datalog.csv cleared and header written.");
}

// Update the data buffer and possibly write to the SD card
void DataManager::updateData(sensorData newData) {
  // Update the data buffer
  dataBuffer[counter] = newData;
  counter++;

  // Check if it's time to attempt transmission
  if (counter >= maxIterations) {
    if (tryTransmitToServer()) { // If successful transmission
      counter = 0; // Reset counter
      clearDataLog(); // Optionally, clear the SD card
      Serial.println("Data successfully sent to server and local data cleared.");
    } else { // If failed transmission
      writeToSDCard(); // Write to SD card
      counter = 0; // Reset counter
      Serial.println("Data could not be sent to server, saved to SD card.");
    }
  }
}
// Write buffered data to the SD card
void DataManager::writeToSDCard() {
  String dataStr = "";
  for (int i = 0; i < maxIterations; i++) {
    dataStr += String(dataBuffer[i].squidID) + ",";
    dataStr += String(dataBuffer[i].nodeID) + ",";
    dataStr += String(dataBuffer[i].timestamp) + ",";
    dataStr += String(dataBuffer[i].soilOxygen) + ",";
    dataStr += String(dataBuffer[i].CO2) + ",";
    dataStr += String(dataBuffer[i].ammonia) + ",";
    dataStr += String(dataBuffer[i].soilMoisture, 2) + ",";
    dataStr += String(dataBuffer[i].soilTempProbe, 2) + ",";
    dataStr += String(dataBuffer[i].soilTempProbe1, 2) + ",";
    dataStr += String(dataBuffer[i].soilTempProbe2, 2) + ",";
    dataStr += String(dataBuffer[i].soilTempProbe3, 2) + ",";
    dataStr += String(dataBuffer[i].ambientTemp, 2) + ",";
    dataStr += String(dataBuffer[i].ambientHumidity) + "\n";
  }
  
  appendFile(SD_MMC, "/datalog.csv", dataStr.c_str());
  dataPendingTransmission = true;
  Serial.println("Data appended to datalog.csv");
}

/* 
 *  Wireless management system
 */

// This function attempts to send the dataBuffer to the server.
// Returns true if successful, false otherwise.
bool DataManager::tryTransmitToServer() {
  // Your code to send data to the server.
  // If successful:
  //     return true;
  // Else:
  //     return false;
  return true;
}
//retry data transmission
//not sure if I need a seperate function for this 
bool DataManager::retrySDTransmitDataToServer() {
    // If successful:
  //     return true;
  // Else:
  //     return false;
  return true;
}

/*
 * SD Card read/write functions
 */

void DataManager::listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void DataManager::createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void DataManager::removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void DataManager::readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
}

void DataManager::writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
}

void DataManager::appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
}

void DataManager::renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void DataManager::deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void DataManager::testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %lu ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %lu ms\n", 2048 * 512, end);
    file.close();
}
