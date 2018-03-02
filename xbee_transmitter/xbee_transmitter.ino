
#include <XBee.h>

// XBEE transmitter code

#define TRUE 1
#define FALSE 0
#define SRC_ADDRESS "0xCE06" // for informational purposes only
#define DEST_ADDRESS 0xAB7F
#define PACKET_SIZE 100

// XBEE channel = C, pan id = F5D9

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();

// dont start unless we got/set current time
int gotTime = FALSE;

void setup() {

    Serial.begin(9600); //Baud rate must be the same as is on xBee module
    xbee.setSerial(Serial);

}


void loop() {

    while (!gotTime) {
        delay(1000);
        Serial.println("Waiting for time before starting");
        gotTime = getTime();
    }

    char * msg = "{ uniqueid: 123, timestamp: 00:12:14 }"; // TODO example msg

    if (sendXbee(msg)) {
        // TODO success, go to sleep
        Serial.println("Msg sent");
    }
    else {
        // TODO fail, write to sd
        Serial.println("Msg failed to send");
    }

}

int sendXbee(char * msg) {

    // 16-bit addressing: Enter address of remote XBee, typically the coordinator
    Tx16Request tx = Tx16Request(DEST_ADDRESS, (uint8_t *)msg, strlen(msg));
    TxStatusResponse txStatus = TxStatusResponse();

    xbee.send(tx);

    // after sending a tx request, we expect a status response
    // wait up to 2 seconds for the status response
    if (xbee.readPacket(2000)) {

        // if a tx request is complete APIidentifier == 0x89
        if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
            // get status frame
            xbee.getResponse().getTxStatusResponse(txStatus);

            // get the delivery status, 6th frame byte == 0
            if (txStatus.getStatus() == SUCCESS) {
                return TRUE;
            } else {
                return FALSE;
            }
        }

    }

    return FALSE;

}


int getTime() {

    // TODO send time request
    char request[PACKET_SIZE] = "";
    strcpy(request, "{ request time, srcAddr:");
    strcat(request, SRC_ADDRESS);
    strcat(request, "}");

    // send time request
    if (!sendXbee(request)) {
        return FALSE;
    }


    // wait for response containing timestamp
    uint8_t data = 0;

    xbee.readPacket(2000); // wait max 2 secs

    if (xbee.getResponse().isAvailable()) {

        if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
            // got RX packet with 16 bit address
            xbee.getResponse().getRx16Response(rx16);

            char msg[PACKET_SIZE] = ""; // TODO adjust size based on msg length
            int i;
            // read the whole message
            for (i = 0; i < rx16.getDataLength(), i < 100; i++) {
                data = rx16.getData(i);
                msg[i] = data;;
            }
            msg[i] = '\0';

            Serial.println(msg);

            // this is a valid msg to handle
            if (strstr(msg, "{") && strstr(msg, "}")) {
                Serial.println("valid");

                // request for timestamp or message with sensor data?
                if (strstr(msg, "time response") != NULL) {
                    // this message contains the time

                    char *time = extractTime(msg);
                    if (time != NULL ) {
                        // got the time
                        Serial.println("Received current time: ");
                        Serial.println(time);
                        // TODO set time
                        return TRUE;
                    }

                }

            }


        }

    }

    return FALSE;

}

char* extractTime(char *message) {

    char msg[PACKET_SIZE];
    strcpy(msg, message);

    char *time;
    char *token;

    // format should be {time response, current time=<time>}
    // where <time> is the timestamp
    // cant use ':' as timestamp will contain that so wont parse properly
    if ( strstr(msg, "current time") != NULL ) {
        token = strtok(msg, "}");
        token = strstr(token, "=");
        memmove(token, token+1, strlen(token)-1);
        time = token;

        return time;
    }

    return NULL;
}

