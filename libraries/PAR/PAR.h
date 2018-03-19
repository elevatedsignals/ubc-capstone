#ifndef PAR_H
#define PAR_H

#include "Constants.h"
#include <stdio.h>
#include <stdlib.h>

/*
 *  Purpose: Polls the PAR Sensor till it obtains a sensor voltage in millivolts
 *           (mV)
 *  Output: A float containing the PAR sensor voltage (mV)
 */
float get_par_voltage(int * error) {
  int sensor_value;

  int attempt = 0; // track number of poll attempts

  // try to poll data for 1 sec max
  do {
    // can only poll every 100 milliseconds
    delay(100);
    // read voltage
    sensor_value = analogRead(PIN_PAR);
    attempt++;
  } while (isnan(sensor_value) && attempt < 10);

  // signal error if value is still invalid after 10th attempt
  if (isnan(sensor_value) && attempt >= 10) { * error = TRUE;
    return (float) sensor_value;
  }

  // analog signal is first calibrated to account for amplifier gain
  // R = 440, G = 1 + (49.4e3)/R
  float voltage = sensor_value / (1 + (49400 / 440.0));

  // next, signal is converted to voltage value in millivolts (mV)
  voltage *= (5000 / 1023.0);

  // check for invalid output
  if (voltage < 0 || voltage > 40) { * error = TRUE;
    return voltage;
  }

  return voltage;
}

/*
 *  Purpose: Converts the PAR sensor voltage in millivolts (mV) to its
 *           equivalent light intensity in micromole per square meter second
 *           (umol*m^(-2)*s^(-1)))
 *  Input: A float containing the PAR sensor voltage (mV)
 *  Output: A float containing the light intensity (umol*m^(-2)*s^(-1)))
 */
float get_par_concentration(float voltage, int * error) {
  // check for invalid input
  if (voltage < 0 || voltage > 40) { * error = TRUE;
    return voltage;
  }

  // convert voltage to equivalent light intensity value
  float light_intensity = voltage * 100.0;

  // check for invalid output
  if (light_intensity < 0 || light_intensity > 4000) { * error = TRUE;
    return light_intensity;
  }

  return light_intensity;
}

#endif
