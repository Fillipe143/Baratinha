#include <Adafruit_PCF8574.h>
#include <Adafruit_VL53L0X.h>
#include <MPU6050_light.h>

#define ARDUINO_ADDR 0x08
#define PCF_ADDR 0x20
#define BASE_XSHUT_ADDR 0x29

#define GO_FORWARD 1
#define GO_BACK 0

#define N_SENSOR 8

#define SENSOR_FRONT 1
#define SENSOR_RIGHT 2
#define SENSOR_LEFT 3
#define SENSOR_BACK_RIGHT 4
#define SENSOR_FRONT_LEFT 5
#define SENSOR_BACK_LEFT 6
#define SENSOR_FRONT_RIGHT 7
#define SENSOR_BACK 8
int x = 0;

bool sensors[] = {
  true,   // VL53L0X 1 F
  true,   // VL53L0X 2 R
  true,   // VL53L0X 3 L
  true,   // VL53L0X 4 BR
  true,   // VL53L0X 5 FL
  true,   // VL53L0X 6 BL
  true,   // VL53L0X 7 FR
  false   // VL53L0X 8 B
};

Adafruit_PCF8574 pcf;
Adafruit_VL53L0X lox[N_SENSOR];
VL53L0X_RangingMeasurementData_t measure;
MPU6050 mpu(Wire);

void setup() {
  Wire.begin();
  Serial.begin(115200);
  setupSensors();

  byte status = mpu.begin();
  while (status != 0);

  mpu.calcOffsets();
  
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  int front = readSensor(SENSOR_FRONT);

  if (front <= 200) {
    // Girar
    mpu.update();
    int startAngle = readGyro();
    motor(GO_FORWARD, 100, GO_BACK, 100);
    while (abs(startAngle - readGyro()) <= 90) mpu.update();
    motor(GO_FORWARD, 100, GO_FORWARD, 100);
  } else {
      motor(GO_FORWARD, 100, GO_FORWARD, 100);
  }
}

void rotate(int angle) {
  mpu.update();
  int startAngle = readGyro();

  motor(GO_FORWARD, 100, GO_BACK, 100);
  while (abs(startAngle - readGyro()) <= angle);
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

int readSensor(int sensor) {
  lox[sensor - 1].rangingTest(&measure, false);

  if (measure.RangeStatus != 4) return measure.RangeMilliMeter;
  else return -1;
}

int readGyro() {
  return ((int) mpu.getAngleZ()) % 360;
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
