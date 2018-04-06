#ifndef AIRFLOW_H
#define AIRFLOW_H

#include "Constants.h"

/*
 *  Purpose: Polls the airflow Sensor till it obtains a wind speed reading (in MPH)
 *  Output: A float containing the wind speed (MPH)
 */
float get_wind_speed(bool * error) {
  const float zeroWindAdjustment = .2; // negative numbers yield smaller wind speeds and vice versa.

  int TMP_Therm_ADunits; //temp termistor value from wind sensor
  float RV_Wind_ADunits; //RV output from wind sensor
  float RV_Wind_Volts;
  float zeroWind_ADunits;
  float zeroWind_volts;
  float WindSpeed_MPH;

  TMP_Therm_ADunits = analogRead(PIN_AIRFLOW_TMP);
  RV_Wind_ADunits = analogRead(PIN_AIRFLOW_RV);
  RV_Wind_Volts = (RV_Wind_ADunits * 0.0048828125);

  /* these are all derived from regressions from raw data
    as such they depend on a lot of experimental factors
    such as accuracy of temp sensors, and voltage at the
    actual wind sensor, (wire losses) which were unaccouted for.
  */
  zeroWind_ADunits = -0.0006 * ((float) TMP_Therm_ADunits * (float) TMP_Therm_ADunits) + 1.0727 * (float) TMP_Therm_ADunits + 47.172; //  13.0C  553  482.39
  zeroWind_volts = (zeroWind_ADunits * 0.0048828125) - zeroWindAdjustment;

  /*
    This from a regression from data in the form of
    Vraw = V0 + b * WindSpeed ^ c
    V0 is zero wind at a particular temperature
    The constants b and c were determined by some Excel wrangling with the solver.
   */
  WindSpeed_MPH = pow(((RV_Wind_Volts - zeroWind_volts) / .2300), 2.7265);
  if (WindSpeed_MPH < 0) { * error = TRUE;
    return WindSpeed_MPH;
  }
  return WindSpeed_MPH;
}

/*
 *  Purpose: Determines if air circulation is present given a wind speed and WIND_THRESHOLD
 *  Output: A bool containing whether air is flowing
 */
bool get_airflow(float WindSpeed_MPH) {
  if (WindSpeed_MPH > WIND_THRESHOLD)
    return 1;
  else
    return 0;
}

#endif
