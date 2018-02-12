#include "constants_bs.h"
#include "MiniW5100.h"

EthernetClient client; // struct for functions that manipulate communication with server

void setup() {
  Serial.begin(9600);
  String payload;
  float x = 50.0; // REPLACE TEST VALUE
  int error = FALSE;

  client = init_ethernet(client);
  print_ip();

  // Remove from loop and replace x with appropriate sensor values
  int i;
  for(i = 0; i < 5; i++) {
    payload = prepare_payload(&payload, i, x);
    client = make_http_request(client, payload, &error);

    if(error) {
      Serial.println(ERROR_HTTP_REQUEST);
      error = FALSE;
    }
  }
}

void loop() {

}
