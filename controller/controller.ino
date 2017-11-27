
#include "constants.h"
#include "CO2.h"

 /*
  * Purpose: Polls the Infrared CO2 Sensor till it obtains a sensor value and calculates
  *          the CO2 concentration value.
  * Output: the CO2 struct containing all sensor data
  */
  float init_ir_co2(void) {
    float co2_conc = -1000;

    int sensor_value;

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

    if(voltage < 400) {
      return co2_conc;
    }

    else {
      int voltage_difference = voltage - 400;
      co2_conc = voltage_difference * (50.0 / 16.0);
    }

    return co2_conc;
  }

void setup() {
  Serial.begin(9600);

  int error;

  // set default reference voltage
  analogReference(DEFAULT);

  // CO2 concentration code
  float co2 = init_ir_co2();

  error = FALSE;

  float co2_conc = get_concentration(co2, &error);
  if (error) {
    Serial.print(ERROR_CO2);
  }

  Serial.print("CO2 Concentration: ");
  Serial.print(co2_conc);
  Serial.print(" ppm");

}

void loop() {

}
