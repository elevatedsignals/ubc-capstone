#ifndef STUNNA_H
#define STUNNA_H

#define TRUE 1
#define FALSE 0
#define SRC_ADDRESS "0xCE06" // for informational purposes only
#define DEST_ADDRESS 0xAB7F
#define PACKET_SIZE 100

int sendXbee(char * msg, XBee xbee) {

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

#endif