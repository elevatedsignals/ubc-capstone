#ifndef CONSTANTS_BS_H
#define CONSTANTS_BS_H

#define TRUE 1
#define FALSE 0

#define PIN_SD 10
#define PIN_SD_CHECK 9

#define MODULE_ID 12345
#define DEFAULT_HTTP_PORT 80

#define INIT_ETHERNET_DELAY 1000
#define RESPONSE_DELAY 1000

#define ID_TEMP 58
#define ID_HUM 43
#define ID_CO2 13
#define ID_PAR 29
#define ID_AF 1

#define HTTP_POST_REQUEST_L1 "POST /api/web/sensor/bd16506f5a4d789465610a8806eec8b2/reading/add HTTP/1.0"
#define HTTP_POST_REQUEST_L2 "Host: https://dashboard.elevatedsignals.com"
#define HTTP_POST_REQUEST_L3 "Content-Type: application/json"
#define HTTP_POST_REQUEST_L4 "Content-Length: "
#define HTTP_POST_REQUEST_L5 ""

#define HTTP_POST_RESPONSE_1L_MAX_CHARS 16 // maximum number of chars to read from first line of HTTP POST response
#define HTTP_POST_RESPONSE_SUCCESS "HTTP/1.0 200 OK "
#define TXT_FILE "data.txt"
#define TX16_REQUEST_MAX_SIZE 30

#define ERROR_SERVER_DISCONNECTED "Error: Server connection failed."
#define ERROR_SERVER_UNAVAILABLE "Error: Server unavailable for response."
#define ERROR_FAILED_DATA_UPLOAD "Error: Server did not receive data successfully."
#define ERROR_PREP_PAYLOAD "Error: Failed to properly format sensor data for upload."
#define ERROR_NOSD "Error: No SD Card Inserted"
#define ERROR_INITSD "Error: Failed to initialize SD Card"

#endif
