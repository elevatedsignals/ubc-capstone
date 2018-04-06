
#ifndef TH_H
#define TH_H

#include "Constants.h"

/* humidity and temp will be initialized to -1000 to
  detect errors obtaining data as those are invalid values */
struct TH {
  volatile int8_t h; // humidity
  volatile int8_t t; // temperature
};

/*
 * Purpose: Gets the temperature data
 * Output: temperature in Celsius
 */
float get_temp(struct TH t_h, int8_t * error) {
  if (t_h.t != -1000) {
    return t_h.t;
  } * error = TRUE;
  return -1000;
}

/*
 * Purpose: Gets the humidity data
 * Output: relative humidity percentage
 */
float get_humidity(struct TH t_h, int8_t * error) {
  // relative humidity must be 0+
  if (t_h.h >= 0 & t_h.h <= 100) {
    return t_h.h;
  } * error = TRUE;
  return -1000;
}

#endif
