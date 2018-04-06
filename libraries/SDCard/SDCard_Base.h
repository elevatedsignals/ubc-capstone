#ifndef SDCARD_H
#define SDCARD_H

#include "SD.h"
#include "Constants_Base.h"

struct SD_card {
  String data_file; // file name to be written to and read from
  File write_file;
  File read_file;

  // Header and tail for keeping track of queue position for reading and writing
  unsigned long base_station_head = 0;
  unsigned long base_station_tail = 0;
  unsigned long sensor_module_head = 0;
  unsigned long sensor_module_tail = 0;
};

/*
 * Purpose: Initialize the SD card and allows for reading and writing
 * Output: N/A
 */
 struct SD_card init_sd(String data_file, int *error) {

   struct SD_card sd;
   sd.data_file = data_file;

   // Note that even if it's not used as the CS pin, the hardware SS pin
   // (10 on most Arduino boards, 53 on the Mega) must be left as an output
   // or the SD library functions will not work.
   pinMode(PIN_SD, OUTPUT);
   pinMode(PIN_SD_BASE_CHECK, INPUT);

   // check if initialization is complete, and that an SD card is inserted
   if (SD.begin(PIN_SD) && digitalRead(PIN_SD_BASE_CHECK)) {
    Serial.println(F("Initialization Succeeded"));
   }
   else if(!digitalRead(PIN_SD_BASE_CHECK)) {
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
int write_sd(struct SD_card sd, float temperature_data, float humidity_data, float CO2_data, bool airflow_data, int *error) {

  // open file for writing
  sd.write_file = SD.open(sd.data_file, FILE_WRITE);

  // check that file opened properly
  if(sd.write_file) {

    sd.write_file.print(F("{ \"id\":"));
    sd.write_file.print(MODULE_ID);
    sd.write_file.print(",");

  sd.write_file.print(F("\"temperature\":"));
  if(temperature_data == ERROR_VALUE) {
      sd.write_file.print(F("NaN"));
  }
  else {
    sd.write_file.print(temperature_data);
  }
  sd.write_file.print(F(","));

  sd.write_file.print(F("\"humidity\":"));
  if(humidity_data == ERROR_VALUE) {
    sd.write_file.print(F("NaN"));
  }
  else {
    sd.write_file.print(humidity_data);
  }
  sd.write_file.print(F(","));

  sd.write_file.print(F("\"CO2\":"));
  if(CO2_data == ERROR_VALUE) {
    sd.write_file.print(F("NaN"));
  }
  else{
      sd.write_file.print(CO2_data);
  }

   sd.write_file.print(F(","));

  sd.write_file.print(F("\"air flow\":"));
  if(CO2_data == ERROR_VALUE) {
    sd.write_file.print(F("NaN"));
  }
  else{
      if (airflow_data == TRUE) {
        sd.write_file.print(F("Yes"));
      }
      else {
        sd.write_file.print(F("No"));
      }
  }
  sd.write_file.print(F(" }\n"));

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
  sd.read_file = SD.open(sd.data_file, FILE_READ);

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


/*
 * Purpose: Writes base station message to SD Card in the event of an error in transmitting to web server. Increments head pointer.
 * Output: true or false
 */
 int write_base_station_message(struct SD_card sd, String unsent_message, int *error){

  // open error file for writing
  sd.write_file = SD.open(BASE_TXT, FILE_WRITE);

  // check that file opened properly
  if(sd.write_file) {
	  sd.write_file.print(unsent_message);
	  sd.write_file.print(F("\n"));

  }

  else {
	  // write unsuccessful
	*error = TRUE;
	sd.write_file.close();
	return 0;
  }

  sd.write_file.close();
  return 1;
 }


/*
 * Purpose: Tries to send base station data stored on SD card line by line. This data is only stored in the
 event of a communication failure. If a line is successfully sent, it will be skipped on the next iteration.

            if all message are sent,  commFailureOcurred = FALSE; Clear SD card file, and clear header and tail pointer.
            if any failed, commFailureOccurred = TRUE;

 * Output: true or false
 */
int recover_base_station_data(struct SD_card *sd, EthernetClient client){

	char line[PACKET_SIZE];
	int error = FALSE;
	sd->read_file = SD.open(BASE_TXT, O_RDWR);
	// As the file is written to, the head will change position
	sd->sensor_module_head = sd->read_file.position();
	sd->read_file.seek(sd->sensor_module_tail);

	if(sd->read_file) {
		// Read from file until there's nothing else in it
		while(sd->read_file.available()) {

			// Read the next line
			sd->read_file.readStringUntil('\n').toCharArray(line, PACKET_SIZE + 1);

			client = make_http_request(client, line, &error);

			// Use count < n if you want to simulate values being sent in blocks
				if(!error){

					// If the message is successfully sent, we will advance the pointer
					sd->sensor_module_tail = sd->read_file.position();

				}
				// Otherwise we had an error transmitting; break and return error
				else {
					Serial.println(F("Error transmitting"));
					sd->read_file.close();
					return 1;
				}

				// When we run out of data, clear the file
				if (sd->sensor_module_tail == sd->sensor_module_head)
				{
					sd->sensor_module_tail = 0;
					Serial.println(F("We have reached the end of the file"));
					sd->read_file.close();
					sd->write_file = SD.open(BASE_TXT, FILE_WRITE);
					sd->write_file.truncate(0);
					sd->write_file.close();
					return 0;
				}
		}
	}
	else {
		// read unsuccessful
		Serial.println(F("Didn't open"));
		sd->read_file.close();
		return 1;
	}


	return 1;

}

#endif
