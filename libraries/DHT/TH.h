
#ifndef TH_H
#define TH_H

#define TRUE 1
#define FALSE 0

struct TH {
  volatile float h; // humidity
  volatile float t; // temperature
};

/*
 * Purpose: Gets the temperature data
 * Output: temperature in Celsius
 */
float get_temp(struct TH t_h, int *error) {
  if (t_h.t != -1000) {
    return t_h.t;
  }
  *error = TRUE;
  return -1000;
}

/*
 * Purpose: Gets the humidity data
 * Output: humidity percentage
 */
float get_humidity(struct TH t_h, int *error) {
  if (t_h.h != -1000) {
    return t_h.h;
  }
  *error = TRUE;
  return -1000;
}

#endif
