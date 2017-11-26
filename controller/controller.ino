#include "constants.h"
#include "DHT.h"
#include "TH.h"
#include "CO2.h"
#include "SDCard.h"

/*
 * Purpose: Initializes the DHT sensor and polls the sensor till it obtains
 *          temperature and humidity data.
 * Output: the TH struct containing all sensor data
 */
 struct TH init_dht(void) {
   
   struct TH t_h;
   float h;
   float t;
   float hi;

   t_h.has_data = FALSE;

   // initialize DHT sensor
   DHT dht(PIN_DHT, DHTTYPE);
   dht.begin();

   int attempt = 0; // track number of poll attempts

   // try to poll data for 20 secs max
   do {
     // can only poll every 2 seconds
     delay(2000);
     // reading temperature or humidity takes about 250 ms
     h = dht.readHumidity();
     // read temperature as Celsius (default)
     t = dht.readTemperature();
     attempt++;
   } while((isnan(h) || isnan(t)) & attempt < 10);

   // do error checking
   if (isnan(h) || isnan(t)) {
     return t_h; // is_ready = false
   }

   // compute heat index in Celsius (isFahreheit = false)
   hi = dht.computeHeatIndex(t, h, false);

   t_h.t = t;
   t_h.h = h;
   t_h.hi = hi;
   t_h.has_data = TRUE;

   return t_h;
 }

 /*
  * Purpose: Polls the Infrared CO2 Sensor till it obtains a sensor value and calculates
  *          the CO2 concentration value.
  * Output: the CO2 struct containing all sensor data
  */
  struct CO2 init_ir_co2(void) {
    struct CO2 co2;
    float co2_conc;
    int sensor_value;

    co2.has_data = FALSE;

    int attempt = 0; // track number of poll attempts

    // try to poll data for 1 sec max
    do {
      // can only poll every 100 milliseconds
      delay(100);
      // read voltage
      sensor_value = analogRead(PIN_CO2);
      attempt++;
    } while(isnan(sensor_value) & attempt < 10);

    // analog signal is converted to a voltage value
    float voltage = sensor_value * (5000 / 1024.0);

    if(voltage == 0)
    {
      Serial.println("Fault");
    }
    else if(voltage < 400)
    {
      Serial.println("preheating");
    }
    else
    {
      int voltage_difference = voltage - 400;
      co2_conc = voltage_difference * (50.0 / 16.0);
    }

    // do error checking
    if (isnan(co2_conc)) {
      return co2; // is_ready = false
    }

    co2.concentration = co2_conc;
    co2.has_data = TRUE;

    return co2;
  }
  
  /*
  * Purpose: Initialize the SD card and allows for reading and writing
  * Output: N/A
  */
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
  

void setup() {
  Serial.begin(9600);
  // set default reference voltage (5V)
  analogReference(DEFAULT);
  SD.remove("data.txt");

  int error = FALSE;

  struct SD_card sd = init_sd("data.txt", error);

  // writes/reads to SD Card if initialized properly
  if(!error) {
    Serial.println("Begin writing");
    for(float i = 0; i < 10; ++i){
      write_sd(sd, i, i, i, error);
    }

    // Reads from SD card
    read_sd(sd, error);
  } 

Serial.println("Finished");

}

void loop() {

}
