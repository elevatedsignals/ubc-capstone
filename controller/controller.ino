#include "constants.h"
#include "DHT.h"
#include "TH.h"
#include "SDCard.h"
#include "CO2.h"
#include "airflow.h"

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
    delay(500);
    // reading temperature or humidity takes about 250 ms
    h = dht.readHumidity();
    // read temperature as Celsius (default)
    t = dht.readTemperature();
    attempt++;
  } while((isnan(h) || isnan(t)) & attempt < 5);

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
    Serial.println(ERROR_TEMP);
  }
  error = FALSE;
  float h = get_humidity(t_h, &error);
  if (error) {
    Serial.println(ERROR_HUMIDITY);
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.println(h);

  /* CO2 code */
  float co2_volt, co2_conc;

  error = FALSE;
  co2_volt = get_co2_voltage(&error);
  if (error) {
    Serial.println(ERROR_GCO2V);
  }
  error = FALSE;
  co2_conc = get_co2_concentration(co2_volt, &error);
  if (error) {
    Serial.println(ERROR_GCO2C);
    co2_conc = -1000;
  }
  Serial.print("CO2 Concentration: ");
  Serial.print(co2_conc);
  Serial.println(" ppm");

  /* Airflow code */
  float airflow_MPH; 
  bool airflow_bool;

  error = FALSE;
  airflow_MPH = get_wind_speed(&error);
  if (error) {
    Serial.println(ERROR_AIRFLOW);
    airflow_MPH = -1000;
  }
  airflow_bool = get_airflow(airflow_MPH);
  if (airflow_bool) {
    Serial.println("Airflow: Yes");
  }
  else {
    Serial.println("Airflow: No");
  }

  /* SD interfacing code */
  error = FALSE;
  struct SD_card sd = init_sd(TXT_FILE, &error); 

  // writes/reads to SD Card if initialized properly
  if(!error) {
    Serial.println("Begin writing to SD");
    write_sd(sd, t, h, co2_conc, airflow_bool, &error);
  }
  error = FALSE;
  read_sd(sd, &error);
  Serial.println("SD Finished");
}

void loop() {
}
