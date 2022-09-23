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

/*
 * Publicar datos en el servidor
 */
long lastMsg = 0;
char msg[42];

void PublishData(String Data) {
  Data.toCharArray(msg, 42);
  Serial.print("Publicamos mensaje -> ");
  Serial.println(msg);
  client.publish("values", msg);
}
String mensaje; 
String dataString(String datos[], int numberVariables){
  mensaje = "";
  for (int i = 0; i < numberVariables; i++){
    mensaje += datos[i];
    mensaje += " ";            
  }
  return mensaje;
  delay(10);
}

/************** Envio de datos por LoRa *************************/

void dataSenderLora(String mensajeSender) {
  Serial.println("Sending packet: ");
  LoRa.beginPacket();
  LoRa.print(mensajeSender);
  LoRa.endPacket();  
}