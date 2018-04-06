#ifndef SDCARD_H
#define SDCARD_H

#include "SD.h"
#include "Constants.h"
#include "XBee.h"
#include "SendXBee.h"

#define ERROR_VALUE -1000
#define SENSOR_TXT "Sensor.txt"

struct SD_card {
  char *data_file; // file name to be written to and read from
  File write_file;
  File read_file;

  // Header and tail for keeping track of queue position for reading and writing
  unsigned long sensor_module_head = 0;
  unsigned long sensor_module_tail = 0;
};

/*
 * Purpose: Initialize the SD card and allows for reading and writing
 * Output: SD_card obj
 */
struct SD_card init_sd(char *data_file, bool * error) {

  struct SD_card sd;
  sd.data_file = data_file;

  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(PIN_SD, OUTPUT);
  pinMode(PIN_SD_CHECK, INPUT);

  // check if initialization is complete, and that an SD card is inserted
  if (SD.begin(PIN_SD) && (!digitalRead(PIN_SD_CHECK))) {
    //Serial.println(F("SD card: Initialization Succeeded"));
  } else if (digitalRead(PIN_SD_CHECK)) {
    //Serial.println(F(ERROR_NOSD)); * error = TRUE;
  } else {
    //Serial.println(F(ERROR_INITSD)); * error = TRUE;
  }

  return sd;
}

/*
 * Purpose: Prints file contents to serial monitor
 * Output: TRUE if successful, FALSE otherwise
 */
bool read_sd(struct SD_card sd, bool * error) {

  //open file for reading
  sd.read_file = SD.open(sd.data_file, FILE_READ);

  if (sd.read_file) {

    // Read from file until there's nothing else in it
    while (sd.read_file.available()) {
      Serial.write(sd.read_file.read());
    }

  } else {
    // read unsuccessful
    * error = TRUE;
    sd.read_file.close();
    return FALSE;
  }

  sd.read_file.close();
  return TRUE;
}

/*
 * Purpose: Write to SD file name and return whether or not write was successful
 * Output: TRUE if successful, FALSE otherwise
 */
bool write_sensor_module_message(struct SD_card sd, char * message, bool *error) {

  // open file for writing
  sd.write_file = SD.open(SENSOR_TXT, FILE_WRITE);

  // check that file opened properly
  if (sd.write_file) {
    sd.write_file.print(message);
    sd.write_file.print("\n");

  } else {
    // write unsuccessful
    //Serial.println(F("SD card: Write fail"));
    *error = TRUE;
    return FALSE;
  }

  sd.write_file.close();

  return TRUE;
}

/*
* Purpose: Tries to send sensor module data stored on SD card line by line.
* This data is only stored in the event of a communication failure. If a
* line is successfully sent, it will be skipped on the next iteration. if
* all message are sent,  commFailureOcurred = FALSE; Clear SD card file,
* and clear header and tail pointer. if any failed, commFailureOccurred = TRUE;
* Output: TRUE if failed, FALSE if success
*/
bool recover_sensor_module_data(struct SD_card * sd, XBee xbee) {

  char line[PACKET_SIZE];
  int8_t count = 0;
  sd -> read_file = SD.open(SENSOR_TXT, O_RDWR);
  // As the file is written to, the head will change position
  sd -> sensor_module_head = sd -> read_file.position();
  sd -> read_file.seek(sd -> sensor_module_tail);

  if (sd -> read_file) {
    // Read from file until there's nothing else in it
    while (sd -> read_file.available()) {

      // Read the next line
      sd -> read_file.readStringUntil('\n').toCharArray(line, PACKET_SIZE + 1);

      // Use count < n if you want to simulate values being sent in blocks
      if (sendXbeeVerify(line, xbee)) {
        // If the message is successfully sent, we will advance the pointer
        sd -> sensor_module_tail = sd -> read_file.position();
        count++;
      }
      // Otherwise we had an error transmitting; break and return error
      else {
        //Serial.println(F("SD card: Error transmitting queued data."));
        sd -> read_file.close();
        count++;
        return TRUE;
      }

      // When we run out of data, clear the file
      if (sd -> sensor_module_tail == sd -> sensor_module_head) {
        sd -> sensor_module_tail = 0;
        sd -> read_file.close();
        sd -> write_file = SD.open(SENSOR_TXT, FILE_WRITE);
        sd -> write_file.truncate(0);
        sd -> write_file.close();
        //Serial.println(F("SD card: Sent all queued data."));
        return FALSE;
      }
    }
  } else {
    // read unsuccessful
    sd -> read_file.close();
    return TRUE;
  }

  return TRUE;

}
#endif
