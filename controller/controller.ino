
#include "constants.h"
#include "CO2.h"
int error;
void setup() {
  Serial.begin(9600);
  // set default reference voltage
  analogReference(DEFAULT);
  // set error variable to default
  error = FALSE;
  float co2_conc = get_co2_concentration(get_co2_voltage(&error));
  if (error) {
    Serial.print(ERROR_CO2);
  }
  Serial.print("CO2 Concentration: ");
  Serial.print(co2_conc);
  Serial.print(" ppm");
}
void loop() {
}
