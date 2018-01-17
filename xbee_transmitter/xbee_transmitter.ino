
#include <XBee.h>

// XBEE transmitter code

#define TRUE 1
#define FALSE 0
#define SRC_ADDRESS 0x0000
#define DEST_ADDRESS 0xFFFF

XBee xbee = XBee();

void setup() {

  Serial.begin(9600); //Baud rate must be the same as is on xBee module
  xbee.setSerial(Serial);
  
  char * msg = "{uniqueid: 123, timestamp: 00:12:14}"; // example msg
  
  if (sendXbee(msg)) {
    // success, go to sleep
  }
  else {
    // fail, write to sd
  }

}


void loop() {

}

int sendXbee(char * msg) {
  
  printf("Sending xbee msg");

  XBee xbee = XBee();

  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(DEST_ADDRESS, (uint8_t *)msg, strlen(msg)); // TODO

  TxStatusResponse txStatus = TxStatusResponse();

  xbee.send(tx);

  // after sending a tx request, we expect a status response
  // wait up to 5 seconds for the status response
  if (xbee.readPacket(5000)) {
    // got a response!
    printf("got a respnose");

    // if a tx request is complete APIidentifier == 0x89     	
    if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      // get status frame
      xbee.getResponse().getTxStatusResponse(txStatus);

      // get the delivery status, 6th frame byte == 0
      if (txStatus.getStatus() == SUCCESS) {
        printf("delivered");
        return TRUE;
      } else {
        printf("undelivered");
        return FALSE;
      }
    }

  }

  return FALSE;

}

/*
int send(char * msg) {

  Serial.println("< %s >", msg); // send data

  int count = 0;
  int found = FALSE;

  char * recv

  // wait for ack, at most ~10 seconds
  while (!found) {
    // if theres data, verify its an ack for us
    if (Serial.available() > 0) {
      int i = 0;
      while (Serial.available() > 0) { // read it all
        recv[i] = Serial.read();
        i++;
      }
      recv[i] = '\0';

      // if the msg contained an ack for us, return true
      if (strstr(recv, SENSORID) && strstr(recv, "ack")) {}
    }

    // keep waiting till timeout
    if (count >= 10) {
      break;
    }
    delay(1000);
    count++;
  }

  return FALSE;
}
*/
// notes

/*
2) trade off between power consumption and range, mesh networks and interrupts for loopback
- if we want sensor modules to comm with eachother,
this requires they all be on always to be able to receieve the msgs, extra interrupt
-since we want to maximimize
battery life, they are put to sleep. so sensor modules can only comm with base station,
must be 30 meters in range.
3) at base-station, verify the incoming data is valid (contains sensor id, timestamp, and sensor values)
4) use api  mode 2
8) serial port may be unavailable for debugging as xbee uses it
9) verify uint16_t ZBRxResponse::getRemoteAddress16() 
*/
