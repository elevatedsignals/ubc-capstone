#ifndef SDCARD_H
#define SDCARD_H

#include <SD.h>
#define ERROR_VALUE -1000

struct SD_card {
  String file_name; // file name to be written to and read from
  File write_file;
  File read_file;
};

/*
 * Purpose: Write to SD file name and return whether or not write was successful
 * Output: true or false
 */
int write_sd(struct SD_card sd, float temperature_data, float humidity_data, float CO2_data, int &error) {
  
  // open file for writing
  sd.write_file = SD.open(sd.file_name, FILE_WRITE);
	
  // check that file opened properly
  if(sd.write_file) {
	
	sd.write_file.print("{ \"temperature\":");
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
	
	sd.write_file.print("\"CO2\"");
	if(CO2_data == ERROR_VALUE) {
	  sd.write_file.print("NaN");
	}
	else{
      sd.write_file.print(CO2_data);
	}
	sd.write_file.print("\n");
	
  }
  else {
	// write unsuccessful  
	error = TRUE;
    return 0; 
  }
	
  sd.write_file.close();
	
  return 1;
}

/*
 * Purpose: Read from SD file name and return whether or not read was successful
 * Output: true or false
 */
int read_sd(struct SD_card sd, int &error){
  
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
	error = TRUE;
	sd.read_file.close();
	return 0; 
  }

  sd.read_file.close();
  return 1;
}
	
#endif