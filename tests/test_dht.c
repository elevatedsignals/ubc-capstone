
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

  struct TH t_h = {-1000,-1000,-1000};
  int error;

/*
  // undefined field
  if (!test_dht_ready(t_h)) {
    printf("Pass 1: test_ready()");
  }
  else {
    printf("Fail 1: test_ready()")
  }

  // data not ready
  t_h.has_data = FALSE;

  if (!test_dht_ready(t_h)) {
    printf("Pass 2: test_ready()");
  }
  else {
    printf("Fail 2: test_not_ready()")
  }

  // data is ready
  t_h.has_data = TRUE;

  if (test_dht_ready(t_h)) {
    printf("Pass 3: test_ready()");
  }
  else {
    printf("Fail 3: test_not_ready()")
  }

*/
  // no temperature available
  error = FALSE;
  float x = get_temp(t_h, &error);
  if (error) {
    printf("Pass 1: temperature\n");
  }
  else {
    printf("Fail 1: temperature\n");
  }

  // temperature obtained
  float t = (float) 27;
  t_h.t = t;
  error = FALSE;
  t = get_temp(t_h, &error);
  if ((!error) & (t == t_h.t)) {
    printf("Pass 2: temperature\n");
  }
  else {
    printf("Fail 2: temperature\n");
  }

  // no humidity available
  error = FALSE;
  get_humidity(t_h, &error);
  if (error) {
    printf("Pass 1: humidity\n");
  }
  else {
    printf("Fail 1: humidity\n");
  }

  // humidity obtained
  float h = (float) 40;
  t_h.h = h;
  error = FALSE;
  h = get_humidity(t_h, &error);
  if ((!error) & (h == t_h.h)) {
    printf("Pass 2: humidity\n");
  }
  else {
    printf("Fail 2: humidity\n");
  }
}


/*
  // no heat index available
  if (!test_heatindex(t_h)) {
    printf("Pass 1: test_heatindex()");
  }
  else {
    printf("Fail 1: test_heatindex()");
  }

  // heat index obtained

  float hi = (float) 30;
  t_h.hi = hi;

  if (test_heatindex(t_h) == hi) {
    printf("Pass 2: test_heatindex()");
  }
  else {
    printf("Fail 2: test_heatindex()");
  }

}
*/
/*

int test_ready(struct TH t_h) {
  error = FALSE;
  is_dht_ready(t_h, &error);
  if (error) {
    return FAIL;
  }
  return PASS;
}

float test_temperature(struct TH t_h) {
  error = FALSE;
  float t = get_temp(t_h, &error);
  if (error) {
    return (float)FAIL;
  }
  return t;
}

int test_humidity(struct TH t_h) {
  error = FALSE;
  float h = get_humidity(t_h, &error);
  if (error) {
    return (float)FAIL;
  }
  return h;
}

int test_heatindex(struct TH t_h) {
  error = FALSE;
  float hi = get_heatindex(t_h, &error);
  if (error) {
    return (float) FAIL;
  }
  return hi;

}*/
