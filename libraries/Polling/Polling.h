#ifndef POLLING_H
#define POLLING_H

#include "constants.h"

void wakeUpCommunication()       // here the interrupt is handled after wakeup
{
  detachInterrupt(1);      // disables interrupt 1 on pin 3 so the 
  
  sleep_disable();         // first thing after waking from sleep:
                           // disable sleep...

  power_all_enable();      // re-enable all peripherals            
                             
  delay(1000);             // wait for arduino to fully wake up

  //Run communication protocol...
  Serial.println("  We are now communicating ");    // for testing
}

void wakeUpTimer()         // here the interrupt is handled after wakeup
{
  detachInterrupt(1);      // disables interrupt 1 on pin 3 so the 
  
  sleep_disable();         // first thing after waking from sleep:
                           // disable sleep...

  power_all_enable();      // re-enable all peripherals                            
                             
  delay(1000);             // wait for arduino to fully wake up

  //Run main program...
  Serial.println("  We are now taking readings ");    // for testing
}

void sleepNow()         // here we put the arduino to sleep
{   
  delay(1000);         // ensure the last command is complete before entering sleep mode

  pinMode(RX_PIN, INPUT);
  attachInterrupt(1, wakeUpCommunication, LOW); // use interrupt 1 (pin 3) and run function
                                                   // wakeUpCommunication when pin 3 gets LOW 
  
  set_sleep_mode(SLEEP_MODE_IDLE);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
                           // so sleep is possible. just a safety pin 

  sleep_mode();            // here the device is actually put to sleep!!
                           // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
}

#endif