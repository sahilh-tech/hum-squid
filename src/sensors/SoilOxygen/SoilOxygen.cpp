
// #include "SoilOxygen.h"

//  // SoilMoisture* globalSoilMoistureObject = nullptr;  // Initialize the global pointer

 

// // SoilOxygen* globalSoilOxygenObject = nullptr;  // Initialize the global pointer
// // // SoilMoisture* globalSoilMoistureObject = nullptr;  // Initialize the global pointer

// // void globalPreTransmission() {  // Implement the trampoline function for preTransmission
// //     if (globalSoilOxygenObject) {
// //         globalSoilOxygenObject->preTransmission();
// //     }
// // }

// // void globalPostTransmission() {  // Implement the trampoline function for postTransmission
// //     if (globalSoilOxygenObject) {
// //         globalSoilOxygenObject->postTransmission();
// //     }
// // }


// SoilOxygen::SoilOxygen(ModBusDriver& modbusDriver, sensorData& squidData ,uint8_t slaveID) 
//             :   mModbusDriver(modbusDriver), mSquidData(squidData), mSlaveID(slaveID)
// {
 
// }

// bool SoilOxygen::init()
// {
//     //mModbusDriver.init(mSlaveID);
//     // node.begin(mSlaveID, Serial1);
//     // Serial1.begin(9600); 
//     // pinMode(DE_RE_RS485, OUTPUT);
 
//     // return true;
// }

// // void SoilOxygen::preTransmission()
// // {
// //   digitalWrite(DE_RE_RS485, HIGH);
// // }
// // // Pin 4 made low for Modbus receive mode
// // void SoilOxygen::postTransmission()
// // {
// //   digitalWrite(DE_RE_RS485, LOW);
// // }

// float SoilOxygen::readSoilOxygen()
// {
//     return mModbusDriver.readRegister(mSlaveID, 0x00);
// }


// bool SoilOxygen::setSlaveAddress(uint8_t newAddress)
// { 
//     bool result;
//     result =  mModbusDriver.writeRegister(mSlaveID, SLAVE_ADDRESS_REG, newAddress);
// //   node.setSlaveId(mSlaveID);
// //   result = node.writeSingleRegister(SLAVE_ADDRESS_REG, newAddress);
//   // if (result == node.ku8MBSuccess) {
//   if (result == true) {
//     mSlaveID = newAddress;
//     // node.begin(mSlaveID, Serial1);
//     return true;
//   } else {
//     return false;
//   }
// }

// void SoilOxygen::printSoilOxygenData() {
//     float oxygenData = readSoilOxygen();
//     uint16_t roundedOxygenData = round(oxygenData);
//     Serial.println(roundedOxygenData);
// }

// void SoilOxygen::updateSoilOxygenData() {
//     float oxygenData = readSoilOxygen();
//     mSquidData.soilOxygen = round(oxygenData);
// }