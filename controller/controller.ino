#include "constants.h"
#include "DHT.h"
#include "TH.h"
#include "SDCard.h"

/*
* Purpose: initializes the DHT sensor and polls the sensor till it obtains
* temperature and humidity data. This needs to be in a .ino file
* (not c file) as DHT library code is in c++.
* Output: the TH struct containing all DHT sensor data
*/
struct TH init_dht(void) {

  /* to determine whether a temp and humidity reading was obtained, we
   initialize them to -1000 since those are invalid values */
  struct TH t_h = {-1000, -1000};
  float h;
  float t;

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

  if (!isnan(t)) {
    t_h.t = t;
  }
  if (!isnan(h)) {
    t_h.h = h;
  }

  return t_h;
}

void setup() {
  Serial.begin(9600);
  // set default reference voltage (5V)
  analogReference(DEFAULT);

  int error;

  /* DHT temperature and humidity code */
  struct TH t_h = init_dht();

  error = FALSE;
  float t = get_temp(t_h, &error);
  if (error) {
    Serial.print(ERROR_TEMP);
  }
  error = FALSE;
  float h = get_humidity(t_h, &error);
  if (error) {
    Serial.print(ERROR_HUMIDITY);
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" \t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  
  /* SD interfacing code */
  error = FALSE;
  struct SD_card sd = init_sd("data.txt", error); // TODO check error pointer 

  // writes/reads to SD Card if initialized properly
  if(!error) {
    Serial.println("Begin writing to SD");
    // TODO pass in sensor vals
    /*
    for(float i = 0; i < 10; ++i){
      write_sd(sd, i, i, i, error);
    }
    
    // reads from SD card
    read_sd(sd, error);
    */
    SD.remove("data.txt"); // TODO create const filename
    
  } 
  Serial.println("SD Finished");

}

void loop() {

}
