#include "constants.h"
#include "XBee.h"
#include "sendXBee.h"
#include "SDCard.h"

void setup() {
  Serial.begin(9600);

  XBee xbee = XBee();
  xbee.setSerial(Serial);
  int commFailureOccurred = 0;
  int numberOfTestSamples = 10;
  /* SD interfacing code */
  int error = FALSE;
  struct SD_card sd = init_sd(TXT_FILE, &error); // TOD check error pointer
  char unsent_message[50]; 

  Serial.println("Begin writing to SD");
  
  for(int i = 0; i < numberOfTestSamples; ++i){
    sprintf(unsent_message, "%i", i);
    //write_base_station_message(sd, unsent_message, &error);
    write_sensor_module_message(sd, String(i), &error);
  }
  
  // writes/reads to SD Card if initialized properly
  if(!error) {

    do{
    commFailureOccurred = recover_sensor_module_data(&sd, xbee);
    //commFailureOccurred = recover_base_station_data(&sd, xbee);
    }while(commFailureOccurred);
    
  } 

   for(int i = 0; i < numberOfTestSamples; ++i){
    sprintf(unsent_message, "%i", i);
    //write_base_station_message(sd, unsent_message, &error);
    write_sensor_module_message(sd, String(i), &error);
  }
  
  // writes/reads to SD Card if initialized properly
  if(!error) {

    do{
    commFailureOccurred = recover_sensor_module_data(&sd, xbee);
    //commFailureOccurred = recover_base_station_data(&sd, xbee);
    }while(commFailureOccurred);
    
  } 

  

  
  Serial.println("SD Finished");

}

void loop() {
}

