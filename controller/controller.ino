
 #include <avr/sleep.h>
 #include <avr/power.h>
 #include "TimerOne.h"
 #include "constants.h"
 #include "Polling.h"

 /*
  * This is the Interrupt Service Routine of the Arduinio Pro Mini
  * The program will initialize by running the setup() function which
  * enables the RX interrupt line and the Timer1 interrupt
  * 
  * RX interrupt will awaken the program from sleep mode for the purpose 
  * of communication 
  * 
  * Timer1 interrupt will awake the program for the purpose of running the 
  * main program (taking readings and recording data)
  * 
  * Once the main code is complete the program will return to the main
  * loop which will put the system back into sleep mode
  */

void setup()
{
  Serial.begin(9600);       // for testing
  Serial.println("start");  // for testing
    
  pinMode(RX_PIN, INPUT);
  attachInterrupt(1, wakeUpCommunication, LOW); // use interrupt 1 (pin 3) and run function
                                                   // wakeUpCommunication when pin 3 gets LOW 
                                      
  Timer1.initialize(ONE_MINUTE/20);           // initialize timer1, and set a 60 second period
  Timer1.attachInterrupt(wakeUpTimer);    // attaches callback() as a timer overflow interrupt                                  
}


void loop()
{
    sleepNow();     // sleep function called here
}
