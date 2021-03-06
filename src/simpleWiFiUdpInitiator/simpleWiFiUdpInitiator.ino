/*

   Simple Wifi UDP Initiator

   Call a UDP follower (server); send a string, and wait for a reply
   Works with WiFiUdpSendReceiveString unmodified

   Based on Udp NTP Client by Michael Margolis

   By Michael Shiloh

   Change log
   10/2/18 - MS - Initial Entry

   This code is in the public domain.

*/

#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen for UDP packets

unsigned int followerPort = 2390;
IPAddress followerAddr(192, 168, 1, 111); /// CHANGE THIS

const int PACKET_SIZE = 50; //arbitrary; don't forget this

char packetBuffer[ PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait for connection:
    delay(5000);
  }

  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
  Udp.begin(localPort);
}

void loop()
{
  Serial.println("prepareing packet for server");
  //send a packet
  Udp.beginPacket(followerAddr, followerPort);
  Udp.write(pass, 10);
  Udp.endPacket();
 
  //Serial.print ("Checking incoming ...  ");
  // wait to see if a reply is available

  if ( Udp.parsePacket() ) {
    Serial.println("packet received: ");
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, PACKET_SIZE); // read the packet into the buffer
    //Serial.println((char *)packetBuffer);

    //char temp[10];
    //Serial.println(itoa(packetBuffer[0], temp, 10));
    Serial.println(packetBuffer);
  } else {
    Serial.println("... Nothing received");
  }
  delay(100);
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}










