
#ifndef TH_H
#define TH_H

#define TRUE 1
#define FALSE 0

struct TH {
  volatile float h; // humidity
  volatile float t; // temperature
  volatile float hi; // heat index
};

/*
 * Purpose: Determines whether temp and humidity data
 * have been obtained.
 * Output: true or false
 */
 /*
int is_dht_ready(struct TH t_h, int *error) {
  if (t_h.has_data == FALSE || t_h.has_data == TRUE) {
    return t_h.has_data;
  }
  *error = TRUE;
  return 0;
}*/

/*
 * Purpose: Gets the temperature data
 * Output: temperature in Celsius
 */
float get_temp(struct TH t_h, int *error) {
  if (t_h.t != -1000) {
    return t_h.t;
  }
  *error = TRUE;
  return 0;
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
  return 0;
}

/*
 * Purpose: Calculates and gets the heat index
 * Output: heat index in Celsius
 */
float get_heatindex(struct TH t_h, int *error) {
  if (t_h.hi != -1000) {
    return t_h.hi;
  }
  *error = TRUE;
  return 0;
}

#endif
