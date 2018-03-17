#include "constants_bs.h"
#include "MiniW5100.h"
#include "SDCard.h"

EthernetClient client; // struct for functions that manipulate communication with server

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing startup variables.");
  String payload;
  float sensor_value = 50.0; // REPLACE TEST VALUE
  int capability_id = ID_TEMP; // REPLACE TEST VALUE
  int error = FALSE; // REPLACE TEST VALUE

  Serial.println("Initializing IP address.");
  client = init_ethernet(client);
  Serial.println("Ethernet initialization successful");
//  print_ip();
//
//  payload = prepare_payload(&payload, capability_id, sensor_value, NULL, &error);
//  Serial.println("Payload prepared. Attempting to upload.");
//
//  if(error) {
//    Serial.println("Failed to prepare payload.");
//  } else {
//    client = make_http_request(client, payload, &error);
//
//    if(error) {
//      Serial.println("Failed to make HTTP request.");
//    }
//  }

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
