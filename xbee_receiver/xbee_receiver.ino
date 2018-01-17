
#include <XBee.h>

// RECEIVER code

#define TRUE 1
#define FALSE 0
#define SRC_ADDRESS 0x0000
#define DEST_ADDRESS 0xFFFF

XBee xbee = XBee();

XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
Rx16Response rx16 = Rx16Response();

void setup() {

  Serial.begin(9600); //Baud rate must be the same as is on xBee module
  xbee.setSerial(Serial);

}


void loop() {
  
  uint8_t option = 0;
  uint8_t data = 0;
  char *msg;

    xbee.readPacket(); // does this send ack back?
    
    if (xbee.getResponse().isAvailable()) {
      // got something
 
        if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
          // got RX packet with 16 bit address
                xbee.getResponse().getRx16Response(rx16);
                
                // read the whole message
                for (int i = 0; i < rx16.getDataLength(); i++) {
                  data = rx16.getData(i);
                  printf("%i", data);
                  msg[i] = (char)data;
                }
                
                // validate msg
                // check if starts with <
                // check if ends with >
 
                
                /*
        	option = rx16.getOption(); // TODO whats this for?
        	data = rx16.getData(0); // TODO is this all data or do we loop?
                msg = (char *)data;
                */
 
        }
        else {
          printf("incoming data is not from a 16 bit address");
        }
        
    } else if (xbee.getResponse().isError()) {
        // when does this occur? will an ack be sent back, if so, we would need to send transmitter a NACK
    } 
  

}

