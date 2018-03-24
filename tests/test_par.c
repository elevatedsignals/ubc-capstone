#include "../libraries/PAR/PAR.h"
#include "../libraries/General/constants.h"
#include <stdio.h>

int error;

/*
 *  Unit tests for the SQ500 PAR Sensor
 */
int main(void) {
  float par_volt, par_conc;

  /*  PAR Test 1
   *  Purpose: Checks for standard output of get_par_voltage.
   *  Expected: !error
   */
  error = FALSE;
  par_volt = get_par_voltage(&error);
  if(!error && (par_volt > -0.1) && (par_volt < 4000.1)) {
    printf("PAR Test 1 Passed. Read PAR voltage value within expected range (0 - 4000).\n");
  }
  else {
    printf("PAR Test 1 Failed. Did not read PAR voltage value within expected range (0 - 4000).\n");
  }

  /*  PAR Test 2
   *  Purpose: Checks for error handling in input of get_par_concentration.
   *  Input: A below-bounds PAR voltage value
   *  Expected: error
   */
  error = FALSE;
  par_volt = -0.1;
  par_conc = get_par_concentration(par_volt, &error);
  if(error) {
    printf("PAR Test 2 Passed. Threw error when given PAR voltage value (-0.1) was outside expected range (0 - 4000).\n");
  }
  else {
    printf("PAR Test 2 Failed. Did not throw error when given PAR voltage value (-0.1) was outside expected range (0 - 4000).\n");
  }

  /*  PAR Test 3
   *  Purpose: Checks for error handling in input of get_par_concentration.
   *  Input: An above-bounds PAR voltage value
   *  Expected: error
   */
  error = FALSE;
  par_volt = 40.1;
  par_conc = get_par_concentration(par_volt, &error);
  if(error) {
    printf("PAR Test 3 Passed. Threw error when given PAR voltage value (40.1) was outside expected range (0 - 4000).\n");
  }
  else {
    printf("PAR Test 3 Failed. Did not throw error when given PAR voltage value (40.1) was outside expected range (0 - 4000).\n");
  }

  /*  PAR Test 4
   *  Purpose: Checks for expected lower-bound output of get_par_concentration.
   *  Input: A lower-bounds par_voltage
   *  Expected: !error, par_conc = 0
   */
  error = FALSE;
  par_volt = 0.0;
  par_conc = get_par_voltage(par_volt, &error);
  if(!error && (par_conc == 0.0)) {
    printf("PAR Test 4 Passed. Did not throw error when given par_voltage value (0.0) was within expected range (0 - 4000).\n");
  }
  else {
    printf("PAR Test 4 Failed. Threw error when given par_voltage value (0.0) was within expected range (0 - 4000).\n");
  }

  /*  PAR Test 5
   *  Purpose: Checks for expected upper-bound output of get_par_concentration.
   *  Input: An upper-bound PAR voltage value
   *  Expected: !error, par_conc = 5000
   */
  error = FALSE;
  par_volt = 40.0;
  par_conc = get_par_concentration(par_volt, &error);
  if(!error && (par_conc == 4000.0)) {
    printf("PAR Test 5 Passed. Did not throw error when given PAR voltage value (40.0) was within expected range (0 - 4000).\n");
  }
  else {
    printf("PAR Test 5 Failed. Threw error when given PAR voltage value (40.0) was within expected range (0 - 4000).\n");
  }

  /*  PAR Test 6
   *  Purpose: Checks for expected output of get_par_concentration.
   *  Input: A PAR voltage value in the expected range (400 - 2000) e.g. 1200.5
   *  Expected: !error, (e.g.) par_conc = 2501.5625
   */
  error = FALSE;
  par_volt = 2000.5;
  par_conc = get_par_concentration(par_volt, &error);
  if(!error && (par_conc > 2501.562) && (par_conc < 2501.563)) {
    printf("PAR Test 6 Passed. Did not throw error when given PAR voltage value (2000.5) was within expected range (400 - 2000).\n");
  }
  else {
    printf("PAR Test 6 Failed. Threw error when given PAR voltage value (2000.5) was within expected range (400 - 2000).\n");
  }
}
