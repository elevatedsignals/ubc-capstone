#ifndef CO2_H
#define CO2_H

#define TRUE 1
#define FALSE 0

/*
 * Purpose: Polls the Infrared CO2 Sensor till it obtains a sensor value and calculates
 *          the CO2 concentration value.
 * Output: A float contraining CO2 concentration in ppm or a respective error
 */
float get_co2_voltage(int *error) {
  int sensor_value;

  int attempt = 0; // track number of poll attempts

  // try to poll data for 1 sec max
  do {
    // can only poll every 100 milliseconds
    delay(100);
    // read voltage
    sensor_value = analogRead(PIN_CO2);
    attempt++;
  } while(isnan(sensor_value) & attempt < 10);

  // analog signal is converted to a voltage value
  float voltage = sensor_value * (5000 / 1024.0);

  if (voltage < 400 || voltage > 2000){
    *error = TRUE;
    return -1000; 
  }
  return voltage;
}

float get_co2_concentration(float voltage){

  int voltage_difference = voltage - 400;
  float co2_conc = voltage_difference * (50.0 / 16.0);

  return co2_conc;
}

#endif
