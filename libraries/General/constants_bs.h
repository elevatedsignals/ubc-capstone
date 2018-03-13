#ifndef CONSTANTS_BS_H
#define CONSTANTS_BS_H

#define TRUE 1
#define FALSE 0

#define MODULE_ID 12345

#define INIT_ETHERNET_DELAY 1000
#define DEFAULT_HTTP_PORT 80

#define ID_TEMP 58
#define ID_HUM 43
#define ID_CO2 13
#define ID_PAR 29
#define ID_AF 1

#define HTTP_GET_REQUEST_L1 "POST /api/web/sensor/bd16506f5a4d789465610a8806eec8b2/reading/add HTTP/1.0"
#define HTTP_GET_REQUEST_L2 "Host: https://dashboard.elevatedsignals.com"
#define HTTP_GET_REQUEST_L3 "Content-Type: application/json"
#define HTTP_GET_REQUEST_L4 "Content-Length: "
#define HTTP_GET_REQUEST_L5 ""

#define ERROR_HTTP_REQUEST "Error: Failed to make HTTP request"
#define ERROR_RECV_BUFFER "Error: Failed to receive any char/s from server"
#define ERROR_CONNECTION "Error: Failed to maintain connection to server"
#define ERROR_PREP_PAYLOAD "Error: Failed to properly format sensor data for upload"

#endif
