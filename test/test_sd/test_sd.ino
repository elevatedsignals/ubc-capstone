#define PASS 1
#define FAIL 0
#define TRUE 1
#define FALSE 0
#define TEST_SIZE 5

#include "SDCard.h"
#include "constants.h"
#include <stdio.h>

struct SD_card init_sd(String file_name, int &error) {
      
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
     Serial.println("Error: No SD Card Inserted");
     error = TRUE;
    }
    
   else {
     Serial.println("Error: Failed to initialize SD Card");
     error = TRUE;
   }
   
   return sd;
  }
  /*
   * Unit tests for SD card
   */
  void setup() {
    Serial.begin(9600);
	  
	  float temperature_data, humidity_data, CO2_data;
	  int error = FALSE;
	  struct SD_card sd = init_sd("unit.txt", error);	  	  
	  
	  // Invalid temperature data
	  for(float i = 0; i < TEST_SIZE; ++i) {
	    temperature_data = -1000;
	    humidity_data = i;
		  CO2_data = i;
		
		  write_sd(sd,temperature_data, humidity_data, CO2_data, error);
	  }
    if(error) {
      Serial.println("Test 1 failed to execute");
    }
    else {
      Serial.println("Test 1 successfully executed");
    }
    error = FALSE;
	  
	  // Invalid humidity data

    for(float i = 0; i < TEST_SIZE; ++i) {
      temperature_data = i;
      humidity_data = -1000;
      CO2_data = i;
    
      write_sd(sd, temperature_data, humidity_data, CO2_data, error);
    }
    if(error) {
      Serial.println("Test 2 failed to execute");
    }
    else {
      Serial.println("Test 2 successfully executed");
    }
    error = FALSE;

    // Invalid CO2 data
    for(float i = 0; i < TEST_SIZE; ++i) {
      temperature_data = i;
      humidity_data = i;
      CO2_data = -1000;
    
      write_sd(sd, temperature_data, humidity_data, CO2_data, error);
    }
    if(error) {
      Serial.println("Test 3 failed to execute");
    }
    else {
      Serial.println("Test 3 successfully executed");
    }
    error = FALSE;
    
	  // All Invalid data
   for(float i = 0; i < TEST_SIZE; ++i) {
     temperature_data = -1000;
      humidity_data = -1000;
      CO2_data = -1000;
    
      write_sd(sd,temperature_data, humidity_data, CO2_data, error);
    }
    if(error) {
      Serial.println("Test 4 failed to execute");
    }
    else {
      Serial.println("Test 4 successfully executed");
    }
    error = FALSE;
	  
	  // All valid data
   for(float i = 0; i < TEST_SIZE; ++i) {
     temperature_data = i;
      humidity_data = i;
      CO2_data = i;
    
      write_sd(sd,temperature_data, humidity_data, CO2_data, error);
    
    }
    if(error) {
      Serial.println("Test 5 failed to execute");
    }
    else {
      Serial.println("Test 5 successfully executed");
    }
    error = FALSE;

    read_sd(sd, error);
	  
  }

  void loop() {

  }

