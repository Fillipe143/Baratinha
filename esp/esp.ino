#include <Adafruit_PCF8574.h>
#include <Adafruit_VL53L0X.h>

#define ARDUINO_ADDR 0x8
#define PCF_ADDR 0x20
#define BASE_XSHUT_ADDR 0x29

#define GO_FORWARD  0
#define GO_BACK     1

#define N_SENSOR 8

bool sensors[] = {
  true,   // VL53L0X 1 F
  true,   // VL53L0X 2 R
  true,  // VL53L0X 3 L
  true,   // VL53L0X 4 BR
  true,   // VL53L0X 5 FL
  true,   // VL53L0X 6 BL
  true,   // VL53L0X 7 FR
  false    // VL53L0X 8 B
};

Adafruit_PCF8574 pcf;
Adafruit_VL53L0X lox[N_SENSOR];
VL53L0X_RangingMeasurementData_t measure;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  setupSensors();
}
void loop() {
  int MIN_DISTANCE = 100;

  if(readSensor(1) < MIN_DISTANCE) {
    motor(GO_BACK, 100, GO_FORWARD, 0);
  } else {
    motor(GO_BACK, 100, GO_BACK, 100);
  }
}

void setupSensors() {
  Serial.println("Iniciando configuração...");

  if (!pcf.begin(PCF_ADDR, &Wire)) {
    Serial.println("Não foi possivel achar o PCF");
    while (true);
  }

  for (uint8_t sensor = 1; sensor <= N_SENSOR; sensor++) {
    Serial.println("Desligando sensor: " + String(sensor));
    pcf.digitalWrite(sensor, LOW);
  }


  for (uint8_t sensor = 1; sensor <= N_SENSOR; sensor++) {
    if (!sensors[sensor - 1]) {
      Serial.println("Ignorando sensor: " + String(sensor));
      continue;
    }
    Serial.println("Ligando sensor: " + String(sensor));
    pcf.digitalWrite(sensor, HIGH);

    if (!lox[sensor - 1].begin()) {
      Serial.println("Falha ao iniciar sensor: " + String(sensor));
      while (true);
    }

    Serial.println("Atualizando endereço do sensor: " + String(sensor));
    Serial.println("Novo endereço: " + String(BASE_XSHUT_ADDR + sensor));
    lox[sensor - 1].setAddress(BASE_XSHUT_ADDR + sensor);
  }


  Serial.println("Configuração terminada com sucesso!");
}

unsigned int readSensor(int sensor) {
  lox[sensor - 1].rangingTest(&measure, false);

  if (measure.RangeStatus != 4) return measure.RangeMilliMeter;
  else return -1;
}

/** Protocolo de comunicação
    
    Utiliza 2 bytes, um para cada motor, sendo o primeiro o da esquerda e o segundo da direita

    O primeiro bit de cada byte simboliza a direção na qual o motor girar, sendo 0 ir para frente e 1 ir para tras
    Os outros 7 bits são utilizados para definir a potência que o motor ira girar, pondendo ela ser de representada de 0 a 100

    Exemplo:
    1 0110011 // Esquerda indo para frente em uma velocidade de 50%
    0 1100100 // Direita indo para tras em uma velocidade de 100%
**/

void motor(bool leftDirection, uint8_t leftSpeed, bool rightDirection, uint8_t rightSpeed) {
  uint8_t leftMotor  = min(leftSpeed,  (uint8_t) 100) + leftDirection  * 0x80;
  uint8_t rightMotor = min(rightSpeed, (uint8_t) 100) + rightDirection * 0x80;

  Wire.beginTransmission(ARDUINO_ADDR);
  Wire.write(leftMotor);
  Wire.write(rightMotor);
  Wire.endTransmission();
}
