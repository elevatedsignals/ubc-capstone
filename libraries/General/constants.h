#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TRUE 1
#define FALSE 0

#define MODULE_ID 12345
#define PIN_CO2 A0
#define PIN_DHT 2
#define PIN_SD 10
#define PIN_SD_CHECK 9

#define DHTTYPE DHT22

#define TXT_FILE "data.txt"
#define TX16_REQUEST_MAX_SIZE 109
#define SRC_ADDRESS "0xCE06" // for informational purposes only
#define DEST_ADDRESS 0xAB7F
#define PACKET_SIZE 100 // for xbee data

#define ERROR_TEMP "Error: DHT Temperature sensor"
#define ERROR_HUMIDITY "Error: DHT Humidity sensor"
#define ERROR_GCO2V "Error: get_co2_voltage output outside expected range (400 - 2000)"
#define ERROR_GCO2C "Error: get_co2_concentration input/output outside expected range (400 - 2000/0 - 5000)"
#define ERROR_NOSD "Error: No SD Card Inserted"
#define ERROR_INITSD "Error: Failed to initialize SD Card"

#endif
