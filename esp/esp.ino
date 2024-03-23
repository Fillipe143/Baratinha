#include <Adafruit_PCF8574.h>
#include <Adafruit_VL53L0X.h>

#define ARDUINO_ADDR 0x8
#define PCF_ADDR 0x20
#define BASE_XSHUT_ADDR 0x29

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
    Serial.println("Direita");
    moveMotor('d');
  } else {
    Serial.println("Frente");
    moveMotor('f');
  }
}

void moveMotor(char direction) {
  Wire.beginTransmission(ARDUINO_ADDR);
  Wire.write(direction);
  Wire.endTransmission();
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
