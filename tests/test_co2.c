#include "../libraries/CO2/CO2.h"
#include "../libraries/General/constants.h"
#include <stdio.h>

#define PASS 1
#define FAIL 0

#define TRUE 1
#define FALSE 0

int error;

/*
 * Unit tests for the Gravity IR CO2 Sensor
 */
int main(void) {
  float co2_volt, co2_conc;
  
  /*  CO2 Test 1
   *  Purpose: Checks for standard output of get_co2_voltage.
   * Expected: !error 
   */
  error = FALSE;
  co2_volt = get_co2_voltage(&error);
  if (!error && (co2_volt > 399) && (co2_volt < 2401))
    printf("CO2 Test 1 Passed. Read co2_voltage value within expected range (400 - 2000).\n");
  }
  else {
    printf("CO2 Test 1 Failed. Did not read co2_voltage within expected range (400 - 2000).\n");
  }

  /*  CO2 Test 2
   *  Purpose: Checks for error handling in input of get_co2_concentration.
   *  Input: below-bounds co2_voltage
   * Expected: error
   */
  error = FALSE;
  co2_volt = (float) 399;
  co2_conc = get_co2_voltage(co2_volt, &error);
  if (error) {
    printf("CO2 Test 2 Passed. Threw error when given co2_voltage value (399) was outside expected range (400 - 2000).\n");
  }
  else {
    printf("CO2 Test 2 Failed. Did not throw error when given co2_voltage value (399) was outside expected range (400 - 2000).\n");
  }

  /*  CO2 Test 3
   *  Purpose: Checks for error handling in input of get_co2_concentration.
   *  Input: above-bounds co2_voltage
   * Expected: error
   */
  error = FALSE;
  co2_volt = (float) 2401;
  co2_conc = get_co2_voltage(co2_volt, &error);
  if (error) {
    printf("CO2 Test 3 Passed. Threw error when given co2_voltage value (2401) was outside expected range (400 - 2000).\n");
  }
  else {
    printf("CO2 Test 3 Failed. Did not throw error when given co2_voltage value (2401) was outside expected range (400 - 2000).\n");
  }

  /*  CO2 Test 4
   *  Purpose: Checks for expected lower-bound output of get_co2_concentration.
   *  Input: lower-bounds co2_voltage
   * Expected: !error, co2_conc = 0
   */
  error = FALSE;
  co2_volt = (float) 400;
  co2_conc = get_co2_voltage(co2_volt, &error);
  if (!error && (co2_conc == 0))
    printf("CO2 Test 4 Passed. Did not throw error when given co2_voltage value (400) was within expected range (400 - 2000).\n");
  }
  else {
    printf("CO2 Test 4 Failed. Threw error when given co2_voltage value (400) was within expected range (400 - 2000).\n");
  }
  
  /*  CO2 Test 5
   *  Purpose: Checks for expected upper-bound output of get_co2_concentration.
   *  Input: valid co2_voltage
   * Expected: !error, co2_conc = 5000
   */
  error = FALSE;
  co2_volt = (float) 2000;
  co2_conc = get_co2_voltage(co2_volt, &error);
  if (!error && (co2_conc == 5000))
    printf("CO2 Test 4 Passed. Did not throw error when given co2_voltage value (400) was within expected range (400 - 2000).\n");
  }
  else {
    printf("CO2 Test 4 Failed. Threw error when given co2_voltage value (400) was within expected range (400 - 2000).\n");
  }

  /*  CO2 Test 6
   *  Purpose: Checks for error handling in above-bounds output of get_co2_concentration.
   *  Input: valid co2_voltage
   * Expected: error
   */
  error = FALSE;
  co2_volt = (float) 2001;
  co2_conc = get_co2_voltage(co2_volt, &error);
  if (error) {
    printf("CO2 Test 6 Passed. Threw error when calculated co2_voltage concentration was outside expected range (0 - 5000).\n");
  }
  else {
    printf("CO2 Test 6 Failed. Did not throw error when calculated co2_voltage concentration was outside expected range (0 - 5000).\n");
  }

  /*  CO2 Test 7
   *  Purpose: Checks for error handling in above-bounds output of get_co2_concentration.
   *  Input: upper-bound co2_voltage
   * Expected: error
   */
  error = FALSE;
  co2_volt = (float) 2400;
  co2_conc = get_co2_voltage(co2_volt, &error);
  if (error) {
    printf("CO2 Test 7 Passed. Threw error when calculated co2_voltage concentration was outside expected range (0 - 5000).\n");
  }
  else {
    printf("CO2 Test 7 Failed. Did not throw error when calculated co2_voltage concentration was outside expected range (0 - 5000).\n");
  }
}
