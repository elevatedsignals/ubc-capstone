#ifndef POLLING_H
#define POLLING_H

#include "Constants.h"

/*
* Handle interrupt here after waking up from RX interrupt
*/
void wakeUpCommunication() {
    detachInterrupt(1); // disables interrupt 1 on pin 3 so the
    sleep_disable(); // when we wake up, disable sleep
    power_all_enable(); // re-enable all peripherals
    delay(1000); // wait for arduino to fully wake up

    Serial.println("  We are now communicating "); // for testing
  }

/*
* Handle interrupt here after time interval
*/
void wakeUpTimer() {
    detachInterrupt(1); // disables interrupt 1 on pin 3 so the
    sleep_disable();
    power_all_enable(); // re-enable all peripherals
    delay(1000); // wait for arduino to fully wake up

    Serial.println("  We are now taking readings "); // for testing
  }

/*
* Put device to sleep
*/
void sleepNow() {
    delay(1000); // ensure the last command is complete before entering sleep mode
    pinMode(RX_PIN, INPUT);
    attachInterrupt(1, wakeUpCommunication, LOW); // use interrupt 1 (pin 3) and run function

    // wakeUpCommunication when pin 3 gets LOW

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable(); // enables the sleep bit in the mcucr register
    sleep_mode(); // here the device is actually put to sleep!!

    // program continues here after waking up
  }

#endif
