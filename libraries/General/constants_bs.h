#ifndef CONSTANTS_BS_H
#define CONSTANTS_BS_H

#define TRUE 1
#define FALSE 0

#define MODULE_ID 12345

#define SERVER "www.google.com"
#define INIT_ETHERNET_DELAY 1000
#define DEFAULT_HTTP_PORT 80

// #define API_KEY_TEMP
// #define API_KEY_HUM
// #define API_KEY_CO2
// #define API_KEY_AF

#define HTTP_GET_REQUEST_L1 "POST /PATH_HERE HTTP/1.0"
#define HTTP_GET_REQUEST_L2 "Host: HOST_URL_HERE"
#define HTTP_GET_REQUEST_L3 "Content-Type: application/json"
#define HTTP_GET_REQUEST_L4 "Content-Length: "
#define HTTP_GET_REQUEST_L5 ""

#define ERROR_HTTP_REQUEST "Error: Failed to make HTTP request"
#define ERROR_RECV_BUFFER "Error: Failed to receive any char/s from server"
#define ERROR_CONNECTION "Error: Failed to maintain connection to server"
#define ERROR_PREP_PAYLOAD "Error: Failed to properly format sensor data for upload"

#endif
