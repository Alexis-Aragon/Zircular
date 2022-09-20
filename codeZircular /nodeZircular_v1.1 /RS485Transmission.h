//***** define section *****/
#define MAX485_DE1 32
#define MAX485_RE1 33
#define RX_PIN1 26  // connect to converter's RO wire
#define TX_PIN1 25  // connect to converter's DI wire
const uint8_t MODBUS_DEVICE_ID1 = 1;

#define MAX485_DE2 2
#define MAX485_RE2 4
#define RX_PIN2 16  // connect to converter's RO wire
#define TX_PIN2 17  // connect to converter's DI wire
const uint8_t MODBUS_DEVICE_ID2 = 1;

//***** define section *****/
const uint8_t FLOW_REGISTER1A = 1;
// #define FLOW_REGISTER1B 35
const uint8_t FLOW_DATA_SIZE = 4;

SoftwareSerial swSerial1(RX_PIN1, TX_PIN1);
SoftwareSerial swSerial2(RX_PIN2, TX_PIN2);
ModbusMaster sensor1;
ModbusMaster sensor2;

void preTransmission() {
  digitalWrite(MAX485_DE1, HIGH);
  digitalWrite(MAX485_DE2, HIGH);
  digitalWrite(MAX485_RE1, HIGH);
  digitalWrite(MAX485_RE2, HIGH);
  delay(10);
}

void postTransmission() {
  digitalWrite(MAX485_DE1, LOW);
  digitalWrite(MAX485_DE2, LOW);
  digitalWrite(MAX485_RE1, LOW);
  digitalWrite(MAX485_RE2, LOW);
  delay(10);
}


void initSoftwareSerial() {
  pinMode(MAX485_DE1, OUTPUT);
  pinMode(MAX485_DE2, OUTPUT);
  pinMode(MAX485_RE1, OUTPUT);
  pinMode(MAX485_RE2, OUTPUT);

  Serial.println("Welcome");
  swSerial1.begin(9600);
  swSerial2.begin(9600);
  sensor1.begin(MODBUS_DEVICE_ID1, swSerial1);
  sensor1.preTransmission(preTransmission);
  sensor1.postTransmission(postTransmission);
  sensor2.begin(MODBUS_DEVICE_ID2, swSerial2);
  sensor2.preTransmission(preTransmission);
  sensor2.postTransmission(postTransmission);
  delay(10);
}

float steamFlow(uint8_t result) {
  uint8_t j;
  uint16_t buf[FLOW_DATA_SIZE];
  uint16_t temp;
  float flow;

  if (result == sensor1.ku8MBSuccess) {
    Serial.println("Correcto! Procesando...");
    for (j = 0; j < FLOW_DATA_SIZE; j++) {
      buf[j] = sensor1.getResponseBuffer(j);
      Serial.print(buf[j]);
      Serial.print(" ");
    }
    Serial.println("<- Bytes");
    // swap bytes because the data comes in Big Endian!
    temp = buf[1];
    buf[1] = buf[0];
    buf[0] = temp;
    // hand-assemble a single-precision float from the bytestream
    memcpy(&flow, &buf, sizeof(float));
    Serial.print("steam flow is ");
    Serial.println(flow, 6);
    delay(1500);
  } 
  else {
    Serial.print("Failuree. Code: ");
    Serial.println(result, HEX);
  }
  return flow;
  delay(10);
}
