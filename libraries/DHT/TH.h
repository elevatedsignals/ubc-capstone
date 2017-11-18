
#ifndef TH_H
#define TH_H

struct TH {
  volatile float h; // humidity
  volatile float t; // temperature
  volatile float hi; // heat index
  volatile int has_data; // whether it obtained data
};

/*
 * Purpose: Determines whether temp and humidity data have been obtained.
 * Output: true or false
 */
int is_ready(struct TH t_h) {
  return t_h.has_data;
}

/*
 * Purpose: Gets the temperature data
 * Output: temperature in Celsius
 */
float get_temp(struct TH t_h) {
  return t_h.t;
}

/*
 * Purpose: Gets the humidity data
 * Output: humidity percentage
 */
float get_humidity(struct TH t_h) {
  return t_h.h;
}

/*
 * Purpose: Calculates and gets the heat index
 * Output: heat index in Celsius
 */
float get_heatindex(struct TH t_h) {
  return t_h.hi;
}

#endif
