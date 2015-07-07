/*
 * UIPEthernet UdpClient example.
 *
 * UIPEthernet is a TCP/IP stack that can be used with a enc28j60 based
 * Ethernet-shield.
 *
 * UIPEthernet uses the fine uIP stack by Adam Dunkels <adam@sics.se>
 *
 *      -----------------
 *
 * This UdpClient example tries to send a packet via udp to 192.168.0.1
 * on port 5000 every 5 seconds. After successfully sending the packet it
 * waits for up to 5 seconds for a response on the local port that has been
 * implicitly opened when sending the packet.
 *
 * Copyright (C) 2013 by Norbert Truchsess (norbert.truchsess@t-online.de)
 */
#include <RF22ReliableDatagram.h>
#include <RF22.h>
#include <SPI.h>
#include <avr/pgmspace.h>
#include <UIPEthernet.h>

#define ENC28J60DEBUG 1
#define RF_SERVER_ADDRESS 0

RF22ReliableDatagram manager(RF_SERVER_ADDRESS, 8, 0);

EthernetUDP udp;

void setup() {

  Serial.begin(9600);
  if (manager.init()) {
    //Serial.println(F("init success"));
  }
  else {
    /*Serial.println(F("init failed"));*/
  }
  uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

  Ethernet.begin(mac);
}

const char nestId[] PROGMEM = "nestId:";
const uint8_t offset PROGMEM =48;
const uint8_t skydome_port=5506;
const char skydome[] PROGMEM = "api.skydome.io";
void loop() {
  uint8_t data[2] PROGMEM;
  if (manager.available()) {
    uint8_t len = 2;
    uint8_t from;

    if (manager.recvfromAck(data, &len, &from)) {
      char message[] PROGMEM = {from + offset,':', data[0] + offset,':', data[1] + offset,'\0'};
      sendMessage(nestId,message);
    }
  }
}


void sendMessage(const char *nestId, char *message) {
  int success = udp.beginPacket(skydome, skydome_port);
  /*Serial.print("beginPacket: ");
  Serial.println(success ? "success" : "failed");*/

  success = udp.write(nestId);
  success = udp.write(message);
  /*Serial.print("bytes written: ");
  Serial.println(success);*/

  success = udp.endPacket();
  /*Serial.print("endPacket: ");
  Serial.println(success ? "success" : "failed");*/
  udp.stop();
}
