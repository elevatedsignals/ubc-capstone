
#include "constants.h"
#include "CO2.h"
int error;
void setup() {
  Serial.begin(9600);
  // set default reference voltage
  analogReference(DEFAULT);
  float co2_volt, co2_conc;
  // set error variable to default
  error = FALSE; 
  co2_volt = get_co2_voltage(&error);
  if (error) {
    Serial.print(ERROR_GCO2V);
  }
  co2_conc = get_co2_concentration(co2_volt, &error);
  if (error) {
    Serial.print(ERROR_GCO2C);
  }
  Serial.print("CO2 Concentration: ");
  Serial.print(co2_conc);
  Serial.print(" ppm");
}
void loop() {
}
