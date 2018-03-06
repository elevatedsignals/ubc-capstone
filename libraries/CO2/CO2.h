#ifndef CO2_H
#define CO2_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

/*
 *  Purpose: Polls the Infrared CO2 Sensor till it obtains a sensor voltage
 *  Input: A float containing the CO2 sensor voltage in millivolts (mV)
 *  Output: A float containing the CO2 sensor voltage
 */
float get_co2_voltage(int *error) {
  int sensor_value;

  int attempt = 0; // track number of poll attempts

  // try to poll data for 1 sec max
  do {
    // can only poll every 100 milliseconds
    delay(100);
    // read voltage
    sensor_value = analogRead(PIN_CO2);
    attempt++;
  } while(isnan(sensor_value) && attempt < 10);

  // signal error if value is still invalid after 10th attempt
  if(isnan(sensor_value) && attempt >= 10) {
    *error = TRUE;
    return (float)sensor_value;
  }

  // analog signal is converted to a voltage value in millivolts (mV)
  float voltage = sensor_value * (5000 / 1023.0);

  // check for invalid output
  if (voltage < 400 || voltage > 2000) {
    *error = TRUE;
    return voltage;
  }

  return voltage;
}

/*
 *  Purpose: Converts the Infrared CO2 sensor voltage to its equivalent CO2
 *           concentration in ppm
 *  Input: A float containing the CO2 sensor voltage
 *  Output: A float containing the CO2 concenctration in ppm
 */
float get_co2_concentration(float voltage, int *error) {
  // check for invalid input
  if (voltage < 400 || voltage > 2000) {
    *error = TRUE;
    return voltage;
  }

  int voltage_difference = voltage - 400;
  float co2_conc = voltage_difference * (50.0 / 16.0);

  //check for invalid output
  if (co2_conc < 0 || co2_conc > 5000) {
    *error = TRUE;
    return co2_conc;
  }

  return co2_conc;
}

#endif
