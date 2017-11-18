
#include "constants.h"
#include "DHT.h"
#include "TH.h"

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

void setup() {
  Serial.begin(9600);

  // DHT temperature and humidity code
  struct TH t_h = init_dht();

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

}

void loop() {

}
