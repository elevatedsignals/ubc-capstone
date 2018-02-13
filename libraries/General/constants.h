#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TRUE 1
#define FALSE 0

#define MODULE_ID 12345
#define PIN_CO2 A0
#define PIN_DHT 2
#define PIN_SD 10
#define PIN_SD_CHECK 9

#define DHTTYPE DHT22

#define TXT_FILE "data.txt"
#define EIGHT_HOURS 28800
#define DELAY_TIME 1000

#define ERROR_TEMP "Error: DHT Temperature sensor"
#define ERROR_HUMIDITY "Error: DHT Humidity sensor"
#define ERROR_GCO2V "get_co2_voltage error: output outside expected range (400 - 2000)\n"
#define ERROR_GCO2C "get_co2_concentration error: input/output outside expected range (400 - 2000/0 - 5000)\n"
#define ERROR_NOSD "Error: No SD Card Inserted"
#define ERROR_INITSD "Error: Failed to initialize SD Card"
#define ERROR_INITUDP "Error: Failed to configure Ethernet using DHCP"

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


#endif