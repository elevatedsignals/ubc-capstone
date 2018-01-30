#ifndef MINIW5100_H
#define MINIW5100_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <SPI.h>
#include <Ethernet.h>

const unsigned long interval = 10L * 1000L; // delay between sending sensor data (milliseconds)
const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
const char server[] = "www.arduino.cc"; // name address for Arduino (using DNS)
const IPAddress ip(192, 168, 1, 177); // static IP address to use if the DHCP fails to assign

/*
* Purpose: Initializes the ethernet connection to the server for logging sensor data
*/
EthernetClient init_ethernet(EthernetClient client, int *error) {
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB port only
  // }

  // start the ethernet connection
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Error: Failed to obtain an IP address using DHCP. Attempting to set IP address manually.");

    Ethernet.begin(mac, ip); // try to set IP address manually
  }

   delay(INIT_ETHERNET_DELAY); // delay for Mini W5100 to start

   return client;
 }

 /*
 * Purpose: Connects to the server and makes a HTTP request to send sensor data
 */
EthernetClient make_http_request(EthernetClient client, int *error) {
  client.stop(); // close any previous connections to free socket

  if (client.connect(server, DEFAULT_HTTP_PORT)) {

    // Make HTTP request
    client.println("GET /search?q=arduino HTTP/1.1"); //REPLACE TEST VALUE
    client.println("Host: www.google.com"); //REPLACE TEST VALUE
    client.println("Connection: close"); //REPLACE TEST VALUE
    client.println(); //REPLACE TEST VALUE
  } else {
    Serial.println("Error: Failed to establish connection with server.");
    *error = TRUE;
  }

  return client;
}

/*
* Purpose: Prints the ethernet module's IP address over serial
*/
void print_ip() {
  Serial.print("The Ethernet Module has the following IP address: ");
  Serial.println(Ethernet.localIP());
}

/*
* Purpose: Prints any char/s received from the server for debugging purposes
*          over serial
*/
EthernetClient check_recv_buffer(EthernetClient client, int *error) {
  int num_bytes_avail = client.available();

  // NOT SURE IF NECESSARY
  if(num_bytes_avail == 0) {
    *error = TRUE;
    Serial.println("Error: No char/s received from server.");
    return client;
  }


  while(num_bytes_avail >= 1) {
    char c = client.read();

    if(c == -1) {
      break; //stop reading if no chars are left
    }

    Serial.write(c);
    num_bytes_avail--;
  }

  // ALTERNATIVE CODE IF ONLY SINGLE CHARS ARE EXPECTED
  // if (client.available()) {
  //   char c = client.read();
  //   Serial.write(c);
  // }

  return client;
}

/*
* Purpose: Stops running client if disconnected from server
*/
EthernetClient check_connection(EthernetClient client, int *error) {
  if (!client.connected()) {
    Serial.println("Error: Disconnected from server. Closing connection.");
    int *error = TRUE;
    client.stop();
  } else {
    Serial.println("Connection to server maintained.");
  }

  return client;
}

/*
* Purpose: Renews DHCP lease if needed
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
