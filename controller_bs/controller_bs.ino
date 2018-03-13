#include "constants_bs.h"
#include "MiniW5100.h"
#include "SDCard.h"

EthernetClient client; // struct for functions that manipulate communication with server

void setup() {
  Serial.begin(9600);
  String payload;
  float sensor_value = 50.0; // REPLACE TEST VALUE
  int capability_id = ID_TEMP; // REPLACE TEST VALUE
  int error = FALSE; // REPLACE TEST VALUE

  client = init_ethernet(client);
  print_ip();

  // successfully-compiled hard code of send_to_server function
  payload = prepare_payload(&payload, capability_id, sensor_value, NULL);
  client = make_http_request(client, payload, &error);

  // @TODO: Fix send_to_server compilation errors
  // client = send_to_server(client, &payload, capability_id, sensor_value, NULL, &error);
  if(error) {
    Serial.println("Error: Failed to send sensor value to server.");
  }

  // // Remove from loop and replace x with appropriate sensor values
  // int i;
  // for(i = 0; i < 5; i++) {
  //   payload = prepare_payload(&payload, i, x);
  //   client = make_http_request(client, payload, &error);
  //
  //   if(error) {
  //     Serial.println(ERROR_HTTP_REQUEST);
  //     error = FALSE;
  //   }
  // }
}

void loop() {

}
