#include "constants.h"
#include "PAR.h"

void setup() {
  Serial.begin(9600);
  // set default reference voltage (5V)
  analogReference(DEFAULT);

  int error;

  /* PAR code */
  float par_volt, par_conc;

  error = FALSE;
  par_volt = get_par_voltage(&error);
  if (error) {
    Serial.println(ERROR_GPARV);
  }
  error = FALSE;
  par_conc = get_par_concentration(par_volt, &error);
  if (error) {
    Serial.println(ERROR_GPARC);
  }
  Serial.print("PAR Concentration: ");
  Serial.print(par_conc);
  Serial.println(" ppm");
}

void loop() {

}
