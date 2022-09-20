/*********************************************************************
 * Configuración y conexion a la red wifi
 */
const char* ssid     = "ZIONING";
const char* password = "zion-ing";

void setup_wifi() 
{
  delay(10);
  // Nos conectamos a nuestra red Wifi
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a red WiFi!");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
}


/********************************************************************
 *  Funcion para volver a conectarse al servidor mqtt 
 */

//*******************************************************************
const char *mqtt_server = "zioning.ml";
const int mqtt_port = 1883;
const char *mqtt_user = "web_client";
const char *mqtt_pass = "121212";

unsigned long tiempo;
unsigned long tiempoReconnect;

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() 
{
  while (!client.connected()) 
  {
    Serial.print("Intentando conexion Mqtt...");
    // Creamos un cliente ID
    String clientId = "esp32_";
    clientId += String(random(0xffff), HEX);
    // Intentamos conectar
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) 
    {
      Serial.println("Conectado!");
      // Nos suscribimos
      client.subscribe("led1");
      client.subscribe("led2");
    }
    else 
    {
      if (tiempo > tiempoReconnect + 5000) 
      {
        Serial.print("fallo :( con error -> ");
        Serial.print(client.state());
        Serial.println(" Intentamos de nuevo en 5 segundos");
        tiempoReconnect = millis();
      }
    }
  }
}


/********************************************************************/
void callback(char* topic, byte* payload, unsigned int length) 
{
  String incoming = "";
  Serial.print("Mensaje recibido desde -> ");
  Serial.print(topic);
  Serial.println("");
  //for (int i = 0; i < length; i++) {
  //incoming += (char)payload[i];
  //}
  incoming.trim();
  Serial.println("Mensaje -> " + incoming);
}
