#include "Constants_Base.h"
#include "Constants.h"
#include "Format.h"
#include "MiniW5100.h"
//#include "SDCard_Base.h"
#include "XBee.h"
#include "SendXBee.h"

#include <TimeLib.h>
#include "Timestamp.h"


EthernetClient client;
XBee xbee = XBee();
// track whether data has queued in SD storage
//volatile int commFailureOccurred = FALSE;
//bool SDerror = FALSE;

void setup() {

  Serial.begin(9600);
  xbee.setSerial(Serial);

  /* SD interfacing code */
//   struct SD_card sd = init_sd(TXT_FILE, &SDerror); // TODO we shouldnt initialize everytime when we refactor this



  // get time from NTP server over UDP
//client = init_ethernet(client);
initialize_UDP();
send_NTP_packet(timeServer);
// set the time accounting for delay
setTime(parse_NTP_packet() + ((DELAY_TIME) / 1000));
String clockTime = get_formatted_time();
char *timestamp = const_cast < char * > (clockTime.c_str());

Serial.println(F("Timestamp obtained from NTP server is:"));
Serial.println(timestamp);


  client = init_ethernet(client);

}

void loop() {

 requestHandler();

}



void upload(char *payload, EthernetClient client) {

 bool error = FALSE;
     client = make_http_request(client, payload, &error);
     if(error) {
       Serial.println(F("Failed to make HTTP request."));
       // TODO STORE ON SD
       //if(!SDerror){
       //     write_base_station_message(sd, payload, &SDerror);
       //     commFailureOccurred = TRUE;
       //   }
     }
     else {
      Serial.println(F("Successfully made HTTP request."));

      // Check for queued messages
      //if(!SDerror){
      //     if(commFailureOccurred){
      //       commFailureOccurred = recover_base_station_data(&sd, client);
      //     }
      // }
     }

}


/*
* Handles requests coming in from sensor modules via XBee
*/
void requestHandler() {

  uint8_t data = 0;
  Rx16Response rx16 = Rx16Response();
  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {

    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      // got RX packet with 16 bit address
      xbee.getResponse().getRx16Response(rx16);

      char msg[PACKET_SIZE] = "";

      int8_t i;
      // read the whole message
      for (i = 0; i < rx16.getDataLength(), i < 100; i++) {
        data = rx16.getData(i);
        msg[i] = data;
      }
      msg[i] = '\0';

      int8_t type = requestParser(msg);

      if (type == 0) { // contains sensor data
        // TODO send to server
        upload(msg, client);
      } else if (type == 1) { // request for timestamp

        /* TODO uncomment for timestamp
        uint16_t srcAddr = rx16.getRemoteAddress16(); // get clients addr so we can respond

        String formattedTime = get_formatted_time();
        Ethernet.maintain(); // TODO, needed? we shouldnt need to contact NTP server again
        char * time = const_cast < char * > (formattedTime.c_str());
        respondToClient(time, srcAddr);
        */
      }
    }
  }
}

/*
* Determine what kind of request msg is
* Returns -1 if not a valid msg
* Returns 0 if request to send sensor data to server
* Returns 1 if request for current time
*/
int8_t requestParser(char * msg) {

  // this is a valid msg to handle
  if (strstr(msg, "{") && strstr(msg, "}")) {

    if (strstr(msg, "capability_id") != NULL) {
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
* Purpose: Responds to client with srcAddr with timestamp
* Output: TRUE when completed
*/
bool respondToClient(char * time, uint16_t srcAddr) {

  char response[PACKET_SIZE] = "";
  // add obtained time to response
  strcpy(response, "{ time response, current time=");
  strcat(response, time);
  strcat(response, "}");

  // dont check for failure, let client retry
  sendXbeeNoVerify(response, srcAddr, xbee);
  Serial.println("Sending time back to client");

  return TRUE;
}
