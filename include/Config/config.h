/*!
  * @file  Config.h
  * @brief This is a header file that contains all config parameters for the platform
  * @copyright   Copyright (c) 2023 Sahil Harriram Tech Pty Ltd.
  * @license     The MIT License (MIT)
  * @author      Sahil Harriram (Hello@sahil.harriram.tech)
  * @version     V0.0.1
  * @date        2023-09-02 
*/
#ifndef __CONFIG_H__
#define __CONFIG_H__
#pragma once 

#include "Arduino.h"


//Config Data Strucutres
struct versionNumber{
	const uint8_t majorVersion;
	const uint8_t minorVersion;
	const uint8_t patchVersion;

    versionNumber(uint8_t major, uint8_t minor, uint8_t patch) 
        : majorVersion(major), minorVersion(minor), patchVersion(patch) 
    {}
};

struct squidConfig{
  uint16_t drumRotationDuration;
  uint16_t ventHVACDuration;
  int8_t temperatureThreshold;
  uint16_t co2Threshold;
  uint8_t ammoniaThreshold;
  String wifiSSID;
  String wifiPassword;
  uint16_t squidID;
  uint16_t nodeID;
};

enum SensorState {
  READ_AMMONIA,
  READ_SOIL_MOISTURE,
  READ_TEMP_HUMIDITY,
  READ_SOIL_OXYGEN,
  READ_CO2,
  READ_SOIL_TEMP,
  END_CYCLE
};


// Define a structure to hold sensor data
struct sensorData {
  // Metadata
  uint16_t squidID;               // Squid ID, INT, 16-bit
  uint16_t nodeID;                // Node ID, INT, 32-bit
  uint32_t timestamp;             // UNIX Timestamp, unsigned int 16-bit
  
  // Sensor values
  uint16_t soilOxygen;            // Soil Oxygen, 0-25 VOL %, unsigned 16-bit int
  uint16_t CO2;                   // CO2, 0-10000 ppm, unsigned 16-bit int (max 10000, fits in 16 bits)
  uint16_t ammonia;               // Ammonia, 0-100 ppm, unsigned 16-bit int
  float soilMoisture;          // Soil Moisture, 0-99 %, unsigned 16-bit int
  
  // Temperatures using floats with 2 decimal places
  float soilTempProbe;            // Soil Temp Probe, -40 to 80 °C, float
  float soilTempProbe1;           // Soil Temp Probe 1, -50 to 105 °C, float
  float soilTempProbe2;           // Soil Temp Probe 2, -50 to 105 °C, float
  float soilTempProbe3;           // Soil Temp Probe 3, -50 to 105 °C, float
  float ambientTemp;              // Ambient Temp, -40 to 120 °C, float
  
  uint16_t ambientHumidity;       // Ambient Humidity, 0-99 %, unsigned 16-bit int
  float enclosureTemp;            // Enclosure Temp, -40 to 80 °C, float
};


// Header Pins in Squid 
// microUSB Pins
#define U0TXD 1
#define U0RXD 3

// SD Card Pins
#define HS2_DATA0 2
#define SD_CLK 6
#define SD_DATA0 7
#define SD_DATA1 8
#define SD_DATA2 9
#define SD_DATA3 10
#define SD_CMD 11
#define	SD_HS2_CLK	14
#define	SD_HS2_CMD	15

// Ethernet Pins
#define ETHERNET_XTAL1 0
#define ETHERNET_MDIO 18
#define ETHERNET_TXD0 19
#define ETHERNET_TX_EN 21
#define ETHERNET_TXD1 22
#define ETHERNET_MDC 23
#define ETHERNET_RXD0 25
#define ETHERNET_RXD1 26
#define ETHERNET_RX_CRS_DV 27

// // Analog Pins
// #define ADC_TEMP_PROBE_0 34  // [ADC Temp Probe 0]
// #define ADC_TEMP_PROBE_1 35  // [ADC Temp Probe 1]
// #define ADC_TEMP_PROBE_2 39  // [ADC Temp Probe 2] // disconnect IR

// Relay IOs (existing)
#define DRUM_ROTATION_RELAY 32  // [Drum Rotation Relay]
#define VENTILATION_CONTROL_RELAY 33  // [Vent & HVAC Control Relay]
#define CO2_PUMP 17             // PIN for controlling the CO2 pump
#define GREEN_LED  12
#define YELLOW_LED 17
 

// I2C Bus
#define SDA_I2C 13  // [SDA I2C]
#define SCL_I2C 16  // [SCL I2C]

// RS485 UART
#define DI_RS485 5// 4  // DI [RS485 Bus] // UART TX (RX on RS485)
#define RO_RS485 35 //36  // RO [RS485 Bus] // UART RX (TX on RS485)
#define DE_RE_RS485 12//5 // [RE/DE RS485] // Digital Output
 
// GPIO 35 CAN RX -> UART2 RX => [RS485 RO]
// GPIO 5 CAN TX => UART2 TX => [RS485 DI] 

 /*
+-------+-----------------+------------------------+------+----------------------+
|  Pin  | Configuration   | Remarks                | EXT1 | Squid Config         |
+-------+-----------------+------------------------+------+----------------------+
| GPIO0 | XTAL1/CLKIN     | Ethernet               |   1  | ETHERNET_XTAL1       |
| GPIO1 | U0TXD           |                        |   2  | U0TXD                |
| GPIO2 | HS2_DATA0       | UEXT / SD/MMC Card     |   3  | HS2_DATA0            |
| GPIO3 | U0RXD           |                        |   4  | U0RXD                |
| GPIO4 | U1TX            |                        |   5  | DI_RS485             |
| GPIO5 | CAN-TX          | CAN Driver             |   6  | DE_RE_RS485          |
| GPIO6 | SD_CLK          |                        |   7  | SD_CLK               |
| GPIO7 | SD_DATA0        |                        |   8  | SD_DATA0             |
| GPIO8 | SD_DATA1        |                        |   9  | SD_DATA1             |
| GPIO9 | SD_DATA2        |                        |  10  | SD_DATA2             |
| GPIO10| SD_DATA3        |                        |  11  | SD_DATA3             |
| GPIO11| SD_CMD          |                        |  12  | SD_CMD               |
| GPIO12| IR_Transmit     | Infrared Communication |  13  | GREEN_LED            |
| GPIO13| I2C-SDA         | UEXT                   |  14  | SDA_I2C              |
| GPIO14| HS2_CLK         | UEXT / SD/MMC Card     |  15  | SD_HS2_CLK           |
| GPIO15| HS2_CMD         | UEXT / SD/MMC Card     |  16  | SD_HS2_CMD           |
| GPIO16| I2C-SCL         | UEXT                   |  17  | SCL_I2C              |
| GPIO17| SPI_CS          | UEXT                   |  18  | CO2_PUMP             |
| GPIO18| RS485_TX        |                        |  19  | ETHERNET_MDIO        |
| GPIO19| RS485_RX        |                        |  20  | ETHERNET_TXD0        |
| GPIO20|                 |                        |      |                      |
| GPIO21| GREEN_LED       |                        |  21  | ETHERNET_TX_EN       |
| GPIO22| EMAC_TXD1(RMII) | Ethernet               |  22  | ETHERNET_TXD1        |
| GPIO23| MDC(RMII)       | Ethernet               |  23  | ETHERNET_MDC         |
| GPIO24|                 |                        |      |                      |
| GPIO25| MAC_RXD0(RMII)  | Ethernet               |  24  | ETHERNET_RXD0        |
| GPIO26| MAC_RXD1(RMII)  | Ethernet               |  25  | ETHERNET_RXD1        |
| GPIO27| EMAC_RX_CRS_DV  | Ethernet               |  26  | ETHERNET_RX_CRS_DV   |
| GPIO28|                 |                        |      |                      |
| GPIO29|                 |                        |      |                      |
| GPIO30|                 |                        |      |                      |
| GPIO31|                 |                        |      |                      |
| GPIO32| REL1            | Relays                 |  27  | DRUM_ROTATION_RELAY  |
| GPIO33| REL2            | Relays                 |  28  | VENTILATION_CONTROL_RELAY |
| GPI34 | BUT1            | Button                 |  29  |                      |
| GPI35 | CAN-RX          | CAN Driver             |  30  |                      |
| GPI36 | U1RXD           | UEXT                   |  31  | RO_RS485             |
| GPIO37|                 |                        |      |                      |
| GPIO38|                 |                        |      |                      |
| GPI39 | IR_RECEIVE      | Infrared Communication |  32  |                      |
+-------+-----------------+------------------------+------+----------------------+



 
 
pin number reference, link: https://github.com/Nuto/ESP32

*/ 
#endif // __CONFIG_H__

