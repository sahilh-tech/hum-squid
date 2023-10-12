#include "AmmoniaSensor.h"

AmmoniaSensor::AmmoniaSensor(TwoWire& wire, sensorData& squidData) 
                    : gasSensor(&wire, I2C_ADDRESS),
                      mSquidData(squidData) {}

bool AmmoniaSensor::init() {
  if (!gasSensor.begin()) {
    return false;
  }
  gasSensor.setTempCompensation(gasSensor.OFF);
  gasSensor.changeAcquireMode(gasSensor.INITIATIVE);
  delay(1000);

  //start warmup timer
  warmUpStartTime = millis(); 

  return true;
}

void AmmoniaSensor::updateAmmoniaConcentration(){
  if (gasSensor.dataIsAvailable()) {
     mSquidData.ammonia = (uint16_t) AllDataAnalysis.gasconcentration;
  } else {
    Serial.println("Error Reading Ammonia Sesnor Data, unable to print values");
    // should set LED to orange solid
  }
}

void AmmoniaSensor::getAndSaveAmmoniaReading() {
  if (gasSensor.dataIsAvailable()) {
    mSquidData.ammonia = (uint16_t) round(AllDataAnalysis.gasconcentration);
    mSquidData.enclosureTemp = round(AllDataAnalysis.temp * 100) / 100.0f;
  } else {
    Serial.println("Error Reading Ammonia Sensor Data, unable to save value");
    // You can set an error flag or indicator here if needed
  }
}

void AmmoniaSensor::printGasConcentration() {
  if (gasSensor.dataIsAvailable()) {
    Serial.print("Gas Concentration: ");
    Serial.print(AllDataAnalysis.gasconcentration);
    Serial.println(" PPM");
  }
}

void AmmoniaSensor::printTemperature() {
  if (gasSensor.dataIsAvailable()) {
    Serial.print("Temperature: ");
    Serial.print(AllDataAnalysis.temp);
    Serial.println(" ℃");
  }
}

void AmmoniaSensor::printAll() {
  if (gasSensor.dataIsAvailable()) {
    printGasConcentration();
    printTemperature();
  }
}



// void AmmoniaSensor::runScheduler() {
//   runner.execute();
// }


// void AmmoniaSensor::warmUpCallback() {
//   if (millis() - warmUpStartTime >= WARM_UP_TIME) {
//     isWarmupComplete = true;
//   }
// }

// void AmmoniaSensor::checkWarmUpStatus() {
//   if (isWarmupComplete) {
//     Serial.println("Ammonia sensor warm-up complete.");
//     warmUpTask.disable();
//   }
// }
