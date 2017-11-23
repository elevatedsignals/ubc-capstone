
#include "constants.h"
#include "DHT.h"
#include "TH.h"

/*
* Purpose: initializes the DHT sensor and polls the sensor till it obtains
* temperature and humidity data. This needs to be in a .ino file
* (not c file) as DHT library code is in c++.
* Output: the TH struct containing all DHT sensor data
*/
struct TH init_dht(void) {

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

}

void loop() {

}
