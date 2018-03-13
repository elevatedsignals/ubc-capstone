#include "constants.h"
#include "XBee.h"
#include "sendXBee.h"
#include "SDCard.h"

void setup() {
  Serial.begin(9600);
  String t1 = SENSOR_MODULE_TXT_FILE;
  String t2 = BASE_TXT_FILE;
  /* SD interfacing code */
  int error = FALSE;
  struct SD_card sd = init_sd(TXT_FILE, &error); // TOD check error pointer 

  //write_sd(sd, 1, 1, 1, 1, &error);

  Serial.println("Begin writing to SD");
  write_sensor_module_message(sd, "One", &error);
  write_sensor_module_message(sd, "Two", &error);
  write_sensor_module_message(sd, "Three", &error);
  write_sensor_module_message(sd, "Four", &error);
  write_sensor_module_message(sd, "Five", &error);

  write_base_station_message(sd, "One", &error);
  write_base_station_message(sd, "Two", &error);
  write_base_station_message(sd, "Three", &error);
  write_base_station_message(sd, "Four", &error);
  write_base_station_message(sd, "Five", &error);
  
  // writes/reads to SD Card if initialized properly
  if(!error) {
    Serial.println("Begin reading from SD");
    // TODO pass in sensor vals
    // reads from SD card
    //read_sd(sd, &error);
    truncate_file(sd, "Sensor.txt", &error);
    truncate_file(sd, "Base.txt", &error);
    
  } 
  Serial.println("SD Finished");

}

void loop() {
}

