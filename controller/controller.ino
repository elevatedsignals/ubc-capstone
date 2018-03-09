/*

 Udp NTP Client adapted from : https://www.arduino.cc/en/Tutorial/UdpNtpClient


 */
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Time.h>
#include <TimeLib.h>
#include "constants.h"
#include "timestamp.h"

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Initializing UDP");
  initialize_UDP();

  send_NTP_packet(timeServer); // send an NTP packet to a time server

  // wait to see if a reply is available
  delay(DELAY_TIME);

  // Set the time accounting for the de!qlay
  setTime(parse_NTP_packet() + ((DELAY_TIME) / 1000));

}

void loop() {
  String formattedTime;
  
  formattedTime = get_formatted_time();
  Serial.println(formattedTime);
  // wait ten seconds before asking for the time again
  Ethernet.maintain();

  delay(DELAY_TIME);
}

