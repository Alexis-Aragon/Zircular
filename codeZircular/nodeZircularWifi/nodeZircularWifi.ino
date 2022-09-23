//***** include section *****//

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>

//*******************************
#include "deepSleep.h"
#include "SetupWifiAndServer.h"
#include "RS485Transmission.h"
#include "dataSender.h"
#include "muestreoADC.h"

const long interval = 5000;
// unsigned long tiempo;
unsigned long start_time;
// Instancia a las clases OneWire y DallasTemperature
float temper;
float temper2;

OneWire ourWire(27);  //Se establece el pin 4  como bus OneWire

DallasTemperature sensors(&ourWire);  //Se declara una variable u objeto para nuestro sensor


void setup() {
  delay(500);
  // se inician las variables de tiempo
  tiempo = millis();
  start_time = millis();
  tiempoReconnect = millis();

  Serial.begin(115200);
  while (!Serial){
    ;
  }
  sensors.begin();  //Se inicia el sensor de temperatura
                    // Se configuran el Lora, softwareserial, las Sd, y el Wifi
  // setupLora();
  setup_wifi();
  initSoftwareSerial();
  setupSD();


  // confiurar el cliente
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()){
    reconnect();
  }
  client.loop();

  if (millis() - tiempo >= interval) {
    sensors.requestTemperatures();  //Se envía el comando para leer la temperatura
    postTransmission();             //recibo de la transmision del rs485 al esp32 para que configure la salida de transmision

    Serial.println("Reading registers");
    uint8_t readRegister1 = sensor1.readHoldingRegisters(FLOW_REGISTER1A, FLOW_DATA_SIZE);
    float flow1 = steamFlow(readRegister1);
    uint8_t readRegister2 = sensor2.readHoldingRegisters(FLOW_REGISTER1A, FLOW_DATA_SIZE);
    float flow2 = steamFlow(readRegister2);

    temper = sensors.getTempCByIndex(0);   //Se obtiene la temperatura en ºC
    temper2 = sensors.getTempCByIndex(1);  //Se obtiene la temperatura en ºC

    muestreoADC();
    String variables[] = { String(temper, 2), String(temper2, 2), String(flow1, 2), String(flow2, 2), nombre };
    int numberVariables = sizeof(variables) / sizeof(String);
    
    dataString(variables, numberVariables);
    Serial.println(mensaje);
    // dataSenderLora(mensaje);
    PublishData(mensaje);
    tiempo = millis();
  }
}