#include "constants_bs.h"
#include "MiniW5100.h"
#include "SDCard_Base.h"

EthernetClient client; // struct for functions that manipulate communication with server
volatile int commFailureOccurred = FALSE;

void setup() {

  // TODO change ethernet pin 9 - 10
  Serial.begin(9600);
  float sensor_value = 1.0; // REPLACE TEST VALUE
  int capability_id = ID_AF; // REPLACE TEST VALUE

  client = init_ethernet(client);

 print_ip();

 int error = FALSE;
 char *payload = prepare_payload(capability_id, sensor_value, "2018::01::12", &error);
 Serial.println(F("Payload prepared:"));
  Serial.println(payload);

 if(error) {
   Serial.println(F("Failed to prepare payload."));
 } else {
   client = make_http_request(client, payload, &error);
   if(error) {
     Serial.println(F("Failed to make HTTP request."));
   }
   else {
    Serial.println(F("Successfully made HTTP request."));
   }
 }

   /* SD interfacing code */
//   int SDerror = FALSE;
//   struct SD_card sd = init_sd(TXT_FILE, &SDerror); // TODO we shouldnt initialize everytime when we refactor this
//
//
//   // We failed to send and SD initialized
//   if(error & !SDerror){
//     write_base_station_message(sd, payload, &SDerror);
//     commFailureOccurred = TRUE;
//   }
//   // We managed to send and SD initialized
//   else if(!error & !SDerror){
//
//   // Check for queued messages
//     if(commFailureOccurred){
//       commFailureOccurred = recover_base_station_data(&sd, client);
//     }
//
//   }



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
