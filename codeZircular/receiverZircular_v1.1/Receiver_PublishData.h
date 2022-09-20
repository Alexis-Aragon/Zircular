/*
 * Publicar datos en el servidor
 */
long lastMsg = 0;
char msg[25];

void PublishData(String LoRaData) {

  if (tiempo > lastMsg + 500) {

    // String registros;
    // String to_send = String(temper1) + "," + String(temper2);
    LoRaData.toCharArray(msg, 25);

    Serial.print("Publicamos mensaje -> ");
    Serial.println(msg);
    client.publish("values", msg);

    lastMsg = millis();

  }
}


/*********************************************************************
 * Configuracion de comunicacion por LoRa
 */
#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

void setupLora()
{
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  LoRa.setGain(6);
  // LoRa.setSpreadingFactor(12);
  // LoRa.setSignalBandwidth(250E3);

  Serial.println("LoRa Initializing OK!");
}


//******************* recibir datos por LoRa **********************************
void receiverData() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.println(LoRaData);
      // splitData(LoRaData);
      PublishData(LoRaData);
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  return;
}
