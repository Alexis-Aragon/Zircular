//***** include section *****//

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>

//*******************************
#include "RS485Transmission.h"
#include "dataSenderLora.h"
#include "muestreoADC.h"

const long interval = 5000;
unsigned long tiempo;
unsigned long start_time;
// Instancia a las clases OneWire y DallasTemperature
float temper;
float temper2;

OneWire ourWire(27);  //Se establece el pin 4  como bus OneWire

DallasTemperature sensors(&ourWire);  //Se declara una variable u objeto para nuestro sensor


void setup() {
  delay(500);
  tiempo = millis();
  start_time = millis();  
  Serial.begin(115200);
  sensors.begin();  //Se inicia el sensor de temperatura
  setupLora();
  initSoftwareSerial();
  setupSD();
}

void loop() {
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
    String variables[] = { String(temper,2), String(temper2,2), String(flow1,2), String(flow2,2), nombre };
    int numberVariables = sizeof(variables) / sizeof(String);
    dataSender(variables, numberVariables);

    tiempo = millis();
  }
  
}
