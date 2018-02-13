#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include "constants.h"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

unsigned int localPort = 8888;       // local port to listen for UDP packets

char timeServer[] = "time.nist.gov"; // time.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

EthernetUDP Udp;// A UDP instance to let us send and receive packets over Ethernet


/*
* Purpose: Sends an NTP request to the time server at the given address
*/
void send_NTP_packet(char* address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

/*
* Purpose: Initializes Ethernet and UDP
*/
void initialize_UDP(){
  // start Ethernet and UDP
  if (Ethernet.begin(mac) == 0) {
    Serial.println(ERROR_INITUDP);
    return;
  }
  
  Udp.begin(localPort);
}

/*
* Purpose: Parses the NTP packet and turns the timestamp into the Unix epoch
* Output: unsigned long that is the Unix epoch in seconds
*/
unsigned long parse_NTP_packet(){
  if (Udp.parsePacket()) {
      // We've received a packet, read the data from it
      Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  
      // the timestamp starts at byte 40 of the received packet and is four bytes,
      // or two words, long. First, extract the two words:
  
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      // combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      unsigned long secsSince1900 = highWord << 16 | lowWord;
  
      // now convert NTP time into everyday time:
      // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
      const unsigned long seventyYears = 2208988800UL;
      // subtract seventy years:
      unsigned long epoch = secsSince1900 - seventyYears - EIGHT_HOURS;

      return epoch;
    }
}

/*
* Purpose: Puts the current time in YYYY-MM--DD HH::MM::SS (24-hour format) into a string
* Input: 
* Output: clockTime, contains time in YYYY-MM--DD HH::MM::SS (24-hour format)
*/
String get_formatted_time(){

  String clockTime,years, months, days, hours, minutes, seconds, zero;
  zero = "0";
  
  years = String(year());
  months = String(month());
  days = String(day());

  hours = String(hour());
  if(hour() < 10){
    hours = String(zero + hours);
  }
  
  minutes = String(minute());
  if(minute() < 10){
    minutes = String(zero + minutes);
  }
  
  seconds = String(second());
  if(second() < 10){
    seconds = String(zero + seconds);
  }

  clockTime = String(years + '-' + months + "-" + days + " " + hours + "::" + minutes + "::" + seconds);

  return clockTime;
      
}

#endif
