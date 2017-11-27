#ifndef CO2_H
#define CO2_H

#define TRUE 1
#define FALSE 0

float get_concentration(float co2, int *error) {
  if (co2 != -1000) {
    return co2;
  }
  *error = TRUE;
  return -1000;
}

#endif
