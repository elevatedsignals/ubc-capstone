#ifndef POLLING_H
#define POLLING_H

#include "Constants.h"

/*
* Handle interrupt here after time interval
*/
void wakeUpTimer() {
    detachInterrupt(1); // disables interrupt 1 on pin 3 so the
    sleep_disable();
    power_all_enable(); // re-enable all peripherals
    delay(DELAY_TIME); // wait for arduino to fully wake up

    Serial.println();
    Serial.println();
    Serial.println(F("Interrupt: Timer wake up")); // for testing
  }

/*
* Put device to sleep
*/
void sleepNow() {
    delay(DELAY_TIME); // ensure the last command is complete before entering sleep mode

    Serial.println();
    Serial.println(F("Interrupt: Entering sleep mode"));
    Serial.println();
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable(); // enables the sleep bit in the mcucr register
    sleep_mode(); // here the device is actually put to sleep!!

    // program continues here after waking up
  }

#endif
