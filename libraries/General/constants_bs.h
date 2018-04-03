#ifndef CONSTANTS_BS_H
#define CONSTANTS_BS_H

#define TRUE 1
#define FALSE 0

#define PIN_SD 10
#define PIN_SD_CHECK 9

#define MODULE_ID 12345
#define MIDDLEMAN_PORT 8080

#define ONE_SEC 1000

#define ID_TEMP 58
#define ID_HUM 43
#define ID_CO2 13
#define ID_PAR 29
#define ID_AF 1

#define HTTP_L1 "POST /api/web/sensor/bd16506f5a4d789465610a8806eec8b2/reading/add HTTP/1.1" // TODO dont post api key in public place
#define HTTP_STATUS_LEN 12 // maximum number of chars to read from first line of HTTP POST response
#define TXT_FILE "data.txt"
#define TX16_REQUEST_MAX_SIZE 30

#define ERROR_SERVER_DISCONNECTED "Error: Server connection failed."
#define ERROR_FAILED_DATA_UPLOAD "Error: Server did not receive data successfully."
#define ERROR_PREP_PAYLOAD "Error: Failed to properly format sensor data for upload."
#define ERROR_NOSD "Error: No SD Card Inserted"
#define ERROR_INITSD "Error: Failed to initialize SD Card"
#define ERROR_NO_RESPONSE "Error: No response from server."

#endif
