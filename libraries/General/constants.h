#ifndef CONSTANTS_H
#define CONSTANTS_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define TRUE 1
#define FALSE 0

#define MODULE_ID 12345

// PIN USAGE
#define PIN_CO2 A0
#define PIN_AIRFLOW_TMP A2
#define PIN_AIRFLOW_RV A3
#define PIN_PAR A6
#define PIN_DHT 2
#define PIN_SD 10
#define PIN_SD_CHECK 8
#define RX_PIN 3

#define DHTTYPE DHT22

// Capability ID's
#define ID_TEMP 58
#define ID_HUM 43
#define ID_CO2 13
#define ID_PAR 29
#define ID_AF 1

#define TXT_FILE "data.txt"

#define EIGHT_HOURS 28800
#define DELAY_TIME 1000
#define ONE_MINUTE 60000000

#define WIND_THRESHOLD 4

#define PACKET_SIZE 100 // for xbee data
#define SRC_ADDRESS "0xCE06" // for informational purposes only
#define DEST_ADDRESS 0xAB7F

/* ERROR MSGS */
#define ERROR_TEMP "Error: DHT Temperature Sensor"
#define ERROR_HUMIDITY "Error: DHT Humidity Sensor"
#define ERROR_GCO2V "Error: CO2 Sensor Voltage"
#define ERROR_GCO2C "Error: CO2 Sensor Concentration"
#define ERROR_PARV "Error: PAR Sensor Voltage"
#define ERROR_PARI "Error: PAR Sensor Intensity"
#define ERROR_AIRFLOW "Error: Airflow Sensor"
#define ERROR_NOSD "Error: No SD Card Inserted"
#define ERROR_INITSD "Error: Failed to initialize SD Card"
#define ERROR_INITUDP "Error: Failed to configure Ethernet using DHCP"

#endif
