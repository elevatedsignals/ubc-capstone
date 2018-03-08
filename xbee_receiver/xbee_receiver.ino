
#include <XBee.h>

// RECEIVER code

#define TRUE 1
#define FALSE 0
#define DEST_ADDRESS 0xCE06
#define PACKET_SIZE 80

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();

volatile int commFailureOccured = FALSE;

void setup() {

    Serial.begin(9600);
    xbee.setSerial(Serial);
}

void loop() {
  
  requestHandler();

}

/* Handles requests coming in from sensor modules via XBEE */
void requestHandler() {
  
    uint8_t data = 0;

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
            
            if (type == 0) {
              // TODO send to server
                    
                    // TODO if success, check commFailureOccured
                    // TODO if occured, resend old data
                    // TODO if fail, set commFailureOccured, store SD
            }
            else if (type == 1) {
              // get clients address
                    uint16_t srcAddr = rx16.getRemoteAddress16();

                    // TODO get time from NTP
                    String formattedTime = "1pm";
                    char *time = const_cast<char*>(formattedTime.c_str());
                    respondToClient(time, srcAddr);

            }

        }

    }
  
}

/* Determine what kind of request msg is */
// returns -1 if not a valid msg
// returns 0 if request to send sensor data to server
// returns 1 if request for current time
int requestParser(char *msg) {

            // this is a valid msg to handle
            if (strstr(msg, "{") && strstr(msg, "}")) {

                // request for timestamp or message with sensor data?
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


/* Responds to client with timestamp */
int respondToClient(char *time, uint16_t srcAddr) {

    char response[PACKET_SIZE] = "";

    strcpy(response, "{ time response, current time=");
    strcat(response, time);
    strcat(response, "}");

    sendXbee(response, srcAddr);
    Serial.println("Sending time back to client");

    return TRUE;
}


/* sends msg to client at addr over Xbee */
int sendXbee(char * msg, uint16_t addr) {

    // dont need to ensure it was sent, let client retry
    Tx16Request tx = Tx16Request(addr, (uint8_t *)msg, strlen(msg));
    xbee.send(tx);
}

