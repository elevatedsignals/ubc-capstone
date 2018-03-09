#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TRUE 1
#define FALSE 0

#define MODULE_ID 12345
#define PIN_CO2 A0
#define PIN_AIRFLOW_TMP A1
#define PIN_AIRFLOW_RV A2
#define PIN_DHT 2
#define PIN_SD 10
#define PIN_SD_CHECK 9

#define DHTTYPE DHT22

#define TXT_FILE "data.txt"
#define EIGHT_HOURS 28800
#define DELAY_TIME 1000

#define WIND_THRESHOLD 4

#define ERROR_TEMP "Error: DHT Temperature Sensor"
#define ERROR_HUMIDITY "Error: DHT Humidity Sensor"
//#define ERROR_GCO2V "get_co2_voltage error: output outside expected range (400 - 2000)"
//#define ERROR_GCO2C "get_co2_concentration error: input/output outside expected range (400 - 2000/0 - 5000)"
#define ERROR_GCO2V "Error: CO2 Sensor Voltage"
#define ERROR_GCO2C "Error: CO2 Sensor Concentration"
#define ERROR_AIRFLOW "Error: Airflow Sensor"
#define ERROR_NOSD "Error: No SD Card Inserted"
#define ERROR_INITSD "Error: Failed to initialize SD Card"
#define ERROR_INITUDP "Error: Failed to configure Ethernet using DHCP"

#endif