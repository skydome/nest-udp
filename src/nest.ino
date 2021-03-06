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

#define RF_SERVER_ADDRESS 0

RF22ReliableDatagram manager(RF_SERVER_ADDRESS, 8, 0);

EthernetUDP udp;

uint8_t mac[6] = {0x02, 0x05, 0x02, 0x04, 0x09, 0x05};

void setup() {
  Serial.begin(9600);
  if (manager.init()) {
      Serial.println(F("RF init success"));
  }
  else {
      Serial.println(F("RF init failed"));
  }

  Ethernet.begin(mac);
}

void loop() {
  uint8_t data[5] PROGMEM;
  if (manager.available()) {
    uint8_t len = 5;
    uint8_t from;

    if (manager.recvfromAck(data, &len, &from)) {
      	sendMessage(from,data);
    }
  }
}


void sendMessage(uint8_t from,uint8_t *message) {
  int success = udp.beginPacket("api.skydome.io", 5506);
  Serial.println(success ? F("success") : F("failed"));

  success = udp.write(mac[0]);
  success = udp.write(mac[1]);
  success = udp.write(mac[2]);
  success = udp.write(mac[3]);
  success = udp.write(mac[4]);
  success = udp.write(mac[5]);
  success = udp.write(message[0]);
  success = udp.write(message[1]);
  success = udp.write(message[2]);
  success = udp.write(message[3]);
  success = udp.write(message[4]);

  Serial.print(F("bytes written: "));
  Serial.println(success);


  success = udp.endPacket();
  Serial.print(F("endPacket: "));
  Serial.println(success ? F("success") : F("failed"));
  udp.stop();
}
