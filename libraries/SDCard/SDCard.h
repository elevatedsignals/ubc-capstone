#ifndef SDCARD_H
#define SDCARD_H

#include <SD.h>

struct SD_card {
  volatile int is_ready; // whether the SD card has been initialized or not
  String file_name; // file name to be written to and read from
  File write_file;
  File read_file;
};

/*
 * Purpose: Determines whether SD card has been initialized
 * Output: true or false
 */
int sd_is_ready(struct SD_card sd) {
  return sd.is_ready;
}

/*
 * Purpose: Write to SD file name and return whether or not write was successful
 * Output: true or false
 */
int write_sd(struct SD_card sd, int data) {
  
  // open file for writing
  sd.write_file = SD.open(sd.file_name, FILE_WRITE);
	
  // check that file opened properly
  if(sd.write_file) {
    sd.write_file.println(data);
  }
  else {
	// write unsuccessful  
    return 0; 
  }
	
  sd.write_file.close();
	
  return 1;
}

/*
 * Purpose: Read from SD file name and return whether or not read was successful
 * Output: true or false
 */
int read_sd(struct SD_card sd){
  
  //open file for reading
  sd.read_file = SD.open(sd.file_name, FILE_READ);
	
  if(sd.read_file) {
		
    // Read from file until there's nothing else in it
    while(sd.read_file.available()) {
	  Serial.write(sd.read_file.read());
	}
		
	sd.read_file.close();
	}
  else {
    // read unsuccessful
	return 0; 
  }

  return 1;
}
	
#endif