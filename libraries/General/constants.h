
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TRUE 1
#define FALSE 0

#define PIN_DHT 2
#define PIN_CO2 A0
#define PIN_AIRFLOW 0
#define PIN_PAR 0

#define DHTTYPE DHT22

#define ERROR_GCO2V "get_co2_voltage error: output outside expected range (400 - 2000)\n"
#define ERROR_GCO2C "get_co2_concentration error: input/output outside expected range (400 - 2000/0 - 5000)\n"

#endif
