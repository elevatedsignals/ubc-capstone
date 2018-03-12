
#include "XBee.h"
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <TimeLib.h>
#include "constants.h"
#include "timestamp.h"

// RECEIVER/Base station code
// XBEE channel = C, pan id = F5D9

#define DEST_ADDRESS 0xCE06
#define PACKET_SIZE 80

XBee xbee = XBee();
// track whether there is existing data on SD
volatile int commFailureOccured = FALSE;

void setup() {

  Serial.begin(9600);

  xbee.setSerial(Serial);
  initialize_UDP();

  // get time from NTP server over UDP
  send_NTP_packet(timeServer);
  // wait to see if a reply is available
  delay(DELAY_TIME);
  // Set the time accounting for above delay
  setTime(parse_NTP_packet() + ((DELAY_TIME) / 1000));

}

void loop() {
  // wait for request from Sensor modules
  requestHandler();
}

/* Handles requests coming in from sensor modules via XBEE */
void requestHandler() {
  
  uint8_t data = 0;
  Rx16Response rx16 = Rx16Response();
  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      // got RX packet with 16 bit address
      xbee.getResponse().getRx16Response(rx16);

      char msg[PACKET_SIZE] = "";

      int i;
      // read the whole message
      for (i = 0; i < rx16.getDataLength(), i < 100; i++) {
        data = rx16.getData(i);
        msg[i] = data;
      }
      msg[i] = '\0';

      int type = requestParser(msg);

      if (type == 0) { // contains sensor data
        // TODO send to server

        // TODO if fail, set commFailureOccured, store on SD
        // TODO if success, check commFailureOccured
        // TODO if occured, resend old data
        /*
        int uploadExistingData() { 
          // return true for all success, false for even a single failure
          // line by line, erase lines successfully sent from SD
          // keep lines that didnt send  
          }
               
        */

        // if uploadExisting == true, commFailureOccured = FALSE
  
      } else if (type == 1) { // request for timestamp
        
        uint16_t srcAddr = rx16.getRemoteAddress16(); // get clients addr so we can respond

        String formattedTime = get_formatted_time();
        Ethernet.maintain(); // TODO, needed? we shouldnt need to contact NTP server again
        char * time = const_cast < char * > (formattedTime.c_str());
        respondToClient(time, srcAddr);
      }
    }
  }
}

/* Determine what kind of request msg is 
* returns -1 if not a valid msg
* returns 0 if request to send sensor data to server
* returns 1 if request for current time
*/
int requestParser(char * msg) {

  // this is a valid msg to handle
  if (strstr(msg, "{") && strstr(msg, "}")) {

    if (strstr(msg, "timestamp") != NULL) {
      // this message contains sensor data
      Serial.println("Receieved a msg containing sensor data");
      Serial.println(msg);
      return 0;
    }
    if (strstr(msg, "request time")) {
      Serial.println("Receieved a request for timestamp");
      return 1;
    }
  }

  return -1;
}

/* 
* Responds to client with timestamp 
*/
int respondToClient(char * time, uint16_t srcAddr) {

  char response[PACKET_SIZE] = "";
  strcpy(response, "{ time response, current time=");
  strcat(response, time);
  strcat(response, "}");

  sendXbee(response, srcAddr); // dont check for failure, let client retry
  Serial.println("Sending time back to client");

  return TRUE;
}

/*
* sends msg to client at addr over Xbee 
*/
int sendXbee(char * msg, uint16_t addr) {
  Tx16Request tx = Tx16Request(addr, (uint8_t * ) msg, strlen(msg));
  xbee.send(tx);
}
