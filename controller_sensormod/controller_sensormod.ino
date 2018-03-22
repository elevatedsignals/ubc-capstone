#include <avr/sleep.h>
#include <avr/power.h>
#include "constants.h"
#include "DHT.h"
#include "TH.h"
#include "CO2.h"
#include "airflow.h"
#include "PAR.h"
#include "XBee.h"
#include "SendXBee.h"
#include "SDCard.h"
#include "TimerOne.h"
#include "Polling.h"
#include "TimeLib.h"

/* Sensor module/transmitter code */

int gotTime = FALSE; // dont start unless we set current time
// track whether there is existing data on SD
volatile int commFailureOccurred = FALSE;

void setup() {

  Serial.begin(9600);

  /* Uncomment
  XBee xbee = XBee();
  xbee.setSerial(Serial);
  */

  pinMode(RX_PIN, INPUT); // TODO is this needed? its also called in sleepNow()

  // Uncomment
  // interrupt 1) wake up on xbee communication
  //attachInterrupt(1, wakeUpCommunication, LOW);
  // interrupt 2) timer interrupt on 60 second interval
  //Timer1.initialize(ONE_MINUTE / 20); // TODO change to every 10 min
  //Timer1.attachInterrupt(wakeUpTimer);

  /* TODO uncomment this
    // get current time from base station before starting
    while (!gotTime) {
          delay(1000);
          Serial.println("Waiting for time before starting");
          gotTime = getTime(xbee);
      }
    // set default reference voltage (5V)
    analogReference(DEFAULT);
  */

  int error;
  /* DHT temperature and humidity code */
  struct TH t_h = init_dht();
  error = FALSE;
  float t = get_temp(t_h, & error);
  if (error) {
    Serial.println(ERROR_TEMP);
  }
  error = FALSE;
  float h = get_humidity(t_h, & error);
  if (error) {
    Serial.println(ERROR_HUMIDITY);
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.println(h);

  /* CO2 code */
  error = FALSE;
  float co2_volt = get_co2_voltage( & error);
  if (error) {
    Serial.println(ERROR_GCO2V);
  }
  error = FALSE;
  float co2_conc = get_co2_concentration(co2_volt, & error);
  if (error) {
    Serial.println(ERROR_GCO2C);
    co2_conc = -1000;
  }

  Serial.print("CO2 Concentration: ");
  Serial.print(co2_conc);
  Serial.println(" ppm");

  /* Airflow code */
  error = FALSE;
  float airflow_MPH = get_wind_speed( & error);
  if (error) {
    Serial.println(ERROR_AIRFLOW);
    airflow_MPH = -1000;
  }
  bool airflow_bool = get_airflow(airflow_MPH);
  if (airflow_bool) {
    Serial.println("Airflow: Yes");
  } else {
    Serial.println("Airflow: No");
  }

  /* TODO PAR SENSOR */
  error = FALSE;
  float par_volt = get_par_voltage( & error);
  if (error) {
    Serial.println(ERROR_PARV);
  }
  error = FALSE;
  float par_intensity = get_par_concentration(par_volt, & error);
  if (error) {
    Serial.println(ERROR_PARI);
    par_intensity = -1000;
  }

  Serial.print("PAR Intensity: ");
  Serial.print(par_intensity);
  Serial.println(" umol*m^(-2)*s^(-1)");

  /* SD interfacing code */
  int SDerror = FALSE;
  struct SD_card sd = init_sd(TXT_FILE, &SDerror); // TODO we shouldnt initialize everytime when we refactor this

  /* XBee Wireless Communication */
  // TODO format data for xbee like this needs to start with { and end with }
  char *msg = "{ uniqueid: 123, timestamp: 00:12:14 }";
  error = FALSE;
  /* Uncomment
  if (sendXbeeVerify(msg, xbee)) {
    // Error recovery
    if (commFailureOccurred) {
      // if old data failed to send, try to send now
      commFailureOccurred = recover_sensor_module_data(&sd, xbee);
    }
    Serial.println("Msg sent over xbee");
  } else {
    commFailureOccurred = TRUE;
    Serial.println("Msg failed to send over xbee");
    // saves msg in the event that transmission fails
    // and SD card was initialized properly
    if (!SDerror) {
      Serial.println("Begin writing to SD");
      write_sensor_module_message(sd, msg, &error);
    }
  }
  */

  SDerror = FALSE;
  read_sd(sd, &SDerror);
}

void loop() {

  sleepNow(); // go to sleep and wake up on either timer/xbee interrupt

  // TODO put main code here

}

/*
* Purpose: Get current time from base station
* Output: FALSE if failed to obtain, TRUE otherwise
*/
int getTime(XBee xbee) {
  Rx16Response rx16 = Rx16Response();

  char request[PACKET_SIZE] = "";
  strcpy(request, "{ request time, srcAddr:");
  strcat(request, SRC_ADDRESS);
  strcat(request, "}");

  // send time request
  if (!sendXbeeVerify(request, xbee)) {
    return FALSE;
  }

  // wait for response containing timestamp
  uint8_t data = 0;

  xbee.readPacket(2000); // wait max 2 secs

  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      // got RX packet with 16 bit address
      xbee.getResponse().getRx16Response(rx16);

      char msg[PACKET_SIZE] = "";
      int i;
      // read the whole message
      for (i = 0; i < rx16.getDataLength(), i < PACKET_SIZE ;i++) {
        data = rx16.getData(i);
        msg[i] = data;;
      }
      msg[i] = '\0';

      Serial.println(msg);

      // this is a valid msg to handle
      if (strstr(msg, "{") && strstr(msg, "}")) {
        Serial.println("valid");

        // this message contains the time
        if (strstr(msg, "time response") != NULL) {
          char * time = extractTime(msg);
          if (time != NULL) {
            Serial.println("Received current time: ");
            Serial.println(time);
            setSystemTime(time);
            return TRUE;
          }

        }

      }

    }

  }

  return FALSE;
}

/*
* Purpose: Parse the time receieved and set arduino time
*/
int setSystemTime(char * time) {

  char hr[3];
  memcpy(hr, & time[0], 2);
  hr[2] = '\0';
  char minute[3];
  memcpy(minute, & time[4], 2);
  hr[2] = '\0';
  char sec[3];
  memcpy(sec, & time[8], 2);
  sec[2] = '\0';
  char day[3];
  memcpy(day, & time[11], 2);
  day[2] = '\0';
  char month[3];
  memcpy(month, & time[14], 2);
  month[2] = '\0';
  char yr[5];
  memcpy(yr, & time[17], 4);
  yr[4] = '\0';

  setTime(atoi(hr), atoi(minute), atoi(sec), atoi(day), atoi(month), atoi(yr));

  return 0;
}

/*
* Parse the time from the response
* Output: the full timestamp, or NULL if msg
* did not contain a timestamp
*/
char * extractTime(char * message) {

  char msg[PACKET_SIZE];
  strcpy(msg, message);

  char * time;
  char * token;

  // format should be {time response, current time=<time>}
  // cant use ':' as timestamp will contain that so wont parse properly
  if (strstr(msg, "current time") != NULL) {
    token = strtok(msg, "}");
    token = strstr(token, "=");
    memmove(token, token + 1, strlen(token) - 1);
    time = token;

    return time;
  }

  return NULL;
}

/*
 * Purpose: initializes the DHT sensor and polls the sensor till it obtains
 * temperature and humidity data. This needs to be in a .ino file
 * (not c file) as DHT library code is in c++.
 * Output: the TH struct containing all DHT sensor data
 */
struct TH init_dht(void) {

  /* to determine whether a temp and humidity reading was obtained, we
   initialize them to -1000 since those are invalid values */
  struct TH t_h = {-1000,
    -1000
  };
  float h;
  float t;

  // initialize DHT sensor
  DHT dht(PIN_DHT, DHTTYPE);
  dht.begin();
  int attempt = 0; // track number of poll attempts

  // try to poll data for 20 secs max
  do {
    // can only poll every 2 seconds
    delay(500);

    // reading temperature or humidity takes about 250 ms
    h = dht.readHumidity();
    // read temperature as Celsius (default)
    t = dht.readTemperature();
    attempt++;
  } while ((isnan(h) || isnan(t)) & attempt < 5);

  if (!isnan(t)) {
    t_h.t = t;
  }
  if (!isnan(h)) {
    t_h.h = h;
  }
  return t_h;
}
