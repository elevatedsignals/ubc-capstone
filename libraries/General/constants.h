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

#define SERVER "www.google.com"
#define INIT_ETHERNET_DELAY 1000
#define DEFAULT_HTTP_PORT 80

#define TXT_FILE "data.txt"

#define ERROR_TEMP "Error: DHT Temperature sensor"
#define ERROR_HUMIDITY "Error: DHT Humidity sensor"
#define ERROR_GCO2V "Error: get_co2_voltage output outside expected range (400 - 2000)"
#define ERROR_GCO2C "Error: get_co2_concentration input/output outside expected range (400 - 2000/0 - 5000)"
#define ERROR_NOSD "Error: No SD Card Inserted"
#define ERROR_INITSD "Error: Failed to initialize SD Card"
#define ERROR_HTTP_REQUEST "Error: Failed to make HTTP request"
#define ERROR_RECV_BUFFER "Error: Failed to receive any char/s from server"
#define ERROR_CONNECTION "Error: Failed to maintain connection to server"


#endif
