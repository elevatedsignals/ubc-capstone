#ifndef CO2_H
#define CO2_H

struct CO2 {
  volatile float concentration;
  volatile int has_data; // whether it obtained data
};

/*
 * Purpose: Determines whether CO2 concentration value been obtained.
 * Output: true or false
 */
int is_co2_ready(struct CO2 co2_h) {
  return co2_h.has_data;
}

/*
 * Purpose: Gets the CO2 concentration value
 * Output: CO2 concentration in ppm
 */
float get_concentration(struct CO2 co2_h) {
  return co2_h.concentration;
}

#endif
