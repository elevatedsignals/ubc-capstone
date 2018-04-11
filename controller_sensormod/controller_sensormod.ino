#include <avr/sleep.h>
#include <avr/power.h>
#include "Constants.h"
#include "DHT.h"
#include "TH.h"
#include "CO2.h"
#include "Airflow.h"
#include "PAR.h"
#include "XBee.h"
#include "SendXBee.h"
//#include "SDCard.h"
#include "TimerOne.h"
//#include "Polling.h"
//#include "TimeLib.h"
#include "Format.h"

/* Sensor module/transmitter code */

//bool gotTime = FALSE; // dont start unless we set current time
//bool SDerror = FALSE;// track whether SD is avail
//struct SD_card sd;

// track whether there is existing data on SD
//volatile bool commFailureOccurred = FALSE;
XBee xbee = XBee();

void wakeUpTimer()         // here the interrupt is handled after wakeup
{
  detachInterrupt(1);      // disables interrupt 1 on pin 3 so the 
  sleep_disable();         // first thing after waking from sleep:
                           // disable sleep...

  power_all_enable();      // re-enable all peripherals                            
                             
  delay(1000);             // wait for arduino to fully wake up

  //Run main program...
  Serial.println();
  Serial.println("We are now taking readings ");    // for testing
  Serial.println();

     bool error;
 /* DHT temperature and humidity code */
  struct TH t_h = init_dht();
  error = FALSE;
  int8_t t = get_temp(t_h, & error);
  if (error) {
    Serial.println(F(ERROR_TEMP));
  }
  else {
    // TODO pass in time rather than NULL
    char *payload = prepare_payload(ID_TEMP, t, NULL);
    sendtoBase(payload, xbee);
  }
  error = FALSE;
  
  
  
  int8_t h = get_humidity(t_h, & error);
  if (error) {
    Serial.println(F(ERROR_HUMIDITY));
  }
  else {
    char *payload = prepare_payload(ID_HUM, h, NULL);
    sendtoBase(payload, xbee);
  }

  /*
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F(" °C"));
  Serial.print(F("Humidity: "));
  Serial.println(h);
  */

  /* CO2 code */
  error = FALSE;
  float co2_volt = get_co2_voltage( & error);
  if (error) {
    Serial.println(F(ERROR_GCO2V));
  }
  error = FALSE;
  short co2_conc = get_co2_concentration(co2_volt, & error);
  if (error) {
    Serial.println(F(ERROR_GCO2C));
    co2_conc = -1000;
  }
  else {
    char *payload = prepare_payload(ID_CO2, co2_conc, NULL);
    sendtoBase(payload, xbee);
  }

/*
  Serial.print(F("CO2 Concentration: "));
  Serial.print(co2_conc);
  Serial.println(F(" ppm"));
*/
  /* Airflow code */
  error = FALSE;
  float airflow_MPH = get_wind_speed( & error);
  if (error) {
    Serial.println(F(ERROR_AIRFLOW));
    airflow_MPH = -1000;
  }
  else {
    bool airflow_bool = get_airflow(airflow_MPH);
    if (airflow_bool) {
      Serial.println(F("Airflow: Yes"));
    } else {
      Serial.println(F("Airflow: No"));
    }

    char *payload = prepare_payload(ID_AF,airflow_bool, NULL);
    sendtoBase(payload, xbee);
  }


  // TODO PAR reading keeps reading 0

  /* PAR SENSOR */ 
  error = FALSE;
  float par_volt = get_par_voltage( & error);
  if (error) {
    Serial.println(ERROR_PARV);
  }

  error = FALSE;
  float par_intensity = get_par_concentration(par_volt, &error);
  if (error) {
    Serial.println(ERROR_PARI);
    par_intensity = -1000;
  }
  else {
    char *payload = prepare_payload(ID_PAR,par_intensity, NULL);
    sendtoBase(payload, xbee);
   
  }
  /*
        Serial.print("PAR Intensity: ");
  Serial.print(par_intensity);
  Serial.println(" umol*m^(-2)*s^(-1)"); */

}
void setup() {

  Serial.begin(9600);
  Serial.println(F("Starting set up . . ."));
  
  xbee.setSerial(Serial);


  /* TODO uncomment this
    // get current time from base station before starting
    int8_t attempt = 0; // wait max 3 sec for time
    while (!gotTime && attempt < 4) {
          delay(1000);
          Serial.println("Waiting for time before starting");
          gotTime = getTime(xbee);
      }
    // set default reference voltage (5V)
    analogReference(DEFAULT);
  */

  /* SD interfacing code */
  //sd = init_sd(TXT_FILE, &SDerror);

  // TODO not working?
  Timer1.initialize(ONE_MINUTE / 6); // TODO change to every 10 min
  Timer1.attachInterrupt(wakeUpTimer);


}

void sleepNow()         // here we put the arduino to sleep
{   
  delay(1000);         // ensure the last command is complete before entering sleep mode


  //Timer1.initialize(ONE_MINUTE / 6);           // initialize timer1, and set a 60 second period
  Timer1.attachInterrupt(wakeUpTimer);    // attaches callback() as a timer overflow interrupt  

  
  set_sleep_mode(SLEEP_MODE_IDLE);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
                           // so sleep is possible. just a safety pin 

  sleep_mode();            // here the device is actually put to sleep!!
                           // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
                        
}

void loop() {

  sleepNow();
  
}



/* Helper Functions */

void sendtoBase(char *msg, XBee xbee) {

  bool error = FALSE;

  if (sendXbeeVerify(msg, xbee)) {
    Serial.println();
    Serial.println(F("Msg sent over xbee"));
    // Error recovery
 //   if (commFailureOccurred) {
      // if old data failed to send, try to send now
 //     commFailureOccurred = recover_sensor_module_data(&sd, xbee);
 //   }
  } else {
    Serial.println();
    Serial.println(F("Msg failed to send over xbee"));
    //commFailureOccurred = TRUE;
    // saves msg in the event that transmission fails
    // and SD card was initialized properly
    //if (!SDerror) {
    //  Serial.println(F("Begin writing to SD"));
    //  write_sensor_module_message(sd, msg, &error);
   // }
  }
}

/*
* Purpose: Get current time from base station
* Output: FALSE if failed to obtain, TRUE otherwise
*/
int8_t getTime(XBee xbee) {
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
      int8_t i;
      // read the whole message
      for (i = 0; i < rx16.getDataLength(), i < PACKET_SIZE; i++) {
        data = rx16.getData(i);
        msg[i] = data;
      }
      msg[i] = '\0';

      //Serial.println(msg);

      // this is a valid msg to handle
      if (strstr(msg, "{") && strstr(msg, "}")) {
        //Serial.println("valid");

        // this message contains the time
       /* if (strstr(msg, "time response") != NULL) {
          char * time = extractTime(msg);
          if (time != NULL) {
            //Serial.println("Received current time: ");
            //Serial.println(time);
            setSystemTime(time);
            return TRUE;
          }

        }*/

      }

    }

  }

  return FALSE;
}

/*
* Purpose: Parse the time receieved and set arduino time
*/
/*
int8_t setSystemTime(char * time) {

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
}*/

/*
* Parse the time from the response
* Output: the full timestamp, or NULL if msg
* did not contain a timestamp
*/
/*
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
}*/

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
  int8_t h;
  int8_t t;

  // initialize DHT sensor
  DHT dht(PIN_DHT, DHTTYPE);
  dht.begin();
  int8_t attempt = 0; // track number of poll attempts

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


