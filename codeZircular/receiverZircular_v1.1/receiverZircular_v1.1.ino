#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>
#include "SetupWifiAndServer.h"
#include "Receiver_PublishData.h"


//***********************************************************************
float temper1 = 0;
float temper2 = 0;


byte ByteArray[255];
int ByteData[20];

//*************************************************************************************

void setup() {

  tiempo = millis();
  tiempoReconnect = millis();
  lastMsg = millis();

  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  setupLora();
  
  randomSeed(micros());
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}
bool state = true;
//****************************************************************************************

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  receiverData();

}
