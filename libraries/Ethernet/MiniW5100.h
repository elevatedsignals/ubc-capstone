#ifndef MINIW5100_H
#define MINIW5100_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "constants_bs.h"
#include <stdio.h>
#include <stdlib.h>
#include <SPI.h>
#include <Ethernet.h>

const unsigned long interval = 10L * 1000L; // delay between sending sensor data (milliseconds)
const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// note that 'http/https' part of url is NOT used
const char server[] = "dashboard.elevatedsignals.com";
//const char server[] = "ec2-52-202-121-17.compute-1.amazonaws.com";
const IPAddress ip(192, 168, 0, 177); // static IP address to use if the DHCP fails to assign

/*
*  Purpose: Initializes the ethernet connection to the server for logging sensor data
*/
EthernetClient init_ethernet(EthernetClient client) {
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB port only
  // }

  // start the ethernet connection
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to obtain an IP address using DHCP. Attempting to set IP address manually.");
    Ethernet.begin(mac, ip); // try to set IP address manually
  }

  client.setTimeout(500);
  delay(INIT_ETHERNET_DELAY); // delay for Mini W5100 to start

  return client;
}

/*
*  Purpose: Formats sensor data for upload
*/
char* prepare_payload(int cap_id, float val, char *time_string, int *error) {
  if(cap_id == NULL) {
    Serial.println(ERROR_PREP_PAYLOAD);
    *error = TRUE;
    return NULL;
  }

  char payload[100] = "";

  switch(cap_id) {
    case 58: { // ID_TEMP
      char array[6];
      dtostrf(val, 1, 1, array);
      strcpy(payload, "{\"capability_id\": 58,\"json_value\":{\"temperature\":");
      strcat(payload, array);
      strcat(payload, "}");
      break;
    }

    case 43: { // ID_HUM
      char array[6];
      dtostrf(val, 1, 1, array);
      strcpy(payload, "{\"capability_id\": 43,\"json_value\":{\"humidity\":");
      strcat(payload, array);
      strcat(payload, "}");
      break;
    }

    case 13: { // ID_CO2
      char array[6];
      dtostrf(val, 1, 1, array);
      strcpy(payload, "{\"capability_id\": 13,\"json_value\":{\"carbonDioxide\":");
      strcat(payload, array);
      strcat(payload, "}");
      break;
    }

    case 29: { // ID_PAR
      char array[6];
      dtostrf(val, 1, 1, array);
      strcpy(payload, "{\"capability_id\": 29,\"json_value\":{\"value\":");
      strcat(payload, array);
      strcat(payload, "}");
      break;
    }
    case 1: { // ID_AF
      strcpy(payload, "{\"capability_id\": 1,\"json_value\":{\"state\":");

      if(val == 0.0) {
        char *array = "\"still\"";
        strcat(payload, array);
      } else {
        char *array = "\"moving\"";
        strcat(payload, array);
      }

      strcat(payload, "}");
      break;
    }
    default: {
      Serial.println(ERROR_PREP_PAYLOAD);
      *error = TRUE;
      return NULL;
    }
  }


  if(time_string != NULL) {
    strcat(payload, "\"timestamp\": \"");
    strcat(payload, time_string);
    strcat(payload, "\"");
  }

  strcat(payload, "}");

  return payload;
}

 /*
 *  Purpose: Connects to the server, uploads sensor data, and validates that the
 *
 */
EthernetClient make_http_request(EthernetClient client, char* payload, int *error) {
  client.stop(); // close any previous connections to free socket

  if(client.connect("ec2-52-202-121-17.compute-1.amazonaws.com", 8080) == 1) {


    char *data = "{\"capability_id\": 58, \"json_value\": { \"temperature\": 28.0}}";
    Serial.println(data);

/*
    client.println("GET / HTTP/1.1");
    client.println("Host: dashboard.elevatedsignals.com");
    client.println("Connection: close");
    client.println();*/

  client.println("POST /api/web/sensor/bd16506f5a4d789465610a8806eec8b2/reading/add HTTP/1.1");
  client.println("Host: ec2-52-202-121-17.compute-1.amazonaws.com");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(strlen(data));
  client.println("Content-Type: application/json");
  client.println();
  client.println(data);

/*
    client.println(HTTP_POST_REQUEST_L1);
    client.println(HTTP_POST_REQUEST_L2);
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println(HTTP_POST_REQUEST_L3);
    client.print(HTTP_POST_REQUEST_L4);
    client.println(strlen(data));
    client.println();
    client.println(data);*/
  } else {
    Serial.println(ERROR_SERVER_DISCONNECTED);
    *error = TRUE;
    return client;
  }

  delay(RESPONSE_DELAY);

  // wait max 3 seconds for response
  int attempt = 0;
  while(!client.available() && attempt < 4) {
    delay(RESPONSE_DELAY);
    attempt++;

    if (attempt == 4) {
      *error = TRUE;
      Serial.println(ERROR_NO_RESPONSE);
      return client;
    }
  }


/*
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
  if (strstr(response, HTTP_RESPONSE_SUCCESS) == NULL) {
    Serial.println(ERROR_FAILED_DATA_UPLOAD);
    *error = TRUE;
  }*/

  while(client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  return client;
}

/*
*  Purpose: Prints the ethernet module's IP address over serial
*/
void print_ip() {
  Serial.print(F("The Ethernet Module has the following IP address: "));
  Serial.println(Ethernet.localIP());
}

/*
*  Purpose: Stops running client if disconnected from server
*/
EthernetClient check_connection(EthernetClient client, int *error) {
  if (!client.connected()) {
    Serial.println(ERROR_SERVER_DISCONNECTED);
    *error = TRUE;
    client.stop();
  } else {
    Serial.println(F("Connection to server maintained."));
  }

  return client;
}

/*
*  Purpose: Renews DHCP lease if needed
*/
void validate_ip() {
  int status = Ethernet.maintain();

  switch(status) {
    case 0: {
      Serial.println(F("IP renewal unnecessary at the moment."));
      break;
    }
    case 1: {
      Serial.println(F("IP renewal failed."));
      break;
    }
    case 2: {
      Serial.println(F("IP renewal successful."));
      break;
    }
    case 3: {
      Serial.println(F("IP rebinding failed."));
      break;
    }
    case 4: {
      Serial.println(F("IP rebinding successful"));
      break;
    }
  }
}

#endif
