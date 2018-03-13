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
const char server[] = "https://dashboard.elevatedsignals.com"; // REPLACE WITH SERVER URL
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
 *  Purpose: Prints any char/s received from the server for debugging purposes
 *          over serial
 */
EthernetClient check_recv_buffer(EthernetClient client, int *error) {
   while(client.available()) {
     char c = client.read();
     Serial.write(c);
   }

   // IF ONLY A SINGLE CHARS IS EXPECTED
   // if(client.available()) {
   //   char c = client.read();
   //   Serial.write(c);
   // }

   return client;
}

/*
*  Purpose: Formats sensor data for upload
*/
char* prepare_payload(String *payload, int cap_id, float val, String *time_string) {

  switch(cap_id) {
    case 58: { // ID_TEMP
      *payload = "{\"capability_id\": 58";
      *payload += ",\"json_value\":{";
      *payload += "\"temperature\":"+(String)val;
      break;
    }
    case 43: { // ID_HUM
      *payload = "{\"capability_id\": 43";
      *payload += ",\"json_value\":{";
      *payload += "\"Humidity\":"+(String)val;
      break;
    }
    case 13: { // ID_CO2
      *payload = "{\"capability_id\": 13";
      *payload += ",\"json_value\":{";
      *payload += "\"carbonDioxide\":"+(String)val;
      break;
    }
    case 29: { // ID_PAR
      *payload = "{\"capability_id\": 29";
      *payload += ",\"json_value\":{";
      *payload += "\"value\":"+(String)val;
      break;
    }
    case 1: { // ID_AF
      *payload = "{\"capability_id\": 1";
      *payload += ",\"json_value\":{";
      *payload += "\"state\":";

      if(val == 0.0) {
        *payload += "\"still\"";
      } else {
        *payload += "\"moving\"";
      }

      break;
    }
  }

  *payload += "}";

  if(time_string != NULL) {
    *payload += "\"timestamp\": \"";
    *payload += *time_string;
    *payload += "\"";
  }

  *payload += "}";

  return (char *)payload;
}

 /*
 *  Purpose: Connects to the server and makes a HTTP request to send sensor data
 */
EthernetClient make_http_request(EthernetClient client, String payload, int *error) {
  client.stop(); // close any previous connections to free socket

  Serial.println("Connecting to google.com on Port 80 (Default).");
  if (client.connect(server, DEFAULT_HTTP_PORT)) { // REPLACE TEST VALUE

    // Test an HTTP GET request
    // Serial.println("Sending 1st line: \"GET /search?q=arduino HTTP/1.1\"");
    // client.println("GET /search?q=arduino HTTP/1.1"); // REPLACE TEST VALUE
    // Serial.println("Sending 2nd line: \"Host: www.google.com\"");
    // client.println("Host: www.google.com"); // REPLACE TEST VALUE
    // Serial.println("Sending 3rd line: \"Connection: close\"");
    // client.println("Connection: close"); // REPLACE TEST VALUE
    // Serial.println("Sending 4th line: \"\"");
    // client.println(); // REPLACE TEST VALUE

    client.println(HTTP_GET_REQUEST_L1);
    client.println(HTTP_GET_REQUEST_L2);
    client.println(HTTP_GET_REQUEST_L3);
    client.print(HTTP_GET_REQUEST_L4);
    client.println(payload.length());
    client.println(HTTP_GET_REQUEST_L5);
    client.println(payload);
  } else {
    Serial.println("Error: Failed to establish connection with server.");
    *error = TRUE;
  }

  return client;
}

/*
*  Purpose: Sends a sensor value to the server using an HTTP 1.0 POST request
*/
EthernetClient send_to_server(EthernetClient client, String *payload, int cap_id, float val, String time_string, int *error) {
  if(time_string == NULL) {
    *payload = prepare_payload(payload, cap_id, val, NULL);
  } else {
    *payload = prepare_payload(payload, cap_id, val, &time_string);
  }

  // client = make_http_request(client, *payload, &error); // @TODO: Fix passing of payload and error

  return client;
}

/*
*  Purpose: Prints the ethernet module's IP address over serial
*/
void print_ip() {
  Serial.print("The Ethernet Module has the following IP address: ");
  Serial.println(Ethernet.localIP());
}

/*
*  Purpose: Stops running client if disconnected from server
*/
EthernetClient check_connection(EthernetClient client, int *error) {
  if (!client.connected()) {
    Serial.println("Error: Disconnected from server. Closing connection.");
    *error = TRUE;
    client.stop();
  } else {
    Serial.println("Connection to server maintained.");
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
      Serial.println("IP renewal unnecessary at the moment.");
      break;
    }
    case 1: {
      Serial.println("IP renewal failed.");
      break;
    }
    case 2: {
      Serial.println("IP renewal successful.");
      break;
    }
    case 3: {
      Serial.println("IP rebinding failed.");
      break;
    }
    case 4: {
      Serial.println("IP rebinding successful");
      break;
    }
  }
}

#endif
