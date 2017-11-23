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
 struct SD_card init_sd(String file_name) {
   	 
   struct SD_card sd;
   sd.file_name = file_name;	
   sd.is_ready = FALSE;
  
   // Note that even if it's not used as the CS pin, the hardware SS pin 
   // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
   // or the SD library functions will not work. 
   pinMode(PIN_SD, OUTPUT);
 
   // check if initialization is complete
   if (!SD.begin(PIN_SD)) {
    // initialization failed
	   return sd;
   }

   sd.is_ready = TRUE;
   	return sd;
  }
  

void setup() {
  Serial.begin(9600);
  // set default reference voltage (5V)
  analogReference(DEFAULT);

  // DHT temperature and humidity code
  struct TH t_h = init_dht();

  // CO2 concentration code
  struct CO2 co2 = init_ir_co2();
  
  // SD card code
  struct SD_card sd = init_sd("beans.txt");

  if(is_ready(t_h)) {
    float t = get_temp(t_h);
    float h = get_humidity(t_h);
    float hi = get_heatindex(t_h);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" \t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Heat index: ");
    Serial.print(hi);
    Serial.print(" *C ");
  }
  else {
    Serial.print("Error reading DHT sensor");
  }

  Serial.print("\n");

  if(is_co2_ready(co2)) {
    float co2_conc = get_concentration(co2);

    Serial.print("CO2 Concentration: ");
    Serial.print(co2_conc);
    Serial.print(" ppm");
  }
  else {
    Serial.print("Error reading CO2 sensor");
  }
  
  Serial.print("\n");

  if(sd_is_ready(sd)) {
    for(int i =0; i < 255; ++i){
      write_sd(sd, i);
    }
	// Prints out all values written to SD Card
    read_sd(sd);
  } 
  else {
    Serial.print("Error reading SD card");  
  }

}

void loop() {

}
