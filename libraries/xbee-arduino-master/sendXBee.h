#ifndef SEND_XBEE_H
#define SEND_XBEE_H

#include "Constants.h"

/*
* Purpose: Send msg over Xbee and check whether
* the destination receieved it (via an ACK)
* Output: TRUE if destination receieved it, FALSE otherwise
*/
int8_t sendXbeeVerify(char * msg, XBee xbee) {

  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(DEST_ADDRESS, (uint8_t * ) msg, strlen(msg));
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

/*
* Purpose: Send msg over Xbee with Address addr, but
* does not check to see if destination receieved it
*/
int8_t sendXbeeNoVerify(char * msg, uint16_t addr, XBee xbee) {
  Tx16Request tx = Tx16Request(addr, (uint8_t * ) msg, strlen(msg));
  xbee.send(tx);
}

#endif
