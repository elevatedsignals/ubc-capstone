#include "constants_bs.h"
#include "constants.h"
#include "Format.h"
#include "MiniW5100.h"
#include "SDCard_Base.h"

#include <TimeLib.h>
#include <EthernetUdp.h>
#include "Timestamp.h"


EthernetClient client;
// track whether data has queued in SD storage
//volatile int commFailureOccurred = FALSE;

void setup() {

  Serial.begin(9600);

/*
  // get time from NTP server over UDP
client = init_ethernet(client);
initialize_UDP();
send_NTP_packet(timeServer);
// set the time accounting for delay
setTime(parse_NTP_packet() + ((DELAY_TIME) / 1000));
String clockTime = get_formatted_time();
char *timestamp = const_cast < char * > (clockTime.c_str());

Serial.println(timestamp);
*/



  float sensor_value = 58.0; // REPLACE TEST VALUE
  int capability_id = ID_HUM; // REPLACE TEST VALUE

  client = init_ethernet(client);

 print_ip();

 // TODO DO THIS FOR EACH SENSOR on SM
 int error = FALSE;
 // TODO format string based on system time, if got time
 // TODO error checking for didnt get time, pass in NULL
 char *payload = prepare_payload(capability_id, sensor_value, NULL, &error);
 Serial.println(F("Payload prepared:"));
  Serial.println(payload);

 if(error) {
   Serial.println(F("Failed to prepare payload."));
 } else {
   client = make_http_request(client, payload, &error);
   if(error) {
     Serial.println(F("Failed to make HTTP request."));
     // TODO STORE ON SD
     //if(!SDerror){
     //     write_base_station_message(sd, payload, &SDerror);
     //     commFailureOccurred = TRUE;
     //   }
   }
   else {
    Serial.println(F("Successfully made HTTP request."));

    // Check for queued messages
    //if(!SDerror){
    //     if(commFailureOccurred){
    //       commFailureOccurred = recover_base_station_data(&sd, client);
    //     }
    // }
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



}

void loop() {

}

/*
void upload(int capability_id, int value, char* time, int *error) {


   // TODO DO THIS FOR EACH SENSOR
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
       // TODO STORE ON SD
       //if(!SDerror){
       //     write_base_station_message(sd, payload, &SDerror);
       //     commFailureOccurred = TRUE;
       //   }
     }
     else {
      Serial.println(F("Successfully made HTTP request."));

      // Check for queued messages
      //if(!SDerror){
      //     if(commFailureOccurred){
      //       commFailureOccurred = recover_base_station_data(&sd, client);
      //     }
      // }
     }
   }

}*/
