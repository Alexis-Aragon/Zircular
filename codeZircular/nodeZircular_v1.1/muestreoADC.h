
/******************* Seccion include *************************/
#include <driver/adc.h>
#include <Arduino.h>
// #include <esp_adc_cal.h>
#include "sdFunction.h"


/******************* variables globales **********************/
int valorADC;


unsigned long tiempoMuestreo;
#define samplesLenght 100000
String dataMessage;

// Para usar otros pines SPI


/*************************************************************************************
  setupSD --> esta funcion va en el setup principal, iniciliza la SD y
  crea un archivo donde guardar las muestras
*/
void setupSD() {
  initSDCard();
  delay(50);
}


/*************************************************************************************
   muestreoADC --> funcion para tomar las muestras desde el ADC y guardarlas en la SD
*/
char nombre[13];
unsigned int contador = 0;

void crearArchivo() {
  sprintf(nombre, "/data%04d.CSV", contador);  
  while (SD.exists(nombre)) {
    if (++contador > 9999) contador = 0;
    sprintf(nombre, "/data%04d.CSV", contador);
    if (!contador) break;
  }
  File file = SD.open(nombre);  // abrimos el archivos donde se guardaran las muestras

  if (!file)  // si no existe el archivo lo creamos
  {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, nombre, "LecturasADC \r\n");
  }
  file.close();
}


void muestreoADC() {
  crearArchivo();  
  adc1_config_width(ADC_WIDTH_12Bit);  // configuramos el ancho de captura del ADC

  File file = SD.open(nombre, FILE_APPEND);  // abrimos el archivo durante la toma de muestras
  Serial.printf("Abriendo archivo: %s\n", nombre);
  
  tiempoMuestreo = millis();
  
    for (int i = 0; i < samplesLenght; i++) {
      valorADC = adc1_get_raw(ADC1_CHANNEL_7);  // lectura ADC del gpio 35
      dataMessage = String(valorADC) + "\r\n";  // concatenamos la info
      // Serial.print("Saving data: ");
      // Serial.println(dataMessage);
      if (!file) {
        Serial.println("Failed to open file for appending");
        return;
      }
      if (file) {
        file.print(dataMessage);  //
        // Serial.print("Message appended");
      } else {
        Serial.println("Append failed");
      }
    }
  
  Serial.println(millis() - tiempoMuestreo);  // calculamos el tiempo de muestreo
  file.close();      // cerramos el archivo
}