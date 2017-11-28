#define PASS 1
#define FAIL 0
#define TRUE 1
#define FALSE 0
#define TEST_SIZE 5

#include "constants.h"
#include "SDCard.h"
#include <stdio.h>

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

