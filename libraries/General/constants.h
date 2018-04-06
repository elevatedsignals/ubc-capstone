#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TRUE 1
#define FALSE 0

#define MODULE_ID 12345
#define PIN_CO2 A0
#define PIN_DHT 2
#define PIN_PAR A3
#define PIN_SD 10
#define PIN_SD_CHECK 9

#define DHTTYPE DHT22

#define TXT_FILE "data.txt"

#define ERROR_TEMP "Error: DHT Temperature sensor"
#define ERROR_HUMIDITY "Error: DHT Humidity sensor"
#define ERROR_GCO2V "get_co2_voltage error: output outside expected range (400 - 2000)\n"
#define ERROR_GCO2C "get_co2_concentration error: input/output outside expected range (400 - 2000/0 - 5000)\n"
#define ERROR_NOSD "Error: No SD Card Inserted"
#define ERROR_INITSD "Error: Failed to initialize SD Card"
#define ERROR_GPARV "get_par_voltage error: output outside expected range (0 - 40)\n"
#define ERROR_GPARC "get_par_concentration error: input/output outside expected range (0 - 40/0 - 4000)\n"

#endif
