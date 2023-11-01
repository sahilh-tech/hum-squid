#ifndef DataManager_h
#define DataManager_h

#include <Arduino.h>
#include "SD_MMC.h" 
#include "FS.h"

#include "config.h" 

class DataManager {
  public:
    DataManager();
    void init();
    void updateData(sensorData newData);
    void writeToSDCard();
    void clearDataLog();

    bool tryTransmitToServer(); // This function tries to send data to server and returns true on success
    bool retrySDTransmitDataToServer();
    bool dataPendingTransmission = false;

  private:
    sensorData dataBuffer[3];
    int counter;
    const int maxIterations;

    // data management functions
    void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
    void createDir(fs::FS &fs, const char * path);
    void removeDir(fs::FS &fs, const char * path);
    void readFile(fs::FS &fs, const char * path);
    void writeFile(fs::FS &fs, const char * path, const char * message);
    void appendFile(fs::FS &fs, const char * path, const char * message);
    void renameFile(fs::FS &fs, const char * path1, const char * path2);
    void deleteFile(fs::FS &fs, const char * path);
    void testFileIO(fs::FS &fs, const char * path);


};

#endif