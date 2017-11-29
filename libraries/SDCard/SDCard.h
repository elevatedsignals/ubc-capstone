#ifndef SDCARD_H
#define SDCARD_H

#include <SD.h>
#include "constants.h"
#define ERROR_VALUE -1000

struct SD_card {
  String file_name; // file name to be written to and read from
  File write_file;
  File read_file;
};

/*
 * Purpose: Initialize the SD card and allows for reading and writing
 * Output: N/A
 */
 struct SD_card init_sd(String file_name, int *error) {

   struct SD_card sd;
   sd.file_name = file_name;

   // Note that even if it's not used as the CS pin, the hardware SS pin
   // (10 on most Arduino boards, 53 on the Mega) must be left as an output
   // or the SD library functions will not work.
   pinMode(PIN_SD, OUTPUT);
   pinMode(PIN_SD_CHECK, INPUT);

   // check if initialization is complete, and that an SD card is inserted
   if (SD.begin(PIN_SD) && digitalRead(PIN_SD_CHECK)) {
    Serial.println("Initialization Succeeded");
   }
   else if(!digitalRead(PIN_SD_CHECK)) {
     Serial.println(ERROR_NOSD);
     *error = TRUE;
    }

   else {
     Serial.println(ERROR_INITSD);
     *error = TRUE;
   }

   return sd;
  }

/*
 * Purpose: Write to SD file name and return whether or not write was successful
 * Output: true or false
 */
int write_sd(struct SD_card sd, float temperature_data, float humidity_data, float CO2_data, int *error) {

  // open file for writing
  sd.write_file = SD.open(sd.file_name, FILE_WRITE);

  // check that file opened properly
  if(sd.write_file) {

    sd.write_file.print("{ \"id\":");
    sd.write_file.print(MODULE_ID);
    sd.write_file.print(",");

  sd.write_file.print("\"temperature\":");
  if(temperature_data == ERROR_VALUE) {
      sd.write_file.print("NaN");
  }
  else {
    sd.write_file.print(temperature_data);
  }
  sd.write_file.print(",");

  sd.write_file.print("\"humidity\":");
  if(humidity_data == ERROR_VALUE) {
    sd.write_file.print("NaN");
  }
  else {
    sd.write_file.print(humidity_data);
  }
  sd.write_file.print(",");

  sd.write_file.print("\"CO2\":");
  if(CO2_data == ERROR_VALUE) {
    sd.write_file.print("NaN");
  }
  else{
      sd.write_file.print(CO2_data);
  }
  sd.write_file.print(" }\n");

  }
  else {
  // write unsuccessful
  *error = TRUE;
    return 0;
  }

  sd.write_file.close();

  return 1;
}

/*
 * Purpose: Read from SD file name and return whether or not read was successful
 * Output: true or false
 */
int read_sd(struct SD_card sd, int *error){

  //open file for reading
  sd.read_file = SD.open(sd.file_name, FILE_READ);

  if(sd.read_file) {

    // Read from file until there's nothing else in it
    while(sd.read_file.available()) {
    Serial.write(sd.read_file.read());
  }

  }
  else {
    // read unsuccessful
  *error = TRUE;
  sd.read_file.close();
  return 0;
  }

  sd.read_file.close();
  return 1;
}

#endif
