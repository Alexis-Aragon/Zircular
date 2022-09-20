#include <SPI.h>
#include <LoRa.h>
#include "stdio.h"

//***** pines conexion LoRa *****//
#define SCK 18
#define MISO 19
#define MOSI 23
#define CS 5
#define RST 22
#define dio0 14


/************ initialize LoRa Sender *************************/
void setupLora() {
  while (!Serial);
  Serial.println("LoRa Sender");
  //setup LoRa transceiver module
  LoRa.setPins(CS, RST, dio0);
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
  // LoRa.setTxPower(20);
  // LoRa.setSignalBandwidth(250E3);
  // LoRa.setSpreadingFactor(12);
  Serial.println("LoRa Initializing OK!");
}


/************** Envio de datos por LoRa *************************/

void dataSender(String variables[], int numberVariables) {
    Serial.println("Sending packet: ");
    LoRa.beginPacket();
    for (int i = 0; i < numberVariables; i++) {
      Serial.println(variables[i]);
      LoRa.print(variables[i]);
      LoRa.print(" ");
    }
    LoRa.endPacket();  
}