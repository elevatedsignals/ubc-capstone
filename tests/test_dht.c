
#include "../libraries/DHT/TH.h"
#include <stdio.h>

#define PASS 1
#define FAIL 0

#define TRUE 1
#define FALSE 0

/*
 * Unit tests for the DHT temperature and humidity sensor
 */
int main(void) {

  struct TH t_h = {-1000,
    -1000
  };
  int error;

  // no temperature available, should return error
  error = FALSE;
  float x = get_temp(t_h, & error);
  if (error) {
    printf("Pass 1: temperature\n");
  } else {
    printf("Fail 1: temperature\n");
  }

  // temperature obtained, should return it
  error = FALSE;
  float t = (float) 50;
  t_h.t = t;
  t = get_temp(t_h, & error);
  if ((!error) & (t == t_h.t)) {
    printf("Pass 2: temperature\n");
  } else {
    printf("Fail 2: temperature\n");
  }

  // negative temperature is valid, should return it
  error = FALSE;
  t = (float) - 10;
  t_h.t = t;
  t = get_temp(t_h, & error);
  if ((!error) & (t == t_h.t)) {
    printf("Pass 3: temperature\n");
  } else {
    printf("Fail 3: temperature\n");
  }

  // no humidity available, should return error
  error = FALSE;
  get_humidity(t_h, & error);
  if (error) {
    printf("Pass 1: humidity\n");
  } else {
    printf("Fail 1: humidity\n");
  }

  // humidity obtained, should return it
  error = FALSE;
  float h = (float) 40;
  t_h.h = h;
  h = get_humidity(t_h, & error);
  if ((!error) & (h == t_h.h)) {
    printf("Pass 2: humidity\n");
  } else {
    printf("Fail 2: humidity\n");
  }

  // negative humidity is invalid, should return error
  error = FALSE;
  h = (float) - 1;
  t_h.h = h;
  get_humidity(t_h, & error);
  if (error) {
    printf("Pass 3: humidity\n");
  } else {
    printf("Fail 3: humidity\n");
  }

  // humidity over 100 is invalid, should return error
  error = FALSE;
  h = (float) 101;
  t_h.h = h;
  get_humidity(t_h, & error);
  if (error) {
    printf("Pass 4: humidity\n");
  } else {
    printf("Fail 4: humidity\n");
  }
}
