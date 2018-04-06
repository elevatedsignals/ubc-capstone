#include "../libraries/CO2/CO2.h"
#include "../libraries/General/Constants.h"
#include <stdio.h>

int error;

/*
 *  Unit tests for the Gravity IR CO2 Sensor
 */
int main(void) {
  float co2_volt, co2_conc;

  /*  CO2 Test 1
   *  Purpose: Checks for standard output of get_co2_voltage.
   *  Expected: !error
   */
  error = FALSE;
  co2_volt = get_co2_voltage(&error);
  if(!error && (co2_volt > 399.9) && (co2_volt < 2000.1)) {
    printf("CO2 Test 1 Passed. Read CO2 voltage value within expected range (400 - 2000).\n");
  }
  else {
    printf("CO2 Test 1 Failed. Did not read CO2 voltage value within expected range (400 - 2000).\n");
  }

  /*  CO2 Test 2
   *  Purpose: Checks for error handling in input of get_co2_concentration.
   *  Input: A below-bounds CO2 voltage value
   *  Expected: error
   */
  error = FALSE;
  co2_volt = 399.9;
  co2_conc = get_co2_concentration(co2_volt, &error);
  if(error) {
    printf("CO2 Test 2 Passed. Threw error when given CO2 voltage value (399) was outside expected range (400 - 2000).\n");
  }
  else {
    printf("CO2 Test 2 Failed. Did not throw error when given CO2 voltage value (399) was outside expected range (400 - 2000).\n");
  }

  /*  CO2 Test 3
   *  Purpose: Checks for error handling in input of get_co2_concentration.
   *  Input: An above-bounds CO2 voltage value
   *  Expected: error
   */
  error = FALSE;
  co2_volt = 2000.1;
  co2_conc = get_co2_concentration(co2_volt, &error);
  if(error) {
    printf("CO2 Test 3 Passed. Threw error when given CO2 voltage value (2401) was outside expected range (400 - 2000).\n");
  }
  else {
    printf("CO2 Test 3 Failed. Did not throw error when given CO2 voltage value (2401) was outside expected range (400 - 2000).\n");
  }

  /*  CO2 Test 4
   *  Purpose: Checks for expected lower-bound output of get_co2_concentration.
   *  Input: A lower-bounds CO2 voltage value
   *  Expected: !error, co2_conc = 0
   */
  error = FALSE;
  co2_volt = 400.0;
  co2_conc = get_co2_concentration(co2_volt, &error);
  if(!error && (co2_conc == 0.0)) {
    printf("CO2 Test 4 Passed. Did not throw error when given CO2 voltage value (400) was within expected range (400 - 2000).\n");
  }
  else {
    printf("CO2 Test 4 Failed. Threw error when given CO2 voltage value (400) was within expected range (400 - 2000).\n");
  }

  /*  CO2 Test 5
   *  Purpose: Checks for expected upper-bound output of get_co2_concentration.
   *  Input: An upper-bound CO2 voltage value
   *  Expected: !error, co2_conc = 5000
   */
  error = FALSE;
  co2_volt = 2000.0;
  co2_conc = get_co2_concentration(co2_volt, &error);
  if(!error && (co2_conc == 5000.0)) {
    printf("CO2 Test 5 Passed. Did not throw error when given CO2 voltage value (400) was within expected range (400 - 2000).\n");
  }
  else {
    printf("CO2 Test 5 Failed. Threw error when given CO2 voltage value (400) was within expected range (400 - 2000).\n");
  }

  /*  CO2 Test 6
   *  Purpose: Checks for expected output of get_co2_concentration.
   *  Input: A CO2 voltage value in the expected range (400 - 2000) e.g. 1200.5
   *  Expected: !error, (e.g.) co2_conc = 2501.5625
   */
  error = FALSE;
  co2_volt = 1200.5;
  co2_conc = get_co2_concentration(co2_volt, &error);
  if(!error && (co2_conc > 2501.562) && (co2_conc < 2501.563)) {
    printf("CO2 Test 6 Passed. Did not throw error when given CO2 voltage value (1200.5) was within expected range (400 - 2000).\n");
  }
  else {
    printf("CO2 Test 6 Failed. Threw error when given CO2 voltage value (1200.5) was within expected range (400 - 2000).\n");
  }
}
