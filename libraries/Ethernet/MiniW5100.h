#ifndef MINIW5100_H
#define MINIW5100_H

#include "constants_bs.h"
#include "constants.h"
#include <Ethernet.h>

const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// note that 'http/https' part of url is NOT used
const char middleman_server[] = "ec2-52-202-121-17.compute-1.amazonaws.com";
const IPAddress ip(192, 168, 0, 177); // static IP address to use if the DHCP fails to assign

/*
*  Purpose: Initializes the ethernet connection to the server for logging sensor data
*/
EthernetClient init_ethernet(EthernetClient client) {

  Ethernet.select(ETHERNET_PIN);

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to obtain an IP address using DHCP. Attempting to set IP address manually.");
    Ethernet.begin(mac, ip); // try to set IP address manually
  }

  client.setTimeout(500);
  delay(DELAY_TIME); // delay for Mini W5100 to start

  return client;
}

 /*
 *  Purpose: Connects to the server, uploads sensor data, and validates that the
 */
EthernetClient make_http_request(EthernetClient client, char* payload, int *error) {
  client.stop(); // close any previous connections to free socket

  if(client.connect(middleman_server, MIDDLEMAN_PORT) == 1) {

  client.println(HTTP_L1);
  client.println("Host: ec2-52-202-121-17.compute-1.amazonaws.com");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(strlen(payload));
  client.println("Content-Type: application/json");
  client.println();
  client.println(payload);

  } else {
    Serial.println(ERROR_SERVER_DISCONNECTED);
    *error = TRUE;
    return client;
  }

  // wait max 3 seconds for response
  int attempt = 0;
  while(!client.available() && attempt < 4) {
    delay(DELAY_TIME);
    attempt++;

    if (attempt == 4) {
      *error = TRUE;
      Serial.println(ERROR_NO_RESPONSE);
      return client;
    }
  }

  // read response
  char response[HTTP_STATUS_LEN+1];
  int i;
  for (i = 0; i < HTTP_STATUS_LEN; i++) {
    char c = client.read();
    Serial.print(c);
    response[i] = c;
  }
  response[i] = '\0';

  // check if response is successful
  if (strstr(response, HTTP_SUCCESS) == NULL) {
    Serial.println(ERROR_FAILED_DATA_UPLOAD);
    *error = TRUE;
  }

  return client;
}

#endif
